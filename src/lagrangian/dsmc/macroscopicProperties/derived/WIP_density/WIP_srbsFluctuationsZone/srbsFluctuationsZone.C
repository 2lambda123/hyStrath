/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016-2021 hyStrath
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of hyStrath, a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description

\*---------------------------------------------------------------------------*/

#include "srbsFluctuationsZone.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(srbsFluctuationsZone, 0);

addToRunTimeSelectionTable(dsmcField, srbsFluctuationsZone, dictionary);



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
srbsFluctuationsZone::srbsFluctuationsZone
(
    Time& t,
    const polyMesh& mesh,
    dsmcCloud& cloud,
    const dictionary& dict
)
:
    dsmcField(t, mesh, cloud, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    fieldName_(propsDict_.lookup("fieldName")),
    regionName_(propsDict_.lookup("zoneName")),
    regionId_(-1),
    resetCounter_(0.0),
    domainLength_(),
    Rt_(0.0),
    RtField_(time_.totalNAvSteps()+1, 0.0)
{

    // standard to reading typeIds ------------
    const List<word> molecules (propsDict_.lookup("typeIds"));

    DynamicList<word> moleculesReduced(0);

    forAll(molecules, i)
    {
        const word& moleculeName(molecules[i]);

        if(findIndex(moleculesReduced, moleculeName) == -1)
        {
            moleculesReduced.append(moleculeName);
        }
    }

    moleculesReduced.shrink();

    typeIds_.setSize(moleculesReduced.size(), -1);

    forAll(moleculesReduced, i)
    {
        const word& moleculeName(moleculesReduced[i]);

        label typeId(findIndex(cloud_.typeIdList(), moleculeName));

        if(typeId == -1)
        {
            FatalErrorIn("srbsFluctuationsZone::srbsFluctuationsZone()")
                << "Cannot find typeId: " << moleculeName << nl << "in: "
                << t.system()/"fieldPropertiesDict"
                << exit(FatalError);
        }

        typeIds_[i] = typeId;
    }
    // ---------------------------------------------------


    const cellZoneMesh& cellZones = mesh_.cellZones();

    regionId_ = cellZones.findZoneID(regionName_);

    if(regionId_ == -1)
    {
        FatalErrorIn("srbsFluctuationsZone::srbsFluctuationsZone()")
            << "Cannot find region: " << regionName_ << nl << "in: "
            << time_.time().system()/"fieldPropertiesDict"
            << exit(FatalError);
    }

   // create bin model
    binModel_ = autoPtr<binModel>
    (
        binModel::New(mesh, propsDict_)
    );

    const label& nBins = binModel_->nBins();
    n_.setSize(nBins, 0.0);
    rt_.setSize(nBins, 0.0);

    scalar writeInterval = readScalar(t.controlDict().lookup("writeInterval"));
    scalar deltaT = t.deltaT().value();
    nInstantSteps_ = label(writeInterval/deltaT);

    if(Pstream::master())
    {
        RtField_.setSize(nInstantSteps_, 0.0);
    }

    timeIndex_ = 0;

    scalar binWidth = binModel_->binPositions()[1]-binModel_->binPositions()[0];
    domainLength_ = binWidth*nBins;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

srbsFluctuationsZone::~srbsFluctuationsZone()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- initial conditions
void srbsFluctuationsZone::createField()
{
    Info << "Initialising srbsFluctuationsZone field" << endl;
}


void srbsFluctuationsZone::calculateField()
{

    const List< DynamicList<dsmcParcel*> >& cellOccupancy
        = cloud_.cellOccupancy();

    const labelList& cells = mesh_.cellZones()[regionId_];

    forAll(cells, c)
    {
        const label& cellI = cells[c];
        const List<dsmcParcel*>& parcelsInCell = cellOccupancy[cellI];

        forAll(parcelsInCell, pIC)
        {
            dsmcParcel* p = parcelsInCell[pIC];

            const vector& rI = p->position();

            label n = binModel_->isPointWithinBin(rI, cellI);

            if(n != -1)
            {
                if(findIndex(typeIds_, p->typeId()) != -1)
                {
                    if(cloud_.axisymmetric())
                    {
                        const point& cC = cloud_.mesh().cellCentres()[cellI];
                        scalar radius = cC.y();

                        scalar RWF = 1.0 + cloud_.maxRWF()*(radius/cloud_.radialExtent());

                        n_[n] += cloud_.nParticle()*RWF;
                    }
                    else
                    {
                        n_[n] += cloud_.nParticle();
                    }
                }
            }
        }
    }

    Rt_ = 0.0;
    const scalarField& binCentres = binModel_->binPositions();

    forAll(binCentres, n)
    {
        scalar volume = binModel_->binVolume(n);
        rt_[n] = (n_[n]/volume)*sin(twoPi*binCentres[n]/domainLength_);
        Rt_ += rt_[n];
    }

    Rt_ /= binModel_->nBins();

    RtField_[timeIndex_] = Rt_;
    timeIndex_++;
    n_ = scalar(0.0);
}


//- write field
void srbsFluctuationsZone::writeField()
{
    const Time& runTime = time_.time();

    if(runTime.outputTime())
    {
        timeIndex_ = 0;

        if(Pstream::master())
        {
            fileName timePath(runTime.path()/runTime.timeName()/"uniform");

            if (!isDir(timePath))
            {
                mkDir(timePath);
            }

            scalar dt = time_.time().deltaT().value();
            scalarField timeField (nInstantSteps_, 0.0);

            forAll(timeField, t)
            {
                timeField[nInstantSteps_-t-1] = time_.time().timeOutputValue()-dt*t;
            }

            writeTimeData
            (
                casePath_,
                "densityFluctuations_"+fieldName_+"_"+regionName_+"_Rt.xy",
                timeField,
                RtField_,
                true
            );
        }
    }
}



void srbsFluctuationsZone::updateProperties(const dictionary& newDict)
{
    //- the main properties should be updated first
    updateBasicFieldProperties(newDict);

}

void srbsFluctuationsZone::setProperties()
{

}

} // End namespace Foam

// ************************************************************************* //

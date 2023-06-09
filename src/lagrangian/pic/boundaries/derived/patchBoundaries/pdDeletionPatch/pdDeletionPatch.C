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

#include "pdDeletionPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(pdDeletionPatch, 0);

addToRunTimeSelectionTable(pdPatchBoundary, pdDeletionPatch, dictionary);



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
pdDeletionPatch::pdDeletionPatch
(
    Time& t,
    const polyMesh& mesh,
    pdCloud& cloud,
    const dictionary& dict
)
:
    pdPatchBoundary(t, mesh, cloud, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    //timeIndex_(0),
    //writeInterval_(readScalar(t.controlDict().lookup("writeInterval"))),
    //writeIntSteps_((t.endTime().value() - t.startTime().value())/writeInterval_),
    //startTime_(t.startTime().value()),
    //deletedMassFlux_(writeIntSteps_+1, 0.0),
    allSpecies_(false),
    typeIds_()
    //massFlux_(0.0)
{
    measurePropertiesAtWall_ = false;
    writeInTimeDir_ = false;
    writeInCase_ = true;

    setProperties();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

pdDeletionPatch::~pdDeletionPatch()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
void pdDeletionPatch::initialConfiguration()
{}

void pdDeletionPatch::calculateProperties()
{}

void pdDeletionPatch::controlParticle(pdParcel& p, pdParcel::trackingData& td)
{
//     if(allSpecies_ || (findIndex(typeIds_, p.typeId()) != -1) )
//     {
//         const pdParcel::constantProperties& constProp = cloud_.constProps(p.typeId());
        //const scalar& massI = constProp.mass();
        //massFlux_ += massI;

        td.keepParticle = false;
//     }
//     else // reflect
//     {
//         const label& faceI = p.face();
//         vector nF = mesh_.faceAreas()[faceI];
//         nF /= mag(nF);
//
//         scalar Un = p.U() & nF;
//
//         if (Un > 0.0)
//         {
//             p.U() -= 2.0*Un*nF;
//         }
//     }
}

void pdDeletionPatch::output
(
    const fileName& fixedPathName,
    const fileName& timePath
)
{
//     if (Pstream::parRun())
//     {
//         reduce(massFlux_, sumOp<scalar>());
//     }
//
//     if(faces_.size() > 0)
//     {
//         deletedMassFlux_[timeIndex_] = massFlux_/writeInterval_;
//     }
//
//     timeIndex_++;
//     massFlux_ = 0.0;
//
//     scalarField writeTimes(writeIntSteps_+1, 0.0);
//
//     forAll(writeTimes, tT)
//     {
//         writeTimes[tT] = startTime_ + tT*writeInterval_;
//     }
//
//     Info << "writing out " << endl;
//
//     writeTimeData(fixedPathName, "massFluxDeleted", writeTimes, deletedMassFlux_);
}

void pdDeletionPatch::updateProperties(const dictionary& newDict)
{
    //- the main properties should be updated first
    updateBoundaryProperties(newDict);

    setProperties();
}



void pdDeletionPatch::setProperties()
{
    if (propsDict_.found("allSpecies"))
    {
        allSpecies_ = Switch(propsDict_.lookup("allSpecies"));
    }

    if(!allSpecies_)
    {
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

        typeIds_.clear();

        typeIds_.setSize(moleculesReduced.size(), -1);

        forAll(moleculesReduced, i)
        {
            const word& moleculeName(moleculesReduced[i]);

            label typeId(findIndex(cloud_.typeIdList(), moleculeName));

            if(typeId == -1)
            {
                FatalErrorIn("pdDeletionPatch::pdDeletionPatch()")
                    << "Cannot find typeId: " << moleculeName << nl << "in: "
                    << mesh_.time().system()/"boundariesDict"
                    << exit(FatalError);
            }

            typeIds_[i] = typeId;
        }
    }
}


} // End namespace Foam

// ************************************************************************* //

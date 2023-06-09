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

The wall temperature is a function of the heat flux from incident parcels.

The radiation equation Q = epsilon*sigma*T^4 is used to calculate the wall temperature.

This class gives a value of temperature for each face on the patch.

\*---------------------------------------------------------------------------*/

#include "dsmcRadiationWallFieldPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(dsmcRadiationWallFieldPatch, 0);

addToRunTimeSelectionTable(dsmcPatchBoundary, dsmcRadiationWallFieldPatch, dictionary);



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
dsmcRadiationWallFieldPatch::dsmcRadiationWallFieldPatch
(
    Time& t,
    const polyMesh& mesh,
    dsmcCloud& cloud,
    const dictionary& dict
)
:
    dsmcPatchBoundary(t, mesh, cloud, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    radiativeT_
    (
        IOobject
        (
            "radiativeWallTemperature",
            time_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("0.0", dimTemperature, 0.0)
    ),
    EcTot_(nFaces_, 0.0),
    EcTotSum_(nFaces_, 0.0),
    TwallRad_(nFaces_, 0.0),
    epsilonSigma_(0.0),
    thermalCapacity_(0.0),
    alpha_(0.0),
    stepCounter_(0.0),
    resetFieldsAtOutput_(false)
{
    writeInTimeDir_ = false;
    writeInCase_ = false;
    measurePropertiesAtWall_ = true;

    setProperties();

    TwallRad_ = temperature_;

}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

dsmcRadiationWallFieldPatch::~dsmcRadiationWallFieldPatch()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
void dsmcRadiationWallFieldPatch::initialConfiguration()
{}


void dsmcRadiationWallFieldPatch::calculateProperties()
{
    stepCounter_++;

    const scalar deltaT = mesh_.time().deltaTValue(); // TODO cloud_.deltaTValue(p.cell());

    forAll(TwallRad_, f)
    {
        EcTotSum_[f] += EcTot_[f];
        EcTot_[f] = 0.0;

        if(EcTotSum_[f] > VSMALL) // zero face temperature not allowed!
        {
            const label& faceI = faces_[f];
            scalar fA = mag(mesh_.faceAreas()[faceI]);

            TwallRad_[f] =
            pow
            (
                (alpha_*cloud_.nParticle()*EcTotSum_[f]/(deltaT*stepCounter_*epsilonSigma_*fA)), 0.25
            );
        }
    }

    if(time_.time().outputTime())
    {
        label wppIndex = patchId_;

        forAll(TwallRad_, f)
        {
            radiativeT_.boundaryFieldRef()[wppIndex][f] = TwallRad_[f];
        }

        //- reset
        if(resetFieldsAtOutput_)
        {
            stepCounter_ = 0.0;
            EcTotSum_ = 0.0;
        }
    }
}

void dsmcRadiationWallFieldPatch::controlParticle(dsmcParcel& p, dsmcParcel::trackingData& td)
{
    measurePropertiesBeforeControl(p);

    vector& U = p.U();

    scalar& ERot = p.ERot();

    labelList& vibLevel = p.vibLevel();

    label typeId = p.typeId();

    scalar m = cloud_.constProps(typeId).mass();

    scalar preIE = 0.5*m*(U & U) + ERot;

    forAll(vibLevel, i)
    {
        preIE += vibLevel[i]*physicoChemical::k.value()*cloud_.constProps(typeId).thetaV()[i];
    }

    label faceId = findIndex(faces_, p.face());

    vector nw = p.normal();
    nw /= mag(nw);

    // Normal velocity magnitude
    scalar U_dot_nw = U & nw;

    // Wall tangential velocity (flow direction)
    vector Ut = U - U_dot_nw*nw;

    Random& rndGen(cloud_.rndGen());

    while (mag(Ut) < SMALL)
    {
        // If the incident velocity is parallel to the face normal, no
        // tangential direction can be chosen.  Add a perturbation to the
        // incoming velocity and recalculate.

        U = vector
        (
            U.x()*(0.8 + 0.2*rndGen.sample01<scalar>()),
            U.y()*(0.8 + 0.2*rndGen.sample01<scalar>()),
            U.z()*(0.8 + 0.2*rndGen.sample01<scalar>())
        );

        U_dot_nw = U & nw;

        Ut = U - U_dot_nw*nw;
    }

    // Wall tangential unit vector
    vector tw1 = Ut/mag(Ut);

    // Other tangential unit vector
    vector tw2 = nw^tw1;

    const scalar& T = TwallRad_[faceId];

    scalar mass = cloud_.constProps(typeId).mass();

    scalar rotationalDof = cloud_.constProps(typeId).rotationalDegreesOfFreedom();

    scalar vibrationalDof = cloud_.constProps(typeId).nVibrationalModes();

    U =
        sqrt(physicoChemical::k.value()*T/mass)
       *(
            rndGen.GaussNormal<scalar>()*tw1
          + rndGen.GaussNormal<scalar>()*tw2
          - sqrt(-2.0*log(max(1 - rndGen.sample01<scalar>(), VSMALL)))*nw
        );

    U += velocity_;

    ERot = cloud_.equipartitionRotationalEnergy(T, rotationalDof);

    vibLevel = cloud_.equipartitionVibrationalEnergyLevel(T, vibrationalDof, typeId);

    measurePropertiesAfterControl(p, 0.0);

    scalar postIE = 0.5*m*(U & U) + ERot;

    forAll(vibLevel, i)
    {
        postIE += vibLevel[i]*physicoChemical::k.value()*cloud_.constProps(typeId).thetaV()[i];
    }

    EcTot_[faceId] += (preIE - postIE);
}

void dsmcRadiationWallFieldPatch::output
(
    const fileName& fixedPathName,
    const fileName& timePath
)
{

}

void dsmcRadiationWallFieldPatch::updateProperties(const dictionary& newDict)
{
    //- the main properties should be updated first
    updateBoundaryProperties(newDict);

    propsDict_ = newDict.subDict(typeName + "Properties");

    setProperties();

}

void dsmcRadiationWallFieldPatch::setProperties()
{
    velocity_ = propsDict_.lookup("velocity");
    temperature_ = readScalar(propsDict_.lookup("temperature"));
    scalar epsilon = readScalar(propsDict_.lookup("emissivity"));
    resetFieldsAtOutput_ = propsDict_.lookup("resetAtOutput");

    epsilonSigma_ = epsilon*physicoChemical::sigma.value();

    thermalCapacity_ = readScalar(propsDict_.lookup("thermalCapacity"));

    alpha_ = readScalar(propsDict_.lookup("energyAccommodationCoeff"));

    if(alpha_ < 0 || alpha_ > 1)
    {

        FatalErrorIn("dsmcRadiationWallFieldPatch::setProperties()")
            << "The value of energyAccommodationCoeff should be between 0 and 1: "
            << alpha_ << nl
            << exit(FatalError);
    }
}

} // End namespace Foam

// ************************************************************************* //

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

#include "dsmcDiffuseWallClassificationPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(dsmcDiffuseWallClassificationPatch, 0);

addToRunTimeSelectionTable(dsmcPatchBoundary, dsmcDiffuseWallClassificationPatch, dictionary);



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
dsmcDiffuseWallClassificationPatch::dsmcDiffuseWallClassificationPatch
(
    Time& t,
    const polyMesh& mesh,
    dsmcCloud& cloud,
    const dictionary& dict
)
:
    dsmcPatchBoundary(t, mesh, cloud, dict),
    propsDict_(dict.subDict(typeName + "Properties"))
{
    writeInTimeDir_ = false;
    writeInCase_ = false;
    measurePropertiesAtWall_ = true;

    setProperties();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

dsmcDiffuseWallClassificationPatch::~dsmcDiffuseWallClassificationPatch()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
void dsmcDiffuseWallClassificationPatch::initialConfiguration()
{

}

void dsmcDiffuseWallClassificationPatch::calculateProperties()
{

}

void dsmcDiffuseWallClassificationPatch::controlParticle(dsmcParcel& p, dsmcParcel::trackingData& td)
{
    measurePropertiesBeforeControl(p);

    vector& U = p.U();

    scalar& ERot = p.ERot();

    labelList& vibLevel = p.vibLevel();

    label typeId = p.typeId();

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

    const scalar& T = temperature_;

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

    //- "Class I or class III molecules are progressively
    //- transformed into class II molecule when it interacts with the
    //- body surface."
    p.classification() = 1;

    measurePropertiesAfterControl(p, 0.0);
}

void dsmcDiffuseWallClassificationPatch::output
(
    const fileName& fixedPathName,
    const fileName& timePath
)
{
}


void dsmcDiffuseWallClassificationPatch::updateProperties(const dictionary& newDict)
{
    //- the main properties should be updated first
    updateBoundaryProperties(newDict);

    propsDict_ = newDict.subDict(typeName + "Properties");

    setProperties();

}

void dsmcDiffuseWallClassificationPatch::setProperties()
{
    velocity_ = propsDict_.lookup("velocity");
    temperature_ = readScalar(propsDict_.lookup("temperature"));
}

} // End namespace Foam

// ************************************************************************* //

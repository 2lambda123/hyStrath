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

#include "dsmcCLLWallBirdFieldPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(dsmcCLLWallBirdFieldPatch, 0);

addToRunTimeSelectionTable(dsmcPatchBoundary, dsmcCLLWallBirdFieldPatch, dictionary);



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
dsmcCLLWallBirdFieldPatch::dsmcCLLWallBirdFieldPatch
(
    Time& t,
    const polyMesh& mesh,
    dsmcCloud& cloud,
    const dictionary& dict
)
:
    dsmcPatchBoundary(t, mesh, cloud, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    normalAccommodationCoefficient_(readScalar(propsDict_.lookup("normalAccommodationCoefficient"))),
    tangentialAccommodationCoefficient_(readScalar(propsDict_.lookup("tangentialAccommodationCoefficient"))),
    rotationalEnergyAccommodationCoefficient_(readScalar(propsDict_.lookup("rotationalEnergyAccommodationCoefficient"))),
    boundaryT_
    (
        volScalarField
        (
            IOobject
            (
                "boundaryT",
                mesh_.time().timeName(),
                mesh_,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh_
        )
    ),
    boundaryU_
    (
        volVectorField
        (
            IOobject
            (
                "boundaryU",
                mesh_.time().timeName(),
                mesh_,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh_
        )
    )
{
    writeInTimeDir_ = false;
    writeInCase_ = false;
    measurePropertiesAtWall_ = true;

    setProperties();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

dsmcCLLWallBirdFieldPatch::~dsmcCLLWallBirdFieldPatch()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
void dsmcCLLWallBirdFieldPatch::initialConfiguration()
{
    if((normalAccommodationCoefficient_ < VSMALL) && (tangentialAccommodationCoefficient_ < VSMALL))
    {
        measurePropertiesAtWall_ = false;
    }
}

void dsmcCLLWallBirdFieldPatch::calculateProperties()
{

}

void dsmcCLLWallBirdFieldPatch::controlParticle(dsmcParcel& p, dsmcParcel::trackingData& td)
{
    measurePropertiesBeforeControl(p);

    vector& U = p.U();

    label typeId = p.typeId();

    scalar& ERot = p.ERot();

    labelList& vibLevel = p.vibLevel();

    label wppIndex = p.patch(p.face());

    const polyPatch& patch = mesh_.boundaryMesh()[wppIndex];

    label wppLocalFace = patch.whichFace(p.face());

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

    const scalar& T = boundaryT_.boundaryField()[wppIndex][wppLocalFace];

    scalar mass = cloud_.constProps(typeId).mass();

//     scalar rotationalDof = cloud_.constProps(typeId).rotationalDegreesOfFreedom();

        scalar vibrationalDof = cloud_.constProps(typeId).nVibrationalModes();

        const scalar& alphaT = tangentialAccommodationCoefficient_*(2.0 - tangentialAccommodationCoefficient_);

        const scalar& alphaN = normalAccommodationCoefficient_;

        scalar mostProbableVelocity = sqrt(2.0*physicoChemical::k.value()*T/mass);

            //normalising the incident velocities

    vector normalisedTangentialVelocity = Ut/mostProbableVelocity;

    scalar normalisedNormalVelocity = U_dot_nw/mostProbableVelocity;


    //normal random number components

    scalar thetaNormal = 2.0*pi*rndGen.sample01<scalar>();

    scalar rNormal = sqrt(-alphaN*log(rndGen.sample01<scalar>()));


    //tangential random number components

    scalar thetaTangential = 2.0*pi*rndGen.sample01<scalar>();

    scalar rTangential = sqrt(-alphaT*log(rndGen.sample01<scalar>()));

    //selecting post-collision velocity components

    scalar um = sqrt(1.0-alphaN)*normalisedNormalVelocity;

    scalar normalVelocity = sqrt(
                                    (rNormal*rNormal)
                                    + (um*um)
                                    + 2.0*rNormal*um*cos(thetaNormal)
                                );

    scalar tangentialVelocity1 = sqrt(1.0 - alphaT)*mag(normalisedTangentialVelocity)
                                + rTangential*cos(thetaTangential);

    scalar tangentialVelocity2 = rTangential*sin(thetaTangential);


    //setting the post interaction velocity

    U =
        mostProbableVelocity
       *(
            tangentialVelocity1*tw1
          + tangentialVelocity2*tw1
          - normalVelocity*nw
        );

//     Info << "U before wall addition = " << U << endl;

    vector uWallNormal = (boundaryU_.boundaryField()[wppIndex][wppLocalFace] & nw) * nw;
    vector uWallTangential1 = (boundaryU_.boundaryField()[wppIndex][wppLocalFace] & tw1) * tw1;
    vector uWallTangential2 = (boundaryU_.boundaryField()[wppIndex][wppLocalFace] & tw2) * tw2;
    vector UNormal = ((U & nw) * nw) + uWallNormal*normalAccommodationCoefficient_;
    vector UTangential1 = (U & tw1) * tw1 + uWallTangential1*alphaT;
    vector UTangential2 = (U & tw2) * tw2 + uWallTangential2*alphaT;


    U = UNormal + UTangential1 + UTangential2;

// 	if(wppLocalFace == 0)
// 	{
// 		if(Pstream::parRun())
//         {
// 			Pout << "Scattering angle, 0 mm = " << atan(U.y()/U.x()) << endl;
// 		}
// 		else
// 		{
// 			Info << "Scattering angle, 0 mm = " << atan(U.y()/U.x()) << endl;
// 		}
// 	}
//
// 	if(wppLocalFace == 26)
// 	{
// 		if(Pstream::parRun())
//         {
// 			Pout << "Scattering angle, 5 mm = " << atan(U.y()/U.x()) << endl;
// 		}
// 		else
// 		{
// 			Info << "Scattering angle, 5 mm = " << atan(U.y()/U.x()) << endl;
// 		}
// 	}

    if( (p.position().x() > 0.002) && ( p.position().x() < 0.0022) )
    {
        if(Pstream::parRun())
        {
            Pout << "Scattering angle, 0 mm = " << atan(U.y()/U.x()) << endl;
        }
        else
        {
            Info << "Scattering angle, 0 mm = " << atan(U.y()/U.x()) << endl;
        }
    }

    if( (p.position().x() > 0.0068) && ( p.position().x() < 0.0072 ) )
    {
        if(Pstream::parRun())
        {
            Pout << "Scattering angle, 5 mm = " << atan(U.y()/U.x()) << endl;
        }
        else
        {
            Info << "Scattering angle, 5 mm = " << atan(U.y()/U.x()) << endl;
        }
    }

    scalar om = sqrt( (ERot*(1.0 - rotationalEnergyAccommodationCoefficient_)) / (physicoChemical::k.value()*T));

    scalar rRot = sqrt(-rotationalEnergyAccommodationCoefficient_*(log(max(1.0 - rndGen.sample01<scalar>(), VSMALL))));

    scalar cosThetaRot = cos(2.0*pi*rndGen.sample01<scalar>());

    ERot = physicoChemical::k.value()*T*((rRot*rRot) + (om*om) + (2.0*rRot*om*cosThetaRot));

    vibLevel = cloud_.equipartitionVibrationalEnergyLevel(T, vibrationalDof, typeId);

    measurePropertiesAfterControl(p, 0.0);
}

void dsmcCLLWallBirdFieldPatch::output
(
    const fileName& fixedPathName,
    const fileName& timePath
)
{

}

void dsmcCLLWallBirdFieldPatch::updateProperties(const dictionary& newDict)
{
//     Info << "old dictionary: " << propsDict_  << endl;

    //- the main properties should be updated first
    updateBoundaryProperties(newDict);

    propsDict_ = newDict.subDict(typeName + "Properties");

    setProperties();

}

void dsmcCLLWallBirdFieldPatch::setProperties()
{
}

} // End namespace Foam

// ************************************************************************* //

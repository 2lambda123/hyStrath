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

\*---------------------------------------------------------------------------*/

#include "VariableHardSphere.H"
#include "constants.H"
#include "addToRunTimeSelectionTable.H"

using namespace Foam::constant::mathematical;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


namespace Foam
{
    defineTypeNameAndDebug(VariableHardSphere, 0);
    addToRunTimeSelectionTable(BinaryCollisionModel, VariableHardSphere, dictionary);
};



Foam::VariableHardSphere::VariableHardSphere
(
    const dictionary& dict,
    pdCloud& cloud
)
:
    BinaryCollisionModel(dict, cloud),
    coeffDict_(dict.subDict(typeName + "Coeffs")),
    Tref_(readScalar(coeffDict_.lookup("Tref")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


Foam::VariableHardSphere::~VariableHardSphere()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


bool Foam::VariableHardSphere::active() const
{
    return true;
}



Foam::scalar Foam::VariableHardSphere::sigmaTcR
(
    const pdParcel& pP,
    const pdParcel& pQ
) const
{
//     const dmscCloud& cloud(this->owner());
    label typeIdP = pP.typeId();
    label typeIdQ = pQ.typeId();


    scalar dPQ =
        0.5
       *(
            cloud_.constProps(typeIdP).d()
          + cloud_.constProps(typeIdQ).d()
        );

    scalar omegaPQ =
        0.5
       *(
            cloud_.constProps(typeIdP).omega()
          + cloud_.constProps(typeIdQ).omega()
        );

    scalar cR = mag(pP.U() - pQ.U());

    if (cR < VSMALL)
    {
        return 0;
    }

    scalar mP = cloud_.constProps(typeIdP).mass();

    scalar mQ = cloud_.constProps(typeIdQ).mass();

    scalar mR = mP*mQ/(mP + mQ);

    // calculating cross section = pi*dPQ^2, where dPQ is from Bird, eq. 4.79
    scalar sigmaTPQ =
        pi*dPQ*dPQ
       *pow(2.0*physicoChemical::k.value()*Tref_/(mR*cR*cR), omegaPQ - 0.5)
       /exp(Foam::lgamma(2.5 - omegaPQ));

    return sigmaTPQ*cR;
}



void Foam::VariableHardSphere::collide
(
    pdParcel& pP,
    pdParcel& pQ
)
{
//     pdCloud& cloud_(this->owner());

    label typeIdP = pP.typeId();
    label typeIdQ = pQ.typeId();
    vector& UP = pP.U();
    vector& UQ = pQ.U();

    Random& rndGen(cloud_.rndGen());

    scalar mP = cloud_.constProps(typeIdP).mass();

    scalar mQ = cloud_.constProps(typeIdQ).mass();

    vector Ucm = (mP*UP + mQ*UQ)/(mP + mQ);

    scalar cR = mag(UP - UQ);

    scalar cosTheta = 2.0*rndGen.sample01<scalar>() - 1.0;

    scalar sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    scalar phi = twoPi*rndGen.sample01<scalar>();

    vector postCollisionRelU =
        cR
       *vector
        (
            cosTheta,
            sinTheta*cos(phi),
            sinTheta*sin(phi)
        );

    UP = Ucm + postCollisionRelU*mQ/(mP + mQ);

    UQ = Ucm - postCollisionRelU*mP/(mP + mQ);
}

const Foam::dictionary& Foam::VariableHardSphere::coeffDict() const
{
    return coeffDict_;
}
// ************************************************************************* //

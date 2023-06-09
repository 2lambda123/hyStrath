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
Class
    Foam::NoBinaryCollision

Description
    No collison BinaryCollision Model

\*---------------------------------------------------------------------------*/

#ifndef NoBinaryCollision_H
#define NoBinaryCollision_H

#include "BinaryCollisionModel.H"
#include "dsmcCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                      Class NoBinaryCollision Declaration
\*---------------------------------------------------------------------------*/

class NoBinaryCollision
:
    public BinaryCollisionModel
{
    // Private data

        //- The coefficients dictionary
        const dictionary coeffDict_;

public:

    //- Runtime type information
    TypeName("NoBinaryCollision");


    // Constructors

        //- Construct from dictionary
        NoBinaryCollision
        (
            const dictionary& dict,
            dsmcCloud& cloud
        );


    //- Destructor
        ~NoBinaryCollision();


    // Member Functions

        //- Flag to indicate whether model activates collision model
        virtual bool active() const;

        //- Return the collision cross section times relative velocity product
        virtual scalar sigmaTcR
        (
            const dsmcParcel& pP,
            const dsmcParcel& pQ
        ) const;

        //- Apply collision
        virtual void collide
        (
            dsmcParcel& pP,
            dsmcParcel& pQ,
            const label cellI,
            scalar cR = -1
        );

        //- Scatter parcels P and Q
        virtual void scatter
        (
            dsmcParcel& pP,
            dsmcParcel& pQ,
            const label cellI,
            scalar cR = -1
        );

        //- Calculate post-collision velocities
        virtual void postCollisionVelocities
        (
            const label typeIdP,
            const label typeIdQ,
            vector& UP,
            vector& UQ,
            scalar cR = -1
        )
        {}

        //- Calculate post-reaction velocities for both products
        virtual void postReactionVelocities
        (
            const label typeIdP,
            const label typeIdQ,
            vector& UP,
            vector& UQ,
            scalar cR
        )
        {}

        //- Apply energy redistribution
        virtual void redistribute
        (
            dsmcParcel& p,
            scalar& translationalEnergy,
            const scalar omegaPQ,
            const bool postReaction = false
        );

        virtual const dictionary& coeffDict() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// #ifdef NoRepository
// #   include "NoBinaryCollision.C"
// #endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

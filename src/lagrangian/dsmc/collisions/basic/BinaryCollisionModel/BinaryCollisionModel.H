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
    Foam::BinaryCollisionModel

Description
    Templated DSMC particle collision class

SourceFiles
    BinaryCollisionModel.C
    BinaryCollisionModelNew.C

\*---------------------------------------------------------------------------*/

#ifndef BinaryCollisionModel_H
#define BinaryCollisionModel_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "dsmcParcel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class BinaryCollisionModel Declaration
\*---------------------------------------------------------------------------*/

class BinaryCollisionModel
{

protected:

    // Protected data

        //- The cloud dictionary
        const dictionary& dict_;

        //- Reference to the owner cloud class
        dsmcCloud& cloud_;


public:

    //- Runtime type information
    TypeName("BinaryCollisionModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        BinaryCollisionModel,
        dictionary,
        (
            const dictionary& dict,
            dsmcCloud& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        BinaryCollisionModel(dsmcCloud& owner);

        //- Construct from components
        BinaryCollisionModel
        (
            const dictionary& dict,
            dsmcCloud& owner
        );


    //- Destructor
    virtual ~BinaryCollisionModel();


    //- Selector
    static autoPtr<BinaryCollisionModel> New
    (
        const dictionary& dict,
        dsmcCloud& owner
    );


    // Access

        //- Return the dictionary
        const dictionary& dict() const;

        //- Return the coefficients dictionary
        virtual const dictionary& coeffDict() const = 0;


    // Member Functions

        //- Flag to indicate whether model activates collision model
        virtual bool active() const = 0;

        //- Return the collision cross section times relative velocity product
        virtual scalar sigmaTcR
        (
            const dsmcParcel& pP,
            const dsmcParcel& pQ
        ) const = 0;

        //- Apply collision
        virtual void collide
        (
            dsmcParcel& pP,
            dsmcParcel& pQ,
            const label cellI,
            scalar cR = -1
        ) = 0;

        //- Scatter parcels P and Q
        virtual void scatter
        (
            dsmcParcel& pP,
            dsmcParcel& pQ,
            const label cellI,
            scalar cR = -1
        ) = 0;

        //- Calculate post-collision velocities
        virtual void postCollisionVelocities
        (
            const label typeIdP,
            const label typeIdQ,
            vector& UP,
            vector& UQ,
            scalar cR = -1
        ) = 0;

        //- Calculate post-reaction velocities for both products
        virtual void postReactionVelocities
        (
            const label typeIdP,
            const label typeIdQ,
            vector& UP,
            vector& UQ,
            scalar cR
        ) = 0;

        //- Apply energy redistribution
        virtual void redistribute
        (
            dsmcParcel& p,
            scalar& translationalEnergy,
            const scalar omegaPQ,
            const bool postReaction = false
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// #include "BinaryCollisionModelI.H"

#endif

// ************************************************************************* //

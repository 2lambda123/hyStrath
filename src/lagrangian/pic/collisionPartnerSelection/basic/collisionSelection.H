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
    An abstract class for selecting collisions in a PD simulation

Description

SourceFiles
    collisionSelectionI.H
    collisionSelection.C
    collisionSelectionIO.C

\*---------------------------------------------------------------------------*/

#ifndef collisionSelection_H
#define collisionSelection_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "Time.H"
#include "fvMesh.H"
#include "pdParcel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class collisionSelection Declaration
\*---------------------------------------------------------------------------*/

class collisionSelection
{
    protected:

        const fvMesh& mesh_;

        pdCloud& cloud_;

        Random& rndGen_;


    private:


    // Private Member Functions


public:

    //- Runtime type information
    TypeName("collisionSelection");

    // Declare runtime constructor selection table
        declareRunTimeSelectionTable
        (
            autoPtr,
            collisionSelection,
            dictionary,
            (
                const polyMesh& mesh,
                pdCloud& cloud,
                const dictionary& dict
            ),
            (mesh, cloud, dict)
        );

    // Static data members

    // Constructors

        collisionSelection
        (
            const polyMesh& mesh,
            pdCloud& cloud,
            const dictionary& dict
        );

    // Selectors

        static autoPtr<collisionSelection> New
        (
            const polyMesh& mesh,
            pdCloud& cloud,
            const dictionary& dict
        );

    // Destructor

        virtual ~collisionSelection();

    // Member Functions

            virtual void initialConfiguration() = 0;

            virtual void collide() = 0;

        // Access

        // Check

        // Edit

        // Write

            //- output data

    // Member Operators

    // Friend Functions

    // Friend Operators

    // IOstream Operators

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// #include "collisionSelectionI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

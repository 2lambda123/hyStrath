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

Description

    Gravity force - gaussian distribution (stress next to a wall)

SourceFiles
    gaussianForceI.H
    gaussianForce.C
    gaussianForceIO.C

\*---------------------------------------------------------------------------*/

#ifndef gaussianForce_H
#define gaussianForce_H

#include "gravityForce.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class gaussianForce Declaration
\*---------------------------------------------------------------------------*/

class gaussianForce
:
    public gravityForce
{

private:

    // Private data

        dictionary propsDict_;

//         vector force_;
        vector startPoint_;
        vector endPoint_;
        vector normalVector_;

        // shear stress
        scalar stress_;

        scalar density_;

        scalar constantA_;
        scalar constantB_;

        vector forceDirection_;


//         label nBins_;
//         scalar binWidth_;

//         scalarField r_;
//         scalarField forces_;


public:

    //- Runtime type information
    TypeName("gaussianForce");

    // Static data members

    // Constructors

        //- construct from components
        gaussianForce
        (
            Time& time,
            const dictionary& dict
        );

    // Selectors


    // Destructor

        ~gaussianForce();

    // Member Functions

        // Access

        // Check

        // Edit

            void updateForce();

            vector force(const vector& position);

            vector force(const scalar& time);

        // Write


            void write
            (
                const fileName& fixedPathName,
                const fileName& timePath
            );

            void updateProperties
            (
                const dictionary& dict
            );

    // Member Operators

    // Friend Functions

    // Friend Operators

    // IOstream Operators

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

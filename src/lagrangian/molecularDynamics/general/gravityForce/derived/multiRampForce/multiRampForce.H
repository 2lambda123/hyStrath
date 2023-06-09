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
    Wall is moved in an oscillatory manner.


Description

SourceFiles
    multiRampForceI.H
    multiRampForce.C
    multiRampForceIO.C

\*---------------------------------------------------------------------------*/

#ifndef multiRampForce_H
#define multiRampForce_H


#include "gravityForce.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class multiRampForce Declaration
\*---------------------------------------------------------------------------*/

class multiRampForce
:
    public gravityForce
{

private:

    // Private data

        dictionary propsDict_;

        vector force_;

        vector direction_;

        scalar initialForce_;

        scalar finalForce_;

        scalar loadingTime_;

        scalar partialLoadingTime_;

        scalar breathingTime_;

        label noOfStops_;

        scalar forceGradient_;

        scalar currentTime_;
        scalar timeLoading_;
        scalar timeBreathing_;

        scalar deltaTMD_;







public:

    //- Runtime type information
    TypeName("multiRampForce");

    // Static data members

    // Constructors

        //- construct from components
        multiRampForce
        (
            Time& time,
            const dictionary& dict
        );

    // Selectors


    // Destructor

        ~multiRampForce();

    // Member Functions

        //- output data

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

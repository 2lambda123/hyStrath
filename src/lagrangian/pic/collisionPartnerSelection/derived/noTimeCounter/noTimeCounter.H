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

    no-time counter with reactions

SourceFiles
    noTimeCounterI.H
    noTimeCounter.C
    noTimeCounterIO.C

\*---------------------------------------------------------------------------*/

#ifndef noTimeCounter_H
#define noTimeCounter_H


#include "collisionPartnerSelection.H"
#include "pdCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class noTimeCounter Declaration
\*---------------------------------------------------------------------------*/

class noTimeCounter
:
    public collisionPartnerSelection
{

private:

    // Private data

//         dictionary propsDict_;

        scalar aveColl;
        scalar counter;


public:

    //- Runtime type information
    TypeName("noTimeCounter");

    // Static data members

    // Constructors

        //- construct from components
        noTimeCounter
        (
            const polyMesh& mesh,
            pdCloud& cloud,
            const dictionary& dict
        );

    // Selectors


    // Destructor

        ~noTimeCounter();

    // Member Functions

        // Access

        // Check

        // Edit

            void initialConfiguration();

            void collide();

        // Write





    // Member Operators

//         void operator=(const noTimeCounter&);

    // Friend Functions

    // Friend Operators

    // IOstream Operators

//         friend Ostream& operator<<(Ostream&, const noTimeCounter&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

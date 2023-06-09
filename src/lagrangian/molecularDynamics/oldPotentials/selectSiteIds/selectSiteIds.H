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

SourceFiles
    selectSiteIdsI.H
    selectSiteIds.C
    selectSiteIdsIO.C

\*---------------------------------------------------------------------------*/

#ifndef selectSiteIds_H
#define selectSiteIds_H

#include "polyMesh.H"
#include "potential.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class selectSiteIds Declaration
\*---------------------------------------------------------------------------*/

class selectSiteIds
{
    // Private data

        List<label> molSiteIds_;



    // Private Member Functions

public:

    // Static data members

    // Constructors

        //- Construct null
        selectSiteIds();

        //- Construct from xs and ys
        selectSiteIds
        (
            const potential& pot,
            const dictionary& dict
        );


    // Selectors

    // Destructor

        ~selectSiteIds();

    // Member Functions

        // Access

            inline List<label> molSiteIds();
//             inline labelList& molIds();


        // Check

        // Edit

        // Write


    // Member Operators

//         void operator=(const selectSiteIds&);

    // Friend Functions

    // Friend Operators

    // IOstream Operators

//         friend Ostream& operator<<(Ostream&, const selectSiteIds&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "selectSiteIdsI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

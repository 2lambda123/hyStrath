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

    distributing points in a boundedBox
    - Randomly - straightforward
    - Uniformly - the problem of distributing a number of points
            uniformly in a rectangle or cuboid is something which
            has not yet been determined properly in mathematics.


    Note this: replaces the obsolete "randomPointsInSquareZone"


SourceFiles
    distributePointsI.H
    distributePoints.C
    distributePointsIO.C

\*---------------------------------------------------------------------------*/

#ifndef distributePoints_H
#define distributePoints_H

#include "IFstream.H"
#include "OFstream.H"
#include "polyMesh.H"
#include "cachedRandomMD.H"
#include "Time.H"
#include "boundedBox.H"
#include "distribution.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class distributePoints Declaration
\*---------------------------------------------------------------------------*/

class distributePoints
:
    public boundedBox
{
    // Private data

        cachedRandomMD& rndGen_;

    // Private Member Functions

public:

    // Static data members

    // Constructors


     distributePoints
     (
         cachedRandomMD& rndGen
     );

     distributePoints
     (
         const boundedBox& bb,
         cachedRandomMD& rndGen
     );

//         //- Construct null
//         distributePoints
// 		(
// 			const label rndSeed,
// 			const label rndCacheSize
// 		);
//
//         //- Construct
//         distributePoints
//         (
//             const boundedBox& bb,
// 			const label rndSeed,
// 			const label rndCacheSize
//         );

    // Selectors

    // Destructor

        ~distributePoints();

    // Member Functions

        void setBoundedBox(const boundedBox& bb);
        vector randomPoint();
		List<vector> uniform(const label& nPoints);
        void testUniform();
        void outputProperties(const List<vector>& r);

        // Access

        // Check

        // Edit

        // Write

    // Member Operators

    // Friend Functions

    // Friend Operators

    // IOstream Operators
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

#endif

// ************************************************************************* //

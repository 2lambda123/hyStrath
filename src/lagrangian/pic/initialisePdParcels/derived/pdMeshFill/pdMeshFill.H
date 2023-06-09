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
    pdMeshFill

Description


SourceFiles
    pdMeshFillI.H
    pdMeshFill.C
    pdMeshFillIO.C


\*---------------------------------------------------------------------------*/

#ifndef pdMeshFill_H
#define pdMeshFill_H

#include "pdAllConfigurations.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "pdCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class pdMeshFill Declaration
\*---------------------------------------------------------------------------*/

class pdMeshFill
:
	public pdConfiguration
{

private:

    // Private data

        //- subDictionary containing the properties
//         dictionary propsDict_;


    // Private Member Functions




public:

    //- Runtime type information
    TypeName("pdMeshFill");

    // Constructors

        //- Construct from components
        pdMeshFill
		(
            pdCloud& cloud,
            const dictionary& dict
		);

    // Destructor

 		~pdMeshFill();


    // Member Functions

        void setInitialConfiguration();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    delFromMesh

Description
    Model which deletes molecules from the mesh. Method should be used to delete
    molecules using their id number. If oneSpecie = no, then all molecules from
    the mesh are deleted.


SourceFiles
    delFromMeshI.H
    delFromMesh.C
    delFromMeshIO.C

\*---------------------------------------------------------------------------*/

#ifndef delFromMesh_H
#define delFromMesh_H

#include "molsToDeleteModel.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "dsmcCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class delFromMesh Declaration
\*---------------------------------------------------------------------------*/

class delFromMesh
:
	public molsToDeleteModel
{

private:

    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;

        bool oneSpecie_;

        labelList typeIds_;



    // Private Member Functions

        void findMolsToDel();

public:

    //- Runtime type information
    TypeName("delFromMesh");

    // Constructors

        //- Construct from components
        delFromMesh
		(
            dsmcCloud& molCloud,
            const dictionary& dict
		);

    // Destructor

 		~delFromMesh();


    // Member Functions


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

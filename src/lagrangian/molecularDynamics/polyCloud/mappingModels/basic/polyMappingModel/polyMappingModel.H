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
    polyMappingModel

Description


SourceFiles
    polyMappingModelI.H
    polyMappingModel.C
    polyMappingModelIO.C

\*---------------------------------------------------------------------------*/

#ifndef polyMappingModel_H
#define polyMappingModel_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "vector.H"
#include "volFields.H"
#include "polyMolecule.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class polyMappingModel Declaration
\*---------------------------------------------------------------------------*/

class polyMappingModel
{

protected:

	// Protected data

        const fvMesh& mesh_;

        polyMoleculeCloud& molCloud_;

    // Private Member Functions

public:

    //- Runtime type information
    TypeName("polyMappingModel");

    // Declare runtime constructor selection table
        declareRunTimeSelectionTable
        (
            autoPtr,
            polyMappingModel,
            dictionary,
            (
                polyMoleculeCloud& molCloud,
                const dictionary& dict
            ),
            (molCloud, dict)
        );

    // Constructors

        //- Construct from components
        polyMappingModel
        (
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Selectors

        static autoPtr<polyMappingModel> New
        (
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        virtual ~polyMappingModel();

    // Member Functions

        // Access
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

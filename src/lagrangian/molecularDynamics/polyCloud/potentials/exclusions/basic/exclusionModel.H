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

    FALSE = interact
    TRUE = do not interact

    OR

    FALSE = do not exclude interactions
    TRUE = exclude interactions

SourceFiles
    exclusionModelI.H
    exclusionModel.C
    exclusionModelIO.C

\*---------------------------------------------------------------------------*/

#ifndef exclusionModel_H
#define exclusionModel_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "Time.H"
#include "polyMesh.H"
#include "polyMolecule.H"

// #include "mathematicalConstants.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class exclusionModel Declaration
\*---------------------------------------------------------------------------*/

class exclusionModel
{

protected:


        const polyMesh& mesh_;

        polyMoleculeCloud& molCloud_;

//         bool excludeMolecules_;
//         bool excludeSites_;

private:

    // Private Member Functions


public:

    //- Runtime type information
    TypeName("exclusionModel");

    // Declare runtime constructor selection table
        declareRunTimeSelectionTable
        (
            autoPtr,
            exclusionModel,
            dictionary,
            (
                const polyMesh& mesh,
                polyMoleculeCloud& molCloud,
                const dictionary& dict
            ),
            (mesh, molCloud, dict)
        );

    // Static data members

    // Constructors

        //- Construct from components
        exclusionModel
        (
            const polyMesh& mesh,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );


    // Selectors

        static autoPtr<exclusionModel> New
        (
            const polyMesh& mesh,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        virtual ~exclusionModel();

    // Member Functions

        virtual void initialiseExclusions
        (
        ) = 0;

        virtual bool excludeMolecules
        (
            polyMolecule* molI,
            polyMolecule* molJ
        ) = 0;

        virtual bool excludeSites
        (
            polyMolecule* molI,
            polyMolecule* molJ,
            const label& siteI,
            const label& siteJ
        ) = 0;


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

#include "exclusionModelI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

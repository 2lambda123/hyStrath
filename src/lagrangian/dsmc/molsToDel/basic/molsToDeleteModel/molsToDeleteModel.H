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
    molsToDeleteModel

Description


SourceFiles
    molsToDeleteModel.C

\*---------------------------------------------------------------------------*/

#ifndef molsToDeleteModel_H
#define molsToDeleteModel_H

#include "IOdictionary.H"
// #include "Time.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "vector.H"
#include "volFields.H"
#include "Random.H"
#include "dsmcParcel.H"



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class molsToDeleteModel Declaration
\*---------------------------------------------------------------------------*/

class molsToDeleteModel
{

protected:

    // Protected data

        const fvMesh& mesh_;

        dsmcCloud& cloud_;


    // Protected Member Functions

        void deleteMolFromMoleculeCloud(dsmcParcel& mol);


public:

    //- Runtime type information
    TypeName("molsToDeleteModel");

    // Declare runtime constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            molsToDeleteModel,
            dictionary,
            (
                dsmcCloud& cloud,
                const dictionary& dict
            ),
            (cloud, dict)
        );

    // Constructors

        //- Construct from components
        molsToDeleteModel
        (
            dsmcCloud& cloud,
            const dictionary& dict
        );


    // Selectors

        static autoPtr<molsToDeleteModel> New
        (
            dsmcCloud& cloud,
            const dictionary& dict
        );


    // Destructor

        virtual ~molsToDeleteModel();


    // Member Functions

      virtual void update();


    // Access

//             const labelList& molsToDel() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

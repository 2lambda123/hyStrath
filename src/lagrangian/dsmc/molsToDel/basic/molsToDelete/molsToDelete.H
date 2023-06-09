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
    molsToDelete

Description



\*---------------------------------------------------------------------------*/

#ifndef molsToDelete_H
#define molsToDelete_H


#include "List.H"
#include "IOdictionary.H"

#include "autoPtr.H"
#include "polyMesh.H"

#include "molsToDeleteModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class molsToDelete Declaration
\*---------------------------------------------------------------------------*/

class molsToDelete
{

    // Private data

        //- The entire dictionary (containing multiple subDictionaries)
        IOdictionary molsToDeleteDict_;

        //- state molsToDelete
		    PtrList<entry> modelList_;
        List< autoPtr<molsToDeleteModel> > delModels_;

public:

    // Constructors

        //- Constructor
        molsToDelete
        (
            const polyMesh& mesh,
            dsmcCloud& cloud,
            const word& deleteType = word::null
        );


    // Destructor

        virtual ~molsToDelete();


    // Member Functions

      virtual void update();



    // Access

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

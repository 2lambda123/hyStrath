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
    Foam::NoElectron

Description
    No collison BinaryCollision Model

\*---------------------------------------------------------------------------*/

#ifndef NoElectronModel_H
#define NoElectronModel_H

#include "ElectronModel.H"
#include "pdCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                      Class NoElectron Declaration
\*---------------------------------------------------------------------------*/

class NoElectronModel
:
    public ElectronModel
{

        //- The coefficients dictionary
        const dictionary coeffDict_;

public:

    //- Runtime type information
    TypeName("NoElectronModel");


    // Constructors

        //- Construct from dictionary
        NoElectronModel
        (
            const dictionary& dict,
            pdCloud& cloud
        );


    //- Destructor
        ~NoElectronModel();


    // Member Functions

        //- Flag to indicate whether model activates collision model
        bool active() const;

        void calculateRhoe();

        //Calculate model's contribution to potential
        void calculatePotential();

        const dictionary& coeffDict() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// #ifdef NoRepository
// #   include "NoElectronModel.C"
// #endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

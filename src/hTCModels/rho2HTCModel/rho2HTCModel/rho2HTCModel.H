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
    Foam::rho2HTCModel

Description
    HTC models for rho-based thermodynamics

SourceFiles
    rho2HTCModelI.H
    rho2HTCModel.C
    rho2HTCModelNew.C

\*---------------------------------------------------------------------------*/

#ifndef rho2HTCModel_H
#define rho2HTCModel_H

#include "hTC2Model.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "rho2ReactionThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace hTC2Models
{

/*---------------------------------------------------------------------------*\
                     class rho2HTCModel Declaration
\*---------------------------------------------------------------------------*/

class rho2HTCModel
:
    public hTC2Model
{

    // Private Member Functions

        //- Construct as copy (not implemented)
        rho2HTCModel(const rho2HTCModel&);

        //- Disallow default bitwise assignment
        void operator=(const rho2HTCModel&);


public:

    //- Runtime type information
    TypeName("rho2HTCModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        rho2HTCModel,
        dictionary,
        (
            const word& modelType,
            const fvMesh& mesh
        ),
        (modelType, mesh)
    );


    // Constructors


        //- Construct from components and thermo
        rho2HTCModel(const word& modelType, const fvMesh& mesh);



    //- Selector
    static autoPtr<rho2HTCModel> New(const fvMesh& mesh);


    //- Destructor
    virtual ~rho2HTCModel();


    // Member Functions

        // Access functions

            //- Access hTC dict
             inline const dictionary& coeff() const;


        //- Return access to the thermo package
        virtual rho2ReactionThermo& thermo() = 0;

        //- Return pointer access to the thermo package
        rho2ReactionThermo* thermoPtr()
        {
            return &(thermo());
        }

        //- Return const access to the thermo package
        virtual const rho2ReactionThermo& thermo() const = 0;

        //- Return tmp of rho
        virtual tmp<volScalarField> rho() const = 0;


     // I-O

        //- Update properties from given dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace hTC2Models
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

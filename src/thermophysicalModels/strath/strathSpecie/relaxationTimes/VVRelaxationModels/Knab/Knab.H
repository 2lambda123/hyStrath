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
    Foam::VVRelaxationModels::Knab

Description
    Knab V-V Relaxation model.

SourceFiles
    Knab.C

\*---------------------------------------------------------------------------*/

#ifndef Knab_H
#define Knab_H

#include "VVRelaxationModel.H"
#include "dimensionedScalar.H"
#include "volFields.H"

#include "fundamentalConstants.H"
#include "physicoChemicalConstants.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace VVRelaxationModels
{

/*---------------------------------------------------------------------------*\
                           Class Knab Declaration
\*---------------------------------------------------------------------------*/

class Knab
:
    public VVRelaxationModel
{
    // Private data

        // Coefficient
        scalar A12_;

        // Molecular weight, species i [g/mol]
        scalar W1_;

        // Reduced mass
        scalar W12_;

        // Cross-sectional diameter
        scalar sigma12_;

        // Probability
        scalar P21_;


public:

    //- Runtime type information
    TypeName("Knab");


    // Constructors

        //- construct from components
        Knab
        (
            const word& name1,
            const word& name2,
            const label& lname1,
            const label& lname2,
            const dictionary& dict1,
            const dictionary& dict2,
            const volScalarField& p,
            const volScalarField& Tt,
            const PtrList<volScalarField>& Tv,
            const PtrList<volScalarField>& nD
        );


    // Destructor

        ~Knab()
        {}


    // Member Functions

        //- V-V relaxation time [s]
        virtual tmp<volScalarField> tauVV() const;

        //- V-V relaxation time for patch [s]
        virtual tmp<scalarField> tauVV
        (
            const label patchi,
            const scalarField& p,
            const scalarField& Tt,
            const PtrList<scalarField>& Tv,
            const PtrList<scalarField>& nD
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace VVRelaxationModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

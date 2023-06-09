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
    Foam::fixed2TREnergyFvPatchScalarField

Group
    grpThermoBoundaryConditions

Description
    This boundary condition provides a fixed condition for 
    trans-rotational energy

    \heading Patch usage

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            fixed2TREnergy;
        value           uniform 100;
    }
    \endverbatim

SeeAlso
    Foam::fixedValueFvPatchField

SourceFiles
    fixed2TREnergyFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef fixed2TREnergyFvPatchScalarField_H
#define fixed2TREnergyFvPatchScalarField_H

#include "fixedValueFvPatchFields.H"
#include "rho2ReactionThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
               Class fixed2TREnergyFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class fixed2TREnergyFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{

private:

    // Private data

        // Thermo package that contains species patch data
        const rho2ReactionThermo& thermo_;

public:

    //- Runtime type information
    TypeName("fixed2TREnergy");


    // Constructors

        //- Construct from patch and internal field
        fixed2TREnergyFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixed2TREnergyFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given fixed2TREnergyFvPatchScalarField
        // onto a new patch
        fixed2TREnergyFvPatchScalarField
        (
            const fixed2TREnergyFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fixed2TREnergyFvPatchScalarField
        (
            const fixed2TREnergyFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new fixed2TREnergyFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        fixed2TREnergyFvPatchScalarField
        (
            const fixed2TREnergyFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new fixed2TREnergyFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //

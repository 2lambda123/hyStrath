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
    Foam::externalWallHeatFluxTemperatureFvPatchScalarField

Group
    grpThermoBoundaryConditions grpWallBoundaryConditions

Description
    This boundary condition supplies a heat flux condition for temperature
    on an external wall. Optional thin thermal layer resistances can be
    specified through thicknessLayers and kappaLayers entries for the
    fixed heat transfer coefficient mode.

    The condition can operate in two modes:
      - fixed heat transfer coefficient: supply h and Ta
      - fixed heat flux: supply q

    where:
    \vartable
        h  | heat transfer coefficient [W/m^2/K]
        Ta | ambient temperature [K]
        q  | heat flux [W/m^2]
    \endvartable

    The thermal conductivity \c kappa can either be retrieved from various
    possible sources, as detailed in the class temperatureCoupledBase.

Usage
    \table
        Property     | Description                 | Required | Default value
        q            | heat flux [W/m^2]           | yes*     |
        Ta           | ambient temperature [K]     | yes*     |
        h            | heat transfer coefficient [W/m^2/K] | yes*|
        thicknessLayers | list of thicknesses per layer [m] | yes |
        kappaLayers  | list of thermal conductivities per layer [W/m/K] | yes |
        Qr           | name of the radiative field | no | no
        relaxation   | relaxation factor for radiative field | no | 1
        kappaMethod  | inherited from temperatureCoupledBase | inherited |
        kappa        | inherited from temperatureCoupledBase | inherited |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            externalWallHeatFluxTemperature;
        q               uniform 1000;
        Ta              uniform 300.0;
        h               uniform 10.0;
        thicknessLayers (0.1 0.2 0.3 0.4);
        kappaLayers     (1 2 3 4);
        value           uniform 300.0;
        Qr              none;
        relaxation      1;
        kappaMethod           multi2Thermo;
        kappa       none;
    }
    \endverbatim

    Note:
      - Only supply \c h and \c Ta, or \c q in the dictionary (see above)

See also
    Foam::temperatureCoupledBase

SourceFiles
    externalWallHeatFluxTemperatureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef solidWallHeatFluxTemperatureFvPatchScalarField_H
#define solidWallHeatFluxTemperatureFvPatchScalarField_H

#include "mixedFvPatchFields.H"
#include "temperatureCoupledBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
      Class externalWallHeatFluxTemperatureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class externalWallHeatFluxTemperatureFvPatchScalarField
:
    public mixedFvPatchScalarField,
    public temperatureCoupledBase
{
public:

    // Public data

        //- Operation mode enumeration
        enum operationMode
        {
            fixedHeatFlux,              //!< Fixed heat flux
            fixedHeatTransferCoeff,     //!< Fixed heat transfer coefficient
            unknown
        };

        static const NamedEnum<operationMode, 3> operationModeNames;


private:

    // Private data

        //- Operation mode
        operationMode mode_;

        //- Heat flux / [W/m2]
        scalarField q_;

        //- Heat transfer coefficient / [W/m2K]
        scalarField h_;

        //- Ambient temperature / [K]
        scalarField Ta_;

        //- Chache Qr for relaxation
        scalarField QrPrevious_;

        //- Relaxation for Qr
        scalar QrRelaxation_;

        //- Name of the radiative heat flux
        const word QrName_;

        //- Thickness of layers
        scalarList thicknessLayers_;

        //- Conductivity of layers
        scalarList kappaLayers_;


public:

    //- Runtime type information
    TypeName("externalWallHeatFluxTemperature");


    // Constructors

        //- Construct from patch and internal field
        externalWallHeatFluxTemperatureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        externalWallHeatFluxTemperatureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        // externalWallHeatFluxTemperatureFvPatchScalarField
        // onto a new patch
        externalWallHeatFluxTemperatureFvPatchScalarField
        (
            const externalWallHeatFluxTemperatureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        externalWallHeatFluxTemperatureFvPatchScalarField
        (
            const externalWallHeatFluxTemperatureFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new externalWallHeatFluxTemperatureFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        externalWallHeatFluxTemperatureFvPatchScalarField
        (
            const externalWallHeatFluxTemperatureFvPatchScalarField&,
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
                new externalWallHeatFluxTemperatureFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchScalarField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        // I-O

            //- Write
            void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    Foam::mixingRule

Description
    Abstract base class for mixing rules.

SourceFiles
    mixingRuleI.H
    mixingRule.C

\*---------------------------------------------------------------------------*/

#ifndef mixingRule_H
#define mixingRule_H

#include "fvc.H"

#include "rho2ChemistryModel.H"
#include "turbulentFluidThermoModel.H"
#include "multi2ComponentMixture.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class mixingRule Declaration
\*---------------------------------------------------------------------------*/

class mixingRule
:
    public IOdictionary
{

protected:

    // Protected data

        //- Mesh
        const fvMesh& mesh_;

        //- Reference to the thermodynamic package
        rho2ReactionThermo& thermo_;

        //- Reference to the turbulence model
        const compressibleTurbulenceModel& turbulence_;

        //- Viscosity [kg/m/s]
        PtrList<volScalarField> spmu_;
        
/*        PtrList<volScalarField> spCptr_;*/
/*        PtrList<volScalarField> spCvvel_;*/

        //- Thermal conductivity, trans-rotational energy mode [W/mK]
        PtrList<volScalarField> spkappatr_;

        //- Thermal conductivity, vibro-electronic energy mode [W/mK]
        PtrList<volScalarField> spkappave_;

        //- Thermal diffusivity, trans-rotational energy mode [kg/ms]
        PtrList<volScalarField> spalphatr_;

        //- Thermal diffusivity, vibro-eletronic energy mode [kg/ms]
        PtrList<volScalarField> spalphave_;

        //- Write the species/mixture viscosity?
        const bool writeMuSpecies_;
        const bool writeMuMixture_;

        //- Write the species/mixture thermal diffusivity?
        const bool writeKappaSpecies_;
        const bool writeKappaMixture_;


private:

    // Private member functions

        //- Disallow copy construct
        mixingRule(const mixingRule&);

        //- Disallow default bitwise assignment
        void operator=(const mixingRule&);


public:

    //- Runtime type information
    TypeName("mixingRule");


    // Declare run-time constructor selection table

#ifndef SWIG
    declareRunTimeSelectionTable
    (
        autoPtr,
        mixingRule,
        fvMesh,
        (
            rho2ReactionThermo& thermo,
            const compressibleTurbulenceModel& turbulence
        ),
        (thermo, turbulence)
    );
#endif


    // Constructors

        //- Construct from components
        mixingRule
        (
            rho2ReactionThermo& thermo,
            const compressibleTurbulenceModel& turbulence
        );


    // Selectors

        //- Return a reference to the selected mixing rule transport model
        static autoPtr<mixingRule> New
        (
            rho2ReactionThermo& thermo,
            const compressibleTurbulenceModel& turbulence
        );


    //- Destructor

        virtual ~mixingRule()
        {}


    // Member Functions

        // Access

            //- Return the const viscosity field for species i
            inline const volScalarField& mu(label i) const;

            //- Return the const trans-rotational thermal conductivity field
            //  for a given species
            inline const volScalarField& kappatr(label i) const;

            //- Return the const vibro-electronic thermal conductivity field
            //  for a given species
            inline const volScalarField& kappave(label i) const;

            //- Return the const trans-rotational thermal conductivity field
            //  for the mixture
            inline volScalarField& kappatr() const;

            //- Return the const vibro-electronic thermal conductivity field
            //  for the mixture
            inline volScalarField& kappave() const;

            //- Return the const trans-rotational thermal diffusivity field
            //  for a given species
            inline const volScalarField& alphatr(label i) const;

            //- Return the const vibro-electronic thermal diffusivity field
            //  for a given species
            inline const volScalarField& alphave(label i) const;

            //- Return the const trans-rotational thermal diffusivity field
            //  for the mixture
            inline volScalarField& alphatr() const;

            //- Return the const vibro-electronic thermal diffusivity field
            //  for the mixture
            inline volScalarField& alphave() const;

            //- Return the table of species
            inline const speciesTable& species() const;

            //- Return the table of species having a vib. eq. being solved
            inline const speciesTable& solvedVibEqSpecies() const;


        //- Molecular weight for a specie given by index
        virtual inline scalar W(const label i) const = 0;

        //- Viscosity for a specie given by index
        virtual inline scalar mu
        (
            const label i,
            const scalar p,
            const scalar T
        ) const = 0;
        
        //- Thermal conductivity [W/mK], for a specie given by index
        //  trans-rotational energy mode
        virtual inline scalar kappatr
        (
            const label i,
            const scalar p,
            const scalar T
        ) const = 0;

        //- Thermal conductivity [W/mK], for a specie given by index
        //  vibro-electronic energy mode
        virtual inline scalar kappave
        (
            const label i,
            const scalar p,
            const scalar T,
            const scalar Tve
        ) const = 0;

        //- Calculations and updates the different fields
        virtual void correct() = 0;

        //- Write the user-defined rarefied quantities
        virtual void write() = 0;

        //- Read transportProperties dictionary
        virtual bool read() = 0;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "mixingRuleI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

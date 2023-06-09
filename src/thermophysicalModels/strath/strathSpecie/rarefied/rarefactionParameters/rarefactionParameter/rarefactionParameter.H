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
    Foam::rarefactionParameter

Description
    Abstract base class for rarefied parameters definitions and models.

SourceFiles
    rarefactionParameterI.H
    rarefactionParameter.C

\*---------------------------------------------------------------------------*/

#ifndef rarefactionParameter_H
#define rarefactionParameter_H

#include "fvc.H"

#include "rho2ChemistryModel.H"
#include "turbulentFluidThermoModel.H"
#include "mfpModel.H"
#include "multi2ComponentMixture.H"

#include "mixingRule.H"
#include "wordList.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class rarefactionParameter Declaration
\*---------------------------------------------------------------------------*/

class rarefactionParameter
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

        //- Compute the rarefaction parameters
        bool computeRarefaction_;

        //- Compute the overall mean free path at the boundaries
        bool computeMfpBoundaries_;

        //- Temporary stuff for the definition of the overall mean free path
        const bool oldMfpDefinition_;

        //- Mean free path model, species i [m]
        autoPtr<mfpModel> mfpModel_;

        //- Store the name of the mean free path model
        const word mfpModelName_;

        //- Mean free path [m]
        PtrList<volScalarField> mfp_;

        //- Mean free path, gas mixture [m]
        volScalarField mfpMix_;

        //- Overall Knudsen number
        volScalarField Knov_;

        //- Characteristic length [m]
        scalar characteristicLength_;

        //- Gradient-length local Knudsen number
        volScalarField KnGLL_;

        //- Gradient-length local Knudsen number, components
        PtrList<volScalarField> KnsGLL_;

        //- Write the mean free path species/mixture?
        bool writeMfpSpecies_ ;
        bool writeMfpMixture_;

        //- Write the gradient length Knudsen number max/components?
        bool writeKnGLL_;
        bool writeKnGLLComponents_;

        //- Write the overall Knudsen number?
        bool writeKnOv_;


private:

    // Private member functions

        //- Disallow copy construct
        rarefactionParameter(const rarefactionParameter&);

        //- Disallow default bitwise assignment
        void operator=(const rarefactionParameter&);


public:

    //- Runtime type information
    TypeName("rarefactionParameter");


    // Declare run-time constructor selection table

#ifndef SWIG
    declareRunTimeSelectionTable
    (
        autoPtr,
        rarefactionParameter,
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
        rarefactionParameter
        (
            rho2ReactionThermo& thermo,
            const compressibleTurbulenceModel& turbulence
        );


    // Selectors

        //- Return a reference to the selected multispecies transport model

        static autoPtr<rarefactionParameter> New
        (
            rho2ReactionThermo& thermo,
            const compressibleTurbulenceModel& turbulence
        );


    //- Destructor

        virtual ~rarefactionParameter()
        {}


    // Member Functions

        // Access

            //- Return the const computeRarefaction boolean
            inline const bool& computeRarefaction() const;

            //- Return the const mean free path field for species i
            inline const volScalarField& mfp(label i) const;

            //- Return the const mixture mean free path
            inline const volScalarField& mfpMix() const;

            //- Return the const gradient-length local Knudsen number
            inline const volScalarField& KnGLL() const;

            //- Return the overall Knudsen number
            inline const volScalarField& Knov() const;

            //- Return the table of species
            inline const speciesTable& species() const;


        //- Shear viscosity for a specie given by index.
        inline virtual scalar mu(label i, scalar p, scalar Tt) const = 0;

        //- Calculations and updates the different fields
        virtual void correct(const volVectorField& U) = 0;

        //- Write the user-defined rarefied quantities
        virtual void write() = 0;

        //- Read transportProperties dictionary
        virtual bool read() = 0;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rarefactionParameterI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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

\*---------------------------------------------------------------------------*/

#include "noeVEnergyTransfer.H"
#include "fvm.H"

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class ThermoType>
void Foam::noeVEnergyTransfer<ThermoType>::updateCoefficients()
{
    forAll(species(), i)
    {
        taueV_[i] = dimensionedScalar("GREAT", dimTime, Foam::GREAT);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
Foam::noeVEnergyTransfer<ThermoType>::noeVEnergyTransfer
(
    rho2ReactionThermo& thermo,
    const compressibleTurbulenceModel& turbulence
)
:
    relaxationTimeModeleV(thermo, turbulence),

    speciesThermo_
    (
        dynamic_cast<const multi2ComponentMixture<ThermoType>&>
            (this->thermo_).speciesData()
    )
{
    taueV_.setSize(solvedVibEqSpecies().size());

    forAll(taueV_, speciei)
    {
        taueV_.set
        (
            speciei,
            new volScalarField
            (
                IOobject
                (
                    "taueV_" + solvedVibEqSpecies()[speciei],
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh_,
                dimensionedScalar("taueV", dimTime, 0.0)
            )
        );
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ThermoType>
void Foam::noeVEnergyTransfer<ThermoType>::correct()
{}


template<class ThermoType>
bool Foam::noeVEnergyTransfer<ThermoType>::read()
{
    if (regIOobject::read())
    {
        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

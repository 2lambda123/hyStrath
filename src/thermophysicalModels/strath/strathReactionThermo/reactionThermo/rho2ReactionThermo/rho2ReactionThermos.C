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

#include "make2ReactionThermo.H"

#include "rho2ReactionThermo.H"
#include "heRho2Thermo.H"

#include "advancedSpecie.H"
#include "perfect2Gas.H"
#include "janafThermo.H"

#include "decoupledEnergyModesThermo.H"
#include "multiThermo.H"

#include "constantTransport.H"
#include "SutherlandEuckenTransport.H"
#include "BlottnerEuckenTransport.H"
#include "powerLawEuckenTransport.H"
#include "CEATransport.H"
//#include "tabulatedTransport.H" // TODO WON'T WORK AT THE MOMENT

#include "multi2ComponentMixture.H"
#include "reacting2Mixture.H"

#include "thermoPhysics2Types.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Multi-component reaction thermo based on sensible internal energy

make2ReactionMixtureThermo
(
    rho2Thermo,
    rho2ReactionThermo,
    heRho2Thermo,
    reacting2Mixture,
    demConstGasEThermoPhysicsH2TGD
);

make2ReactionMixtureThermo
(
    rho2Thermo,
    rho2ReactionThermo,
    heRho2Thermo,
    reacting2Mixture,
    demGasEThermoPhysicsH2TGD
);

make2ReactionMixtureThermo
(
    rho2Thermo,
    rho2ReactionThermo,
    heRho2Thermo,
    reacting2Mixture,
    demBEGasEThermoPhysicsH2TGD
);

make2ReactionMixtureThermo
(
    rho2Thermo,
    rho2ReactionThermo,
    heRho2Thermo,
    reacting2Mixture,
    demPLEGasEThermoPhysicsH2TGD
);

make2ReactionMixtureThermo
(
    rho2Thermo,
    rho2ReactionThermo,
    heRho2Thermo,
    reacting2Mixture,
    demCEAGasEThermoPhysicsH2TGD
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

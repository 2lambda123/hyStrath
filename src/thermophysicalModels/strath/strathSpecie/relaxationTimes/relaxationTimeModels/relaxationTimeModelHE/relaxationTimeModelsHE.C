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

#include "makeRelaxationTimeModelHE.H"

#include "noHEEnergyTransfer.H"
#include "AppletonBray.H"

#include "thermoPhysics2Types.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

// Based on sensible internal energy

makeRelaxationTimeModelHE(noHEEnergyTransfer, demConstGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelHE(AppletonBray, demConstGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelHE(noHEEnergyTransfer, demGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelHE(AppletonBray, demGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelHE(noHEEnergyTransfer, demBEGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelHE(AppletonBray, demBEGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelHE(noHEEnergyTransfer, demPLEGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelHE(AppletonBray, demPLEGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelHE(noHEEnergyTransfer, demCEAGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelHE(AppletonBray, demCEAGasEThermoPhysicsH2TGD);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

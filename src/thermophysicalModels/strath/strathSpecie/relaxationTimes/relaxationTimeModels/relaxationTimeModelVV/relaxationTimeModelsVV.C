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

#include "makeRelaxationTimeModelVV.H"

#include "noVVEnergyTransfer.H"
#include "KnabVV.H"

#include "thermoPhysics2Types.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

// Based on sensible internal energy

makeRelaxationTimeModelVV(noVVEnergyTransfer, demConstGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelVV(KnabVV, demConstGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelVV(noVVEnergyTransfer, demGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelVV(KnabVV, demGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelVV(noVVEnergyTransfer, demBEGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelVV(KnabVV, demBEGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelVV(noVVEnergyTransfer, demPLEGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelVV(KnabVV, demPLEGasEThermoPhysicsH2TGD);

makeRelaxationTimeModelVV(noVVEnergyTransfer, demCEAGasEThermoPhysicsH2TGD);
makeRelaxationTimeModelVV(KnabVV, demCEAGasEThermoPhysicsH2TGD);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

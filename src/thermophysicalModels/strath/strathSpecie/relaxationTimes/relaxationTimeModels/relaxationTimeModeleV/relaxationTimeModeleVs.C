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

#include "makeRelaxationTimeModeleV.H"

#include "noeVEnergyTransfer.H"
#include "LeeLandauTellereV.H"

#include "thermoPhysics2Types.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

// Based on sensible internal energy

makeRelaxationTimeModeleV(noeVEnergyTransfer, demConstGasEThermoPhysicsH2TGD);
makeRelaxationTimeModeleV(LeeLandauTellereV, demConstGasEThermoPhysicsH2TGD);

makeRelaxationTimeModeleV(noeVEnergyTransfer, demGasEThermoPhysicsH2TGD);
makeRelaxationTimeModeleV(LeeLandauTellereV, demGasEThermoPhysicsH2TGD);

makeRelaxationTimeModeleV(noeVEnergyTransfer, demBEGasEThermoPhysicsH2TGD);
makeRelaxationTimeModeleV(LeeLandauTellereV, demBEGasEThermoPhysicsH2TGD);

makeRelaxationTimeModeleV(noeVEnergyTransfer, demPLEGasEThermoPhysicsH2TGD);
makeRelaxationTimeModeleV(LeeLandauTellereV, demPLEGasEThermoPhysicsH2TGD);

makeRelaxationTimeModeleV(noeVEnergyTransfer, demCEAGasEThermoPhysicsH2TGD);
makeRelaxationTimeModeleV(LeeLandauTellereV, demCEAGasEThermoPhysicsH2TGD);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

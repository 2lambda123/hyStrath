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

#include "rho2ThermoHTC.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::hTC2Models::rho2ThermoHTC::rho2ThermoHTC
(
    const word& modelType,
    const fvMesh& mesh
)
:
    rho2HTCModel(modelType, mesh),
    thermoPtr_(rho2ReactionThermo::New(mesh))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::hTC2Models::rho2ThermoHTC::~rho2ThermoHTC()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::rho2ReactionThermo&
Foam::hTC2Models::rho2ThermoHTC::thermo()
{
    return thermoPtr_();
}


const Foam::rho2ReactionThermo&
Foam::hTC2Models::rho2ThermoHTC::thermo() const
{
    return thermoPtr_();
}


Foam::tmp<Foam::volScalarField>
Foam::hTC2Models::rho2ThermoHTC::rho() const
{
    return thermoPtr_().rho();
}


// ************************************************************************* //

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

inline const Foam::volScalarField&
Foam::mixingRule::mu(label i) const
{
    return spmu_[i];
}

inline const Foam::volScalarField&
Foam::mixingRule::kappatr(label i) const
{
    return spkappatr_[i];
}


inline const Foam::volScalarField&
Foam::mixingRule::kappave(label i) const
{
    return spkappave_[i];
}


inline Foam::volScalarField&
Foam::mixingRule::kappatr() const
{
    return thermo_.kappatr();
}


inline Foam::volScalarField&
Foam::mixingRule::kappave() const
{
    return thermo_.kappave();
}


inline const Foam::volScalarField&
Foam::mixingRule::alphave(label i) const
{
    return spalphave_[i];
}


inline Foam::volScalarField&
Foam::mixingRule::alphatr() const
{
    return thermo_.alphatr();
}


inline Foam::volScalarField&
Foam::mixingRule::alphave() const
{
    return thermo_.alphave();
}


inline const Foam::speciesTable&
Foam::mixingRule::species() const
{
    return thermo_.composition().species();
}


inline const Foam::speciesTable&
Foam::mixingRule::solvedVibEqSpecies() const
{
    return thermo_.composition().solvedVibEqSpecies();
}

// ************************************************************************* //

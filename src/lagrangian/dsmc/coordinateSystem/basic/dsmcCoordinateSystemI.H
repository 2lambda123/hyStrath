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

Description

\*---------------------------------------------------------------------------*/

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline scalar dsmcCoordinateSystem::deltaTValueOrg() const
{
    return timeStepModel_().deltaTValueOrg();
}


inline const dsmcTimeStepModel& dsmcCoordinateSystem::dtModel() const
{
    return timeStepModel_;
}


inline dsmcTimeStepModel& dsmcCoordinateSystem::dtModel()
{
    return timeStepModel_();
}


inline scalar dsmcCoordinateSystem::deltaTValue(const label celli) const
{
    return timeStepModel_().deltaTValue(celli);
}


inline scalar dsmcCoordinateSystem::nParticlesOrg() const
{
    return nParticlesOrg_;
}


inline const volScalarField& dsmcCoordinateSystem::nParticles() const
{
    return timeStepModel_().nParticles();
}


inline scalar dsmcCoordinateSystem::nParticles(const label celli) const
{
    return timeStepModel_().nParticles(celli);
}


inline scalar dsmcCoordinateSystem::nParticles
(
    const label patchi,
    const label facei
) const
{
    return timeStepModel_().nParticles(patchi, facei);
}


inline scalar dsmcCoordinateSystem::RWF(const label celli) const
{
    return 1.0;
}


inline scalar dsmcCoordinateSystem::pRWF
(
    const label patchi,
    const label facei
) const
{
    return 1.0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

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

#include "restrainedHarmonicSpring.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace tetherPotentials
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(restrainedHarmonicSpring, 0);

addToRunTimeSelectionTable
(
    tetherPotential,
    restrainedHarmonicSpring,
    dictionary
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

restrainedHarmonicSpring::restrainedHarmonicSpring
(
    const word& name,
    const reducedUnits& rU,
    const dictionary& tetherPotentialProperties
)
:
    tetherPotential(name, rU, tetherPotentialProperties),
    restrainedHarmonicSpringCoeffs_
    (
        tetherPotentialProperties.subDict(typeName + "Coeffs")
    ),
    springConstant_
    (
        readScalar(restrainedHarmonicSpringCoeffs_.lookup("springConstant"))
    ),
    rR_
    (
        readScalar(restrainedHarmonicSpringCoeffs_.lookup("rR"))
    )
{
    if(rU.runReducedUnits())
    {
        springConstant_ /= (rU.refForce()/rU.refLength());
        rR_ /= rU.refLength();
    }
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

scalar restrainedHarmonicSpring::energy(const vector r) const
{
    scalar magR = mag(r);

    if (magR < rR_)
    {
        return 0.5 * springConstant_ * magSqr(r);
    }
    else
    {
        return 0.5 * springConstant_ * rR_ * rR_
          + springConstant_ * rR_ * (magR - rR_);
    }
}


vector restrainedHarmonicSpring::force(const vector r) const
{
    scalar magR = mag(r);

    if (magR < rR_)
    {
        return -springConstant_ * r;
    }
    else
    {
        return -springConstant_ * rR_ * r / magR;
    }
}


bool restrainedHarmonicSpring::read
(
    const dictionary& tetherPotentialProperties,
    const reducedUnits& rU
)
{
    tetherPotential::read(tetherPotentialProperties, rU);

    restrainedHarmonicSpringCoeffs_ =
        tetherPotentialProperties.subDict(typeName + "Coeffs");

    restrainedHarmonicSpringCoeffs_.lookup("springConstant") >> springConstant_;
    restrainedHarmonicSpringCoeffs_.lookup("rR") >> rR_;

    if(rU.runReducedUnits())
    {
        springConstant_ /= (rU.refForce()/rU.refLength());
        rR_ /= rU.refLength();
    }

    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tetherPotentials
} // End namespace Foam

// ************************************************************************* //

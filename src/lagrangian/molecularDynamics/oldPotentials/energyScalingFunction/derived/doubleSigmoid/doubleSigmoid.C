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

#include "doubleSigmoid.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace energyScalingFunctions
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(doubleSigmoid, 0);

addToRunTimeSelectionTable
(
    energyScalingFunction,
    doubleSigmoid,
    dictionary
);

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

scalar doubleSigmoid::sigmoidScale
    (
        const scalar r,
        const scalar shift,
        const scalar scale
    ) const
{
    return 1.0 / (1.0 + exp( scale * (r - shift)));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

doubleSigmoid::doubleSigmoid
(
    const word& name,
    const dictionary& energyScalingFunctionProperties,
    const pairPotential& pairPot,
    const reducedUnits& rU
)
:
    energyScalingFunction(name, energyScalingFunctionProperties, pairPot, rU),
    doubleSigmoidCoeffs_
    (
        energyScalingFunctionProperties.subDict(typeName + "Coeffs")
    ),
    shift1_(readScalar(doubleSigmoidCoeffs_.lookup("shift1"))),
    scale1_(readScalar(doubleSigmoidCoeffs_.lookup("scale1"))),
    shift2_(readScalar(doubleSigmoidCoeffs_.lookup("shift2"))),
    scale2_(readScalar(doubleSigmoidCoeffs_.lookup("scale2")))
{
    FatalErrorIn("doubleSigmoid::doubleSigmoid()")
        << "You will need to check and modify the code for the doubleSigmoid model (i.e. you need to make sure that the coefficents are changed to reduced units if you are using reduced units)"
        << nl << "in: " << "potentialDict"
        << exit(FatalError);

    if(rU.runReducedUnits())
    {
        shift1_ /= rU.refLength();
        shift2_ /= rU.refLength();
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void doubleSigmoid::scaleEnergy(scalar& e, const scalar r) const
{
    e *= sigmoidScale(r, shift1_, scale1_) * sigmoidScale(r, shift2_, scale2_);
}


bool doubleSigmoid::read
(
    const dictionary& energyScalingFunctionProperties,
    const reducedUnits& rU
)
{
    energyScalingFunction::read(energyScalingFunctionProperties, rU);

    doubleSigmoidCoeffs_ =
        energyScalingFunctionProperties.subDict(typeName + "Coeffs");

    doubleSigmoidCoeffs_.lookup("shift1") >> shift1_;
    doubleSigmoidCoeffs_.lookup("scale1") >> scale1_;
    doubleSigmoidCoeffs_.lookup("shift2") >> shift2_;
    doubleSigmoidCoeffs_.lookup("scale2") >> scale2_;


    if(rU.runReducedUnits())
    {
        shift1_ /= rU.refLength();
        shift2_ /= rU.refLength();
    }

    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace energyScalingFunctions
} // End namespace Foam

// ************************************************************************* //

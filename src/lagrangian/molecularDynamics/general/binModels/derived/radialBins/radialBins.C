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
    radialBins

Description

\*----------------------------------------------------------------------------*/

#include "radialBins.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(radialBins, 0);

addToRunTimeSelectionTable(binModel, radialBins, dictionary);



// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

label radialBins::findBin(const scalar& r)
{
    label n = -1;

    label n2 = label(r/minBinWidth_) - 1;

    if(n2 < 0)
    {
        n2 = 0;
    }

    if(n2 < n_.size())
    {
        n = n_[n2];

        scalar rLimit1 = magRadii_[n] - 0.5*binWidths_[n];
        scalar rLimit2 = magRadii_[n] + 0.5*binWidths_[n];

        if((r >= rLimit1) && (r < rLimit2))
        {}
        else
        {
            n++;
        }
    }

    return n;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from components
radialBins::radialBins
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    binModel(mesh, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    startPoint_(propsDict_.lookup("startPoint")),
    endPoint_(propsDict_.lookup("endPoint")),
    h_(mag(endPoint_ - startPoint_)),
    unitVector_((endPoint_ - startPoint_)/mag(endPoint_ - startPoint_)),
    normalVector_(vector::zero),
    radius_(readScalar(propsDict_.lookup("radius"))),
    nBins_(readLabel(propsDict_.lookup("nBins"))),
    binWidth_(radius_/scalar(nBins_)),

    magRadii_(),
    binWidths_(),
//     volumes_(),
    avVolume_(0.0),
    minBinWidth_(0.0),
    n_()

{
    //- set volumes --- here we impose a constant volume in all bins,
    // hence the binWidth is going change radially

   avVolume_ = radius_*radius_*constant::mathematical::pi*h_/nBins_;

//     Info << "average volume: " << avVolume_ << endl;

    DynamicList<scalar> radii(0);
    DynamicList<scalar> binWidths(0);

    binWidths.append(sqrt(avVolume_/(constant::mathematical::pi*h_)));

    radii.append(0.5*binWidths[0]);

    for(label i = 1; i < nBins_; i++)
    {
        scalar r2 = radii[i-1] + 0.5*binWidths[i-1];
        scalar r1 = sqrt((avVolume_/(constant::mathematical::pi*h_)) + (r2*r2));

        if(r2 <= radius_)
        {
            radii.append(0.5*(r1+r2));
            binWidths.append(r1-r2);
        }
        else
        {
            break;
        }
    }

    //binWidths.shrink();
    //radii.shrink();

    nBins_ = binWidths.size();

    magRadii_.setSize(nBins_, 0.0);
    binWidths_.setSize(nBins_, 0.0);
//     volumes_.setSize(nBins_, 0.0);

    forAll(magRadii_, n)
    {
        magRadii_[n] = radii[n];
        binWidths_[n] = binWidths[n];

//         if(n == 0)
//         {
//             volumes_[n] = mathematicalConstant::pi*binWidths_[n]*binWidths_[n]*h_;
//         }
//         else
//         {
//             volumes_[n] = 2.0*mathematicalConstant::pi*magRadii_[n]*binWidths_[n]*h_;
//         }
    }

//     Info << "volumes: " << volumes_ << " avVolume: " << avVolume_ << endl;

    minBinWidth_ = binWidths_[nBins_-1]/3.0;

    n_.setSize(label(radius_/minBinWidth_), 0);

    forAll(n_, n)
    {
        scalar r = 0.5*minBinWidth_ + scalar(n)*minBinWidth_;

        for(label i = 0; i < nBins_; i++)
        {
            scalar rLimit1 = magRadii_[i] - 0.5*binWidths_[i];
            scalar rLimit2 = magRadii_[i] + 0.5*binWidths_[i];

            if((r >= rLimit1) && (r < rLimit2))
            {
                n_[n] = i;
            }
        }
    }

    normalVector_ = vector(1,0,0);

    if (propsDict_.found("normalVector"))
    {
        normalVector_ = propsDict_.lookup("normalVector");

        normalVector_ /= mag(normalVector_);
    }
}



// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

radialBins::~radialBins()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //






label radialBins::isPointWithinBin
(
    const vector& rI,
    const label& cellI
)
{
    label binNumber = -1;

    vector rSI = rI - startPoint_;
    scalar rD = rSI & unitVector_;

    if((rD <= h_) && (rD >= 0.0))
    {
        scalar rN = mag((rD*unitVector_ + startPoint_) - rI);

        if(rN <= radius_)
        {
            label n = findBin(rN);

            if
            (
                n != -1
            )
            {
                binNumber = n;
            }
        }
    }

    return binNumber;
}


scalarField radialBins::binPositions()
{
//     scalarField positions(nBins_, 0.0);
//
//     forAll(positions, i)
//     {
//         positions[i] = 0.5*binWidth_ + scalar(i)*binWidth_;
//     }

    return magRadii_;
}


// this is a work around
vectorField radialBins::bins()
{
    vectorField positions(nBins_, vector::zero);

    forAll(positions, i)
    {
        positions[i] = magRadii_[i]*normalVector_;
    }

    return positions;
}

const label& radialBins::nBins() const
{
    return nBins_;
}

scalar radialBins::binVolume(const label& n)
{
    return avVolume_;
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

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

SourceFiles
    radial3DBinsI.H
    radial3DBins.C
    radial3DBinsIO.C

\*---------------------------------------------------------------------------*/

#ifndef radial3DBins_H
#define radial3DBins_H


#include "threeDimBinModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class radial3DBins Declaration
\*---------------------------------------------------------------------------*/

class radial3DBins
:
    public threeDimBinModel
{

private:

    // Private data

        dictionary propsDict_;

        vector startPoint_;
        vector endPoint_;
        scalar h_;
        vector unitVector_;

        vector angleUnitVectorY_;
        vector angleUnitVectorX_;

        vector normalVector_;

        scalar radius_;

        label nBinsL_;
        label nBinsR_;
        label nBinsA_;

        scalar binWidthL_;
        scalar binWidthR_;
        scalar binWidthA_;

        scalarField magRadii_;
        scalarField binWidthsR_;

//         scalarField volumes_;

        scalar avVolume_;

        // for searching
        scalar minBinWidth_;
        labelField n_;


    // Private Member Functions

        label findBinR(const scalar& r);
        label findBinL(const scalar& r);
        label findBinA(const scalar& theta);

public:

    //- Runtime type information
    TypeName("radial3DBins");

    // Static data members

    // Constructors

        //- construct from components
        radial3DBins
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

    // Selectors


    // Destructor

        ~radial3DBins();

    // Member Functions

        // Access

        // Check

        // Edit

            // 0 length, 1 radius, 2 angle
            List<label> isPointWithinBin
            (
                const vector& rI,
                const label& cellI
            );

            vectorField binPositionsX();
            scalarField binPositionsY();
            scalarField binPositionsZ();

            vector position(const vector&, const scalar&, const scalar&);

//             Field <Field<vectorField > > bins();
//             vectorField bins();

            // 0 length, 1 radius, 2 angle
            List<label> nBins();

            scalar binVolume(const label& n);

            void write
            (
                const fileName& path,
                const word& name
            );

        // Write





    // Member Operators

    // Friend Functions

    // Friend Operators

    // IOstream Operators


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

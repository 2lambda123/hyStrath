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
    fenePotentialI.H
    fenePotential.C
    fenePotentialIO.C

\*---------------------------------------------------------------------------*/

#ifndef fenePotential_H
#define fenePotential_H

#include "polyMesh.H"
#include "potential.H"
#include "dictionary.H"
// #include "writeTimeData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class fenePotential Declaration
\*---------------------------------------------------------------------------*/

class fenePotential
{
    // Private data

        const polyMesh& mesh_;

        const reducedUnits& rU_;
//         const dictionary& potentialProperties_;

        scalar dr_;
        scalar rCut_;
        scalar rCutSqr_;
        scalar r0_;
        scalar r0in_;
        scalar k_;
        label nBins_;
        scalarField U_;
        scalarField f_;


    // Private Member Functions

        void setLookUpTables();

public:

    // Static data members

    // Constructors

        //- Construct null
        fenePotential
        (
            const polyMesh& mesh,
            const reducedUnits& redUnits
        );



    // Selectors

    // Destructor

        ~fenePotential();

    // Member Functions

        void setPotential();

        // Access

//             inline List<label> molIds();
//             inline labelList& molIds();


        inline scalar fFene(const scalar& r);
        inline scalar Ufene(const scalar& r);

        inline scalar force (const scalar& r);
        inline scalar potentialEnergy (const scalar& r);


//         inline scalar numericalDifferentiation
//         (
//             const scalar& ra,
//             const scalar& r,
//             const scalar& rb,
//             const scalar& Ua,
//             const scalar& U,
//             const scalar& Ub
//         );

        inline const scalar& k() const;
        inline const scalar& rCut() const;
        inline const scalar& rCutSqr() const;
        inline const scalar& r0() const;

        // Check

        // Edit

        // Write
            void outputProperties();



    // Member Operators

//         void operator=(const fenePotential&);

    // Friend Functions

    // Friend Operators

    // IOstream Operators

//         friend Ostream& operator<<(Ostream&, const fenePotential&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fenePotentialI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

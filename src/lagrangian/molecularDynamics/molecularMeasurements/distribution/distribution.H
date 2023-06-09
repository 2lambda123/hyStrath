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
    Foam::distribution

Description
    Accumulating histogram of values.  Specified bin resolution
    automatic generation of bins.

SourceFiles
    distributionI.H
    distribution.C

\*---------------------------------------------------------------------------*/

#ifndef distribution_H
#define distribution_H

#include "Map.H"
#include "Pair.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class distribution Declaration
\*---------------------------------------------------------------------------*/

class distribution
:
    public Map<label>
{
    // Private data

        scalar binWidth_;


public:

    //- Runtime type information

        TypeName("distribution");

    // Static functions

        //- write to file

            static void write
            (
                const fileName& file,
                const List<Pair<scalar> >& pairs
            );


    // Constructors

        //- Construct null
        distribution();

        //- Construct from binWidth
        distribution(const scalar binWidth);

        //- Construct as copy
        distribution(const distribution&);


    //- Destructor
    virtual ~distribution();


    // Member Functions

        label totalEntries() const;

        scalar approxTotalEntries() const;

        scalar mean() const;

        scalar median();

        //- Add a value to the appropriate bin of the distribution.
        void add(const scalar valueToAdd);

        void add(const label valueToAdd);

        void insertMissingKeys();

        List<Pair<scalar> > normalised();

        List<Pair<scalar> > normalisedMinusMean();

        List<Pair<scalar> > normalisedShifted(scalar shiftValue);

        List<Pair<scalar> > raw();


        // Access

            inline scalar binWidth() const;


    // Member Operators

        void operator=(const distribution&);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const distribution&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "distributionI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

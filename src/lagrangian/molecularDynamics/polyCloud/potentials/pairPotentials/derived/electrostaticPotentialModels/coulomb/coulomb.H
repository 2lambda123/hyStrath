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
    coulomb


Description
    coulomb potential implemented in table format

SourceFiles
    coulombI.H
    coulomb.C
    coulombIO.C

\*---------------------------------------------------------------------------*/

#ifndef coulomb_H
#define coulomb_H

#include "pairPotentialModel.H"
#include "polyMoleculeCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class coulomb Declaration
\*---------------------------------------------------------------------------*/

class coulomb
:
    public pairPotentialModel
{

private:

    // Private data

        //- subDictionary containing the properties
//         dictionary propsDict_;

        scalar oneOverFourPiEps0_;

        // exclusion lists may go here



    // Private Member Functions



public:

    //- Runtime type information
    TypeName("coulomb");

    // Constructors

        //- Construct from components
        coulomb
        (
            const polyMesh& mesh,
            polyMoleculeCloud& molCloud,
            const reducedUnits& redUnits,
            const word& name,
            const dictionary& dict
        );

    // Destructor

        ~coulomb();

    // Member Functions

        scalar unscaledEnergy(const scalar r) const;


        scalar force(const scalar r) const;

        scalar energy(const scalar r) const;

//         void interaction (const scalar r, scalar& force, scalar& energy);

        //- Read dictionary
//         bool read
//         (
//             const dictionary& pairPotentialProperties,
//         );


        void write(const fileName& pathName);

        const dictionary& dict() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

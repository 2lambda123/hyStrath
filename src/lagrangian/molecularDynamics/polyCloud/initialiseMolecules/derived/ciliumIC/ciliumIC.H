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
    ciliumIC

Description


SourceFiles
    ciliumICI.H
    ciliumIC.C
    ciliumICIO.C


\*---------------------------------------------------------------------------*/

#ifndef ciliumIC_H
#define ciliumIC_H

#include "polyConfiguration.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"
#include "distributePoints.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class ciliumIC Declaration
\*---------------------------------------------------------------------------*/

class ciliumIC
:
	public polyConfiguration
{

private:

    // Private data


        DynamicList<label> trackingNumbers_;
//         DynamicList<vector> positions_;

        DynamicList<label> trackingNumbersA_;
        DynamicList<label> trackingNumbersB_;

        label nC_;

        List<scalar> x_;
        List<scalar> y_;

    // Private Member Functions


        void insertMoleculeLocal
        (
            const point& position,
            const label cell,
            const label tetFace,
            const label tetPt,
            const label& id,
            const bool& tethered,
            const bool& frozen,
            const scalar& temperature,
            const vector& bulkVelocity
        );


public:

    //- Runtime type information
    TypeName("ciliumIC");

    // Constructors

        //- Construct from components
        ciliumIC
        (
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        ~ciliumIC();


    // Member Functions

        void setInitialConfiguration();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

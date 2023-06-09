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
    velocityVerlet

Description
    simple velocity controller in a zone

    parallelised

    recommendation: control and measure properties every time step, ensure
                    size of zone.

SourceFiles
    velocityVerletI.H
    velocityVerlet.C
    velocityVerletIO.C

\*---------------------------------------------------------------------------*/

#ifndef velocityVerlet_H
#define velocityVerlet_H

#include "polyIntegrator.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"
// #include "boundedBox.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class velocityVerlet Declaration
\*---------------------------------------------------------------------------*/

class velocityVerlet
:
    public polyIntegrator
{

private:

    // Private data

       void updateVelocity(const scalar& trackTime);

public:

    //- Runtime type information
    TypeName("velocityVerlet");

    // Constructors

        //- Construct from components
        velocityVerlet
        (
            Time& t,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        ~velocityVerlet();


    // Member Functions

        void init();

        void evolve();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    dsmcLaserHeatingFill

Description


SourceFiles
    dsmcLaserHeatingFillI.H
    dsmcLaserHeatingFill.C
    dsmcLaserHeatingFillIO.C


\*---------------------------------------------------------------------------*/

#ifndef dsmcLaserHeatingFill_H
#define dsmcLaserHeatingFill_H

#include "dsmcAllConfigurations.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "dsmcCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class dsmcLaserHeatingFill Declaration
\*---------------------------------------------------------------------------*/

class dsmcLaserHeatingFill
:
	public dsmcConfiguration
{

private:

    // Private data

        //- subDictionary containing the properties
//         dictionary propsDict_;
        scalar r0_;
        scalar deltaT_;
        vector centrePoint_;


    // Private Member Functions




public:

    //- Runtime type information
    TypeName("dsmcLaserHeatingFill");

    // Constructors

        //- Construct from components
        dsmcLaserHeatingFill
        (
            dsmcCloud& cloud,
            const dictionary& dict
        );

    // Destructor

        ~dsmcLaserHeatingFill();


    // Member Functions

        void setInitialConfiguration();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

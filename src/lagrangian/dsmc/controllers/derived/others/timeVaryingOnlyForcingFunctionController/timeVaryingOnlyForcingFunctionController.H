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

#ifndef timeVaryingOnlyForcingFunctionController_H
#define timeVaryingOnlyForcingFunctionController_H

#include "dsmcStateController.H"
#include "vector.H"
#include "fvMesh.H"
#include "volFields.H"
#include "dsmcCloud.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
Class timeVaryingOnlyForcingFunctionController Declaration
\*---------------------------------------------------------------------------*/

    class timeVaryingOnlyForcingFunctionController
        :
            public dsmcStateController
    {

        private:
            //- subDictionary containing the properties
            dictionary propsDict_;

            vector accelerationDirection_;
            scalar amplitude_;
            scalar waveNumber_;
            scalar frequency_;
            scalar nTimeSteps_;
            scalar currentTime_;
            List<vector> writeAcceleration_;

            // Private Member Functions
            void setProperties();

        public:

            //- Runtime type information
            TypeName("timeVaryingOnlyForcingFunctionController");

            // Constructors
            //- Construct from components
            timeVaryingOnlyForcingFunctionController
            (
                Time& t,
                dsmcCloud& cloud,
                const dictionary& dict
            );

            // Destructor
            ~timeVaryingOnlyForcingFunctionController();

            // Member Functions
            void initialConfiguration();
            void controlParcelsBeforeMove();
            void controlParcelsBeforeCollisions();
            void controlParcelsAfterCollisions();

            //- calculate any required properties
            void calculateProperties();

            //- output data
            void output
            (
                const fileName& fixedPathName,
                const fileName& timePath
            );

            //- update properties from a modified dictionary
            void updateProperties(const dictionary&);
	};

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

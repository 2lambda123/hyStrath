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
    polyVelocity2DBins

Description


SourceFiles
    polyVelocity2DBinsI.H
    polyVelocity2DBins.C
    polyVelocity2DBinsIO.C

\*---------------------------------------------------------------------------*/

#ifndef polyVelocity2DBins_H
#define polyVelocity2DBins_H

#include "polyStateController.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"
#include "twoDimBinModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class polyVelocity2DBins Declaration
\*---------------------------------------------------------------------------*/

class polyVelocity2DBins
:
	public polyStateController
{

private:

    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;

        //- time constant
        scalar lambda_;

        // component control
        bool componentControl_;
        bool X_;
        bool Y_;
        bool Z_;

        autoPtr<twoDimBinModel> binModel_;

        List<label> molIds_;

        scalar deltaT_;

        vector velocity_;

    // Private Member Functions


public:

    //- Runtime type information
    TypeName("polyVelocity2DBins");

    // Constructors

        //- Construct from components
        polyVelocity2DBins
        (
            Time& t,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        ~polyVelocity2DBins();


    // Member Functions

        //- create an initial configuration
        void initialConfiguration();

        //- control molecules at various stages of time-step
        void controlBeforeVelocityI();
        void controlBeforeMove();
        void controlBeforeForces();
        void controlDuringForces
        (
            polyMolecule* molI,
            polyMolecule* molJ
        );
        void controlAfterForces();
        void controlAfterVelocityII();

        //- calculate any required properties (end of time-step)
        void calculateProperties();

        //- output data (optional)
        void output
        (
            const fileName& fixedPathName,
            const fileName& timePath
        );

        //-update properties from a modified dictionary (optional)
        void updateProperties(const dictionary&);


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    polyVelocityBins

Description


SourceFiles
    polyVelocityBinsI.H
    polyVelocityBins.C
    polyVelocityBinsIO.C

\*---------------------------------------------------------------------------*/

#ifndef polyVelocityBins_H
#define polyVelocityBins_H

#include "polyStateController.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"
#include "binModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class polyVelocityBins Declaration
\*---------------------------------------------------------------------------*/

class polyVelocityBins
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

        autoPtr<binModel> binModel_;

        List<label> molIds_;

        vector velocity_;


    // Private Member Functions


public:

    //- Runtime type information
    TypeName("polyVelocityBins");

    // Constructors

        //- Construct from components
        polyVelocityBins
        (
            Time& t,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        ~polyVelocityBins();


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

        //- update properties from a modified dictionary (optional)
        void updateProperties(const dictionary&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

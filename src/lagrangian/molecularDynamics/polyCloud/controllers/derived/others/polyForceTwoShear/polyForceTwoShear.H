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
    polyForceTwoShear

Description
    Model that applies a gravitational force on all molecules.

SourceFiles
    polyForceTwoShearI.H
    polyForceTwoShear.C
    polyForceTwoShearIO.C

\*---------------------------------------------------------------------------*/

#ifndef polyForceTwoShear_H
#define polyForceTwoShear_H

#include "polyStateController.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"

#include "boundedBox.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class polyForceTwoShear Declaration
\*---------------------------------------------------------------------------*/

class polyForceTwoShear
:
	public polyStateController
{

private:

    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;

        //- ids of mols
        List<label> molIds_;

        // liquid section

        boundedBox bb_liquid_A_;
        boundedBox bb_liquid_B_;

        vector forceMagLiquid_;

        boundedBox bb_gas_A_;
        boundedBox bb_gas_B_;

        vector forceMagGas_;

    // Private Member Functions

        void readProperties();

        void setBoundBox
        (
            const dictionary& propsDict,
            boundedBox& bb,
            const word& name
        );

public:

    //- Runtime type information
    TypeName("polyForceTwoShear");

    // Constructors

        //- Construct from components
        polyForceTwoShear
		(
            Time& t,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
		);

    // Destructor

 		~polyForceTwoShear();

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

        // Access
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

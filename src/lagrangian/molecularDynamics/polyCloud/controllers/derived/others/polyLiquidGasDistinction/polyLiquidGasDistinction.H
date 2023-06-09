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
    Written by Jun Zhang.

SourceFiles
   polyLiquidGasDistinction.H
    polyLiquidGasDistinction.C
    polyLiquidGasDistinctionIO.C

\*---------------------------------------------------------------------------*/

#ifndef polyLiquidGasDistinction_H
#define polyLiquidGasDistinction_H

#include "polyStateController.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
*                           Class polyLiquidGasDistinction Declaration
*----------------------------------------------------------------------------*/

class polyLiquidGasDistinction
:
    public polyStateController
{

private:

    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;

        // propertyField fields_;

        word regionName_;

        // 	word RregionName_;

        label regionId_;

        // 	label RregionId_;
        // word fieldName_;

        //
        scalar rMax_;

        label liquidLimit_;
        label molIdA_;
        label molIdB_;

        labelList lg_;


        // Private Member Functions

        void evaluatePair
        (
            polyMolecule* molI,
            polyMolecule* molJ
        );

        void evaluatePairS
        (
            polyMolecule* molReal,
            polyMolecule* molRef
        );

        void readProperties();

public:

    //- Runtime type information
    TypeName("polyLiquidGasDistinction");

    // Constructors

        //- Construct from components
        polyLiquidGasDistinction
        (
            Time& t,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        ~polyLiquidGasDistinction();


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


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

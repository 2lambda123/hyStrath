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
    temperatureBerendsenBins

Description


SourceFiles
    temperatureBerendsenBins.C


\*---------------------------------------------------------------------------*/

#ifndef temperatureBerendsenBins_H
#define temperatureBerendsenBins_H

#include "dsmcStateController.H"
#include "vector.H"
#include "fvMesh.H"
#include "volFields.H"
#include "dsmcCloud.H"
#include "binModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class temperatureBerendsenBins Declaration
\*---------------------------------------------------------------------------*/

class temperatureBerendsenBins
:
   public dsmcStateController
{

private:
    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;

        autoPtr<binModel> binModel_;

        //- time constant
        scalar tauT_;

        // target temperatures
        scalarField binTemperatures_;

        labelList typeIds_;

        //- measurements

        scalar averagingCounter_;

        scalarField mols_;
        scalarField dsmcMols_;
        scalarField mass_;
        scalarField mcc_;
        vectorField mom_;
        vectorField UCollected_;
        vectorField UMean_;

        scalarField translationalTemperature_;

        scalarField chi_;

        bool resetFieldsAtOutput_;
    // Private Member Functions

        void setRadii();

        void setProperties();

public:

    //- Runtime type information
    TypeName("temperatureBerendsenBins");

    // Constructors

        //- Construct from components
        temperatureBerendsenBins
        (
            Time& t,
            dsmcCloud& cloud,
            const dictionary& dict
        );

        // Destructor
        ~temperatureBerendsenBins();

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

        // Access
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

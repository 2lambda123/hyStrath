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
    dsmcRayleighBrillouinBinsMethod

Description


SourceFiles
    dsmcRayleighBrillouinBinsMethodI.H
    dsmcRayleighBrillouinBinsMethod.C
    dsmcRayleighBrillouinBinsMethodIO.C

\*---------------------------------------------------------------------------*/

#ifndef dsmcRayleighBrillouinBinsMethod_H
#define dsmcRayleighBrillouinBinsMethod_H

#include "dsmcField.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "dsmcCloud.H"
#include "binModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class dsmcRayleighBrillouinBinsMethod Declaration
\*---------------------------------------------------------------------------*/

class dsmcRayleighBrillouinBinsMethod
:
	public dsmcField
{

private:

    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;

//         propertyField fields_;

        autoPtr<binModel> binModel_;

        word regionName_;
        label regionId_;
        word fieldName_;

        labelList typeIds_;

//         label nBins_;

        scalar averagingCounter_;

        scalarField mols_;
        scalarField rhoN_;

        List<bool> outputField_;

        bool averagingAcrossManyRuns_;


    // Private Member Functions

        void readIn();

        void writeOut();

public:

    //- Runtime type information
    TypeName("dsmcRayleighBrillouinBinsMethod");

    // Constructors

        //- Construct from components
        dsmcRayleighBrillouinBinsMethod
        (
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud,
            const dictionary& dict
        );

    // Destructor

        ~dsmcRayleighBrillouinBinsMethod();


    // Member Functions

        //- create initial field
        void createField();

        //- calculate field
        void calculateField();

        //- return field
//         const volScalarField& densityField() const;
//         const propertyField& fields() const;
        //- write field
        void writeField();

        void updateProperties(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

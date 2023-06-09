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
    dsmcVibrationalQuantumLevelDistribution

Description


SourceFiles
    dsmcVibrationalQuantumLevelDistributionI.H
    dsmcVibrationalQuantumLevelDistribution.C
    dsmcVibrationalQuantumLevelDistributionIO.C

\*---------------------------------------------------------------------------*/

#ifndef dsmcVibrationalQuantumLevelDistribution_H
#define dsmcVibrationalQuantumLevelDistribution_H

#include "dsmcField.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "dsmcCloud.H"
#include "distribution.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class dsmcVibrationalQuantumLevelDistribution Declaration
\*---------------------------------------------------------------------------*/

class dsmcVibrationalQuantumLevelDistribution
:
	public dsmcField
{

private:

    // Private data
        dictionary propsDict_;

        word fieldName_;

        label typeId_;

        word regionName_;
        label regionId_;

        scalar binWidth_;
        distribution distr_;



    // Private Member Functions


public:

    //- Runtime type information
    TypeName("dsmcVibrationalQuantumLevelDistribution");

    // Constructors

        //- Construct from components
        dsmcVibrationalQuantumLevelDistribution
	(
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud,
            const dictionary& dict
	);

    // Destructor

	~dsmcVibrationalQuantumLevelDistribution();


    // Member Functions

        //- create initial field
        void createField();

        //- calculate field
        void calculateField();

        //- return field
//         const volScalarField& densityField() const;

        //- write field
        void writeField();

//         const propertyField& fields() const;

        void updateProperties(const dictionary& dict);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

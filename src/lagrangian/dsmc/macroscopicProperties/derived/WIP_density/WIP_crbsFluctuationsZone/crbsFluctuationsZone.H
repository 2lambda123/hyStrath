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
    crbsFluctuationsZone

Description
    Geometric field for number density

    Coarse-graining technique may be chosen (parallelised):
        A measurement within an arbitrary cell, consists also of
        sampling from neighbouring cells (within rCut).

    One specie or all species handled only.

SourceFiles
    crbsFluctuationsZoneI.H
    crbsFluctuationsZone.C
    crbsFluctuationsZoneIO.C

\*---------------------------------------------------------------------------*/

#ifndef crbsFluctuationsZone_H
#define crbsFluctuationsZone_H

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
                           Class crbsFluctuationsZone Declaration
\*---------------------------------------------------------------------------*/

class crbsFluctuationsZone
:
	public dsmcField
{

private:

    // Private data
        dictionary propsDict_;

//         propertyField fields_;
        autoPtr<binModel> binModel_;

        word fieldName_;

        labelList typeIds_;

        word regionName_;
        label regionId_;

        scalar resetCounter_;

//         scalarField cellVolumes_;
//         scalarField cellCentres_;

		scalar domainLength_;

        scalarField n_;

        scalarField rt_;
        label timeIndex_;
        label nInstantSteps_;

        scalar Rt_;

        scalarField RtField_;
	//label counter_;

    // Private Member Functions


public:

    //- Runtime type information
    TypeName("crbsFluctuationsZone");

    // Constructors

        //- Construct from components
        crbsFluctuationsZone
		(
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud,
            const dictionary& dict
		);

    // Destructor

 		~crbsFluctuationsZone();


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

        void setProperties();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

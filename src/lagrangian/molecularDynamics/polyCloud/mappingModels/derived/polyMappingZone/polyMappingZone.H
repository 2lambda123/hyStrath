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
    polyMappingZone

Description

SourceFiles
    polyMappingZoneI.H
    polyMappingZone.C
    polyMappingZoneIO.C

\*---------------------------------------------------------------------------*/

#ifndef polyMappingZone_H
#define polyMappingZone_H

#include "polyMappingModel.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class polyMappingZone Declaration
\*---------------------------------------------------------------------------*/

class polyMappingZone
:
	public polyMappingModel
{

private:

    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;


        //- name of control zone
        vector translation_;

        List<label> molIds_;



    // Private Member Functions

        void findMolsToMap();

public:

    //- Runtime type information
    TypeName("polyMappingZone");

    // Constructors

        //- Construct from components
        polyMappingZone
		(
            polyMoleculeCloud& molCloud,
            const dictionary& dict
		);

    // Destructor

 		~polyMappingZone();


    // Member Functions


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

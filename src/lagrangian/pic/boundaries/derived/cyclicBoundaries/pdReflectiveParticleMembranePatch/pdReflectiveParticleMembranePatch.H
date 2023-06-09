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
    pdReflectiveParticleMembranePatch

Description

    RPM technique Li et al.


SourceFiles
    pdReflectiveParticleMembranePatch.C


\*---------------------------------------------------------------------------*/

#ifndef pdReflectiveParticleMembranePatch_H
#define pdReflectiveParticleMembranePatch_H

#include "pdCyclicBoundary.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "pdCloud.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class pdReflectiveParticleMembranePatch Declaration
\*---------------------------------------------------------------------------*/

class pdReflectiveParticleMembranePatch
:
	public pdCyclicBoundary
{

private:

    // Private data
        dictionary propsDict_;

        scalar p_;

        label nReflections_;
        label nRejections_;



    // Private Member Functions

        void readProperties();


public:

    //- Runtime type information
    TypeName("pdReflectiveParticleMembranePatch");

    // Constructors

        //- Construct from components
        pdReflectiveParticleMembranePatch
		(
            Time& t,
            const polyMesh& mesh,
            pdCloud& cloud,
            const dictionary& dict
		);

    // Destructor

 		~pdReflectiveParticleMembranePatch();


    // Member Functions


        void calculateProperties();

        //- create an initial configuration
        void initialConfiguration();

        //- control the molecules
        void controlMol
        (
            pdParcel& mol,
            pdParcel::trackingData& td
        );

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

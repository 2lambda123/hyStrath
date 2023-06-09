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
    freezeMolecules

Description


SourceFiles
    freezeMoleculesI.H
    freezeMolecules.C
    freezeMoleculesIO.C


\*---------------------------------------------------------------------------*/

#ifndef freezeMolecules_H
#define freezeMolecules_H

#include "polyConfiguration.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"
#include "boundedBox.H"
#include "distributePoints.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class freezeMolecules Declaration
\*---------------------------------------------------------------------------*/

class freezeMolecules
:
	public polyConfiguration
{

private:

    // Private data

        //- ids of mols
        List<label> molIds_;


    // Private Member Functions

        void setBoundBox
        (
            const dictionary& propsDict,
            boundedBox& bb,
            const word& name
        );

public:

    //- Runtime type information
    TypeName("freezeMolecules");

    // Constructors

        //- Construct from components
        freezeMolecules
        (
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

    // Destructor

        ~freezeMolecules();


    // Member Functions

        void setInitialConfiguration();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

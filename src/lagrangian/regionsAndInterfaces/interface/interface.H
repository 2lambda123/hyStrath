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
    interface

Description
    An interface (set of faces) is built, given any two cell zones.

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef interface_H
#define interface_H

#include "argList.H"
#include "polyMesh.H"
#include "Time.H"
#include "cellInfo.H"
#include "faceInfo.H"
#include "cellInfoList.H"
#include "faceInfoList.H"
#include "processorPolyPatch.H"

// #include "cellToFaceZones.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class interface Declaration
\*---------------------------------------------------------------------------*/

class interface
{
    // Private data

        const polyMesh& mesh_;

        faceInfoList& faceData_;

        cellInfoList& cellData_;

        const labelList& cellZoneA_;

        const word& zoneNameA_;

        const labelList& cellZoneB_;

        const word& zoneNameB_;

        DynamicList<label> boundaryCells_;

        DynamicList<label> interface_;

        word interfaceName_;


    // Private Member Functions

        void setInterface();

        void setInternalInterface();

        inline void findInterfaceFace
        (
            const label& cellI,
            const label& cellN
        );

        void handleBoundaries();

        void setName();

        void syncInterfaceFaces();

public:

    // Constructors

        //- Construct from two cellZones and their names
        interface
        (
            const polyMesh&,
            faceInfoList&,
            cellInfoList&,
            const labelList&,
            const word&,
            const labelList&,
            const word&
        );

    // Destructor

        ~interface();

    // Member Functions

        // Access

            const labelList& interfaceFaces() const;

            const word& interfaceName() const;

        // Check

        // Edit

        // Write

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// # include "interfaceI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    layerRegions

Description

\*----------------------------------------------------------------------------*/

#include "layerRegions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //





// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

layerRegions::~layerRegions()
{}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- construct null
// layerRegions::layerRegions()
// :
//     List<subLayer> ()
// {}

//- construct from components
// layerRegions::layerRegions
// (
//     const List<subLayer>& subLayerList,
// )
// :
//     List<subLayer> (subLayerList),
// {}


//- Construct from components: polyMesh, list of pointInfo,
//   list of faces (interface), list of cells (zone) and
//   a list of sub layer thicknesses.

layerRegions::layerRegions
(
    const polyMesh& mesh,
    pointInfoList& pointData,
    faceInfoList& faceData,
    cellInfoList& cellData,
    const labelList& interface,
    const labelList& zone,
    const List<word>& layerNames,
    const List<scalar>& layerThicknesses
)
:
    List<region>(),
    remainingCells_(zone),
    regionsCells_(0)
{
    List<region> layers(layerThicknesses.size());

    labelList newInterface = interface;

    forAll(layers, l)
    {
        Info << "Building layer region: " << l << endl;

        layers[l].setRegion
        (
            mesh,
            remainingCells_,
            newInterface,
            sqr(layerThicknesses[l]),
            layerNames[l],
            pointData,
            faceData,
            cellData
        );

        newInterface = layers[l].interfaceFaces();

        const labelList& regionCells = layers[l].regionCells();

        setRegionsCells(regionCells);

        setRemainingCells(regionCells);
    }

    transfer(layers);
}


// cells belonging to all the regions
void layerRegions::setRegionsCells
(
    const labelList& cellsInRegion
)
{
    DynamicList<label> regionsCells(regionsCells_);

    forAll(cellsInRegion, c)
    {
        regionsCells.append(cellsInRegion[c]);
    }

    regionsCells_ = regionsCells.shrink();
}

// cells remaining within the zone
// i.e. cells that are in the zone but not part of the regions
void layerRegions::setRemainingCells
(
    const labelList& regionCells
)
{
    DynamicList<label> newZone(0);

    forAll(remainingCells_, c)
    {
        if(findIndex(regionCells, remainingCells_[c]) == -1)
        {
            newZone.append(remainingCells_[c]);
        }
    }

    remainingCells_ = newZone.shrink();
}







// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

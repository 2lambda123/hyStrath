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

Description

\*---------------------------------------------------------------------------*/

#include "polyMesh.H"

namespace Foam
{






// // Construct as copy
// inline pointInfoList::pointInfoList(const pointInfoList& wpt)
// :
//     originPoint_(wpt.originPoint()),
//     currentPoint_(wpt.currentPoint()),
//     currentPointLabel_(wpt.currentPointLabel()),
//     Rsqr_(wpt.Rsqr()),
// //     visitedPoint_(wpt.visitedPoint()),
//     zonePoint_(wpt.isZonePoint()),
//     acceptedPoint_(wpt.isAcceptedPoint()),
//     boundaryPoint_(wpt.isBoundaryPoint()),
//     interfacePoint_(wpt.isInterfacePoint())
// {}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// inline void pointInfoList::findGlobalPoint
// (
//     const polyMesh& mesh,
//     const polyPatch& patch,
//     const label& localPoint
// //     const label& localPointIndex
// )
// {
//     const labelListList& pointFacesList = patch.pointFaces();
//
//     const faceList& localFaces = patch.localFaces();
//
//     const label& faceI = pointFacesList[localPoint][0];
//
// //     Pout << "faceI: " << faceI << endl;
//
//     const label& pointFaceIndex = findIndex(localFaces[faceI], localPoint);
//
//     const label globalFace = patch.start(); //+ faceI
//
// //     Pout << "globalFace: " << globalFace << endl;
//
// //     const labelList& pointList = mesh.faces()[globalFace];
//
// //     const label& globalPoint = pointList[pointFaceIndex];
//
// //     Pout << "Local Point " << localPoint
// //          << " Global Point: " << globalPoint << endl;
//
//
// //     label startFaceI = 0;
// //
// //     for(label i = 0; i < patch.size(); i++)
// //     {
// //         label patchFaceI = i + startFaceI;
// //
// //         label meshFaceI = patch.start() + patchFaceI;
// //     }
//
// }


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //








// - Set


// - Access



// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //



} // End namespace Foam

// ************************************************************************* //

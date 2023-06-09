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

\*---------------------------------------------------------------------------*/

void createCellZone
(
    polyMesh& mesh,
    const labelList& cellList,
    const word& name
)
{
    if(cellList.size() > 0)
    {
        if (mesh.cellZones().findZoneID(name) == -1)
        {
            label scz = mesh.cellZones().size();
            mesh.cellZones().setSize(scz+1);
            mesh.cellZones().set
            (
                scz,
                new cellZone
                (
                    name,             //name
                    cellList,                 //addressing
                    scz,                        //index
                    mesh.cellZones()            //cellZoneMesh
                )
            );

            mesh.cellZones().writeOpt() = IOobject::AUTO_WRITE;
        }
    }
}

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
    polyMoleculeCloud

Description

\*----------------------------------------------------------------------------*/

#include "polyMoleculeCloud.H"
// #include "potential.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

void polyMoleculeCloud::updateNeighbouringRadii
(
    polyMolecule* molI
)
{
    molI->R() = GREAT;
    const label& cell = molI->cell();

    const labelListList& fullInteractionCellList = iL_.fil();

    //- real cells

    const labelList& dICL = fullInteractionCellList[cell];

    //- molecules within direct interaction cells (not incl. owner cell)
    forAll(dICL, dCell)
    {
        forAll(cellOccupancy_[dICL[dCell]], m)
        {
            polyMolecule* molJ = cellOccupancy_[dICL[dCell]][m];

            vector rIJ = molI->position() - molJ->position();

            scalar rIJMag = mag(rIJ);

            if(molI->R() > rIJMag)
            {
                molI->R() = rIJMag;
            }

            if(molJ->R() > rIJMag)
            {
                molJ->R() = rIJMag;
            }
        }
    }

    //- molecules within owner cell

    forAll(cellOccupancy_[cell], mols)
    {
        polyMolecule* molJ = cellOccupancy_[cell][mols];

        if(molJ != molI)
        {
            vector rIJ = molI->position() - molJ->position();

            scalar rIJMag = mag(rIJ);

            if(molI->R() > rIJMag)
            {
                molI->R() = rIJMag;
            }

            if(molJ->R() > rIJMag)
            {
                molJ->R() = rIJMag;
            }
        }
    }

    //- referred cells

    const labelList& rs = iL_.sourceCellToRefs()[cell];

    forAll(rs, r)
    {
        const label& rId = rs[r];

        referredCell& refCellI = iL_.refCells()[rId];

        // OLDER const polyMolecule& particle = *molI;
        //OLDER iL_.referredCloud().append(particle.clone().ptr());

// OLD        iL_.referredCloud().append(molI);


        const polyMolecule& molIN = *molI;
        iL_.referredCloud().append(molIN.clone().ptr());


        polyMolecule* molN = iL_.referredCloud().last();

        refCellI.transformPoint(molN->position());
        molN->transformProperties(refCellI.translation());

        forAll(refCellI.neighbouringCells(), c)
        {
            const label& realCellI = refCellI.neighbouringCells()[c];

            forAll(cellOccupancy_[realCellI], mols)
            {
                polyMolecule* molJ = cellOccupancy_[realCellI][mols];

                vector rIJ = molN->position() - molJ->position();

                scalar rIJMag = mag(rIJ);

                if(molN->R() > rIJMag)
                {
                    molN->R() = rIJMag;
                }

                if(molJ->R() > rIJMag)
                {
                    molJ->R() = rIJMag;
                }
            }
        }
    }
}

void polyMoleculeCloud::updateRadii()
{
    forAll(iL_.refCells(), r)
    {
        referredCell& refCellI = iL_.refCells()[r];

        forAll(refCellI.neighbouringCells(), c)
        {
            const label& realCellI = refCellI.neighbouringCells()[c];

            forAll(cellOccupancy_[realCellI], mols)
            {
                polyMolecule* molI = cellOccupancy_[realCellI][mols];

                forAll(iL_.refCellsParticles()[r], molsJ)
                {
                    polyMolecule* molJ = iL_.refCellsParticles()[r][molsJ];

                    vector rIJ = molI->position() - molJ->position();

                    scalar rIJMag = mag(rIJ);

                    if(molI->R() > rIJMag)
                    {
                        molI->R() = rIJMag;
                    }

                    if(molJ->R() > rIJMag)
                    {
                        molJ->R() = rIJMag;
                    }
                }
            }
        }
    }
}

} // End namespace Foam

// ************************************************************************* //

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
    noTimeCounterPorousMaterial

Description

\*----------------------------------------------------------------------------*/

#include "noTimeCounterPorousMaterial.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(noTimeCounterPorousMaterial, 0);

addToRunTimeSelectionTable(collisionPartnerSelection, noTimeCounterPorousMaterial, dictionary);



// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from components
noTimeCounterPorousMaterial::noTimeCounterPorousMaterial
(
    const polyMesh& mesh,
    dsmcCloud& cloud,
    const dictionary& dict
)
:
    collisionPartnerSelection(mesh, cloud, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    porosity_(readScalar(propsDict_.lookup("porosity"))),
    temperature_(readScalar(propsDict_.lookup("surfaceTemperature")))
{}



// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

noTimeCounterPorousMaterial::~noTimeCounterPorousMaterial()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void noTimeCounterPorousMaterial::initialConfiguration()
{

}

void noTimeCounterPorousMaterial::collide()
{
    if (!cloud_.binaryCollision().active())
    {
        return;
    }

    // Temporary storage for subCells
    List<DynamicList<label> > subCells(8);

    label collisionCandidates = 0;

    label collisions = 0;

    const List<DynamicList<dsmcParcel*> > cellOccupancy = cloud_.cellOccupancy();

    const polyMesh& mesh = cloud_.mesh();

    forAll(cellOccupancy, cellI)
    {
        const scalar deltaT = cloud_.deltaTValue(cellI);

        const DynamicList<dsmcParcel*>& cellParcels(cellOccupancy[cellI]);

        label nC(cellParcels.size());

        if (nC > 1)
        {

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Assign particles to one of 8 Cartesian subCells

            // Clear temporary lists
            forAll(subCells, i)
            {
                subCells[i].clear();
            }

            // Inverse addressing specifying which subCell a parcel is in
            List<label> whichSubCell(cellParcels.size());

            const point& cC = mesh.cellCentres()[cellI];

            forAll(cellParcels, i)
            {
                const dsmcParcel& p = *cellParcels[i];

                vector relPos = p.position() - cC;

                label subCell =
                    pos(relPos.x()) + 2*pos(relPos.y()) + 4*pos(relPos.z());

                subCells[subCell].append(i);

                whichSubCell[i] = subCell;
            }

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            scalar sigmaTcRMax = cloud_.sigmaTcRMax()[cellI];

            scalar selectedPairs =
                cloud_.collisionSelectionRemainder()[cellI]
              + 0.5*nC*(nC - 1)*cloud_.nParticles(cellI)*sigmaTcRMax*deltaT
               /mesh.cellVolumes()[cellI];

            label nCandidates(selectedPairs);

            cloud_.collisionSelectionRemainder()[cellI] = selectedPairs - nCandidates;

            collisionCandidates += nCandidates;

            label porousCollisions = 0;

            for (label c = 0; c < nCandidates; c++)
            {
                if(cloud_.rndGen().sample01<scalar>() > porosity_)
                {
                    //label candidateP = rndGen_.position<label>(0, nC - 1); OLD
                    label candidateP = cloud_.randomLabel(0, nC-1);

                    dsmcParcel& p = *cellParcels[candidateP];

                    scalar mass = cloud_.constProps(p.typeId()).mass();

//                     scalar rand1 = cloud_.rndGen().sample01<scalar>();
//                     scalar rand2 = cloud_.rndGen().sample01<scalar>();
//                     scalar rand3 = cloud_.rndGen().sample01<scalar>();
//
//                     if(cloud_.rndGen().sample01<scalar>() > 0.5)
//                     {
//                        rand1 *= -1.0;
//                     }
//                     if(cloud_.rndGen().sample01<scalar>() > 0.5)
//                     {
//                        rand2 *= -1.0;
//                     }
//                     if(cloud_.rndGen().sample01<scalar>() > 0.5)
//                     {
//                        rand3 *= -1.0;
//                     }


                    p.U() = sqrt(physicoChemical::k.value()*temperature_/mass)
                            *vector
                            (
                                rndGen_.GaussNormal<scalar>(),
                                rndGen_.GaussNormal<scalar>(),
                                rndGen_.GaussNormal<scalar>()
                            );

//                     Info << "Velocity after control = " << p->U() << endl;

                    porousCollisions++;
                }
            }

            nCandidates -= porousCollisions;

            for (label c = 0; c < nCandidates; c++)
            {
                // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // subCell candidate selection procedure

                // Select the first collision candidate
                //label candidateP = rndGen_.position<label>(0, nC - 1); OLD
                label candidateP = cloud_.randomLabel(0, nC-1);

                // Declare the second collision candidate
                label candidateQ = -1;

                const List<label>& subCellPs = subCells[whichSubCell[candidateP]];

                label nSC = subCellPs.size();

                if (nSC > 1)
                {
                    // If there are two or more particle in a subCell, choose
                    // another from the same cell.  If the same candidate is
                    // chosen, choose again.

                    do
                    {
                        //candidateQ = subCellPs[rndGen_.position<label>(0, nSC - 1)]; OLD
                        candidateQ = subCellPs[cloud_.randomLabel(0, nSC-1)];

                    } while (candidateP == candidateQ);
                }
                else
                {
                    // Select a possible second collision candidate from the
                    // whole cell.  If the same candidate is chosen, choose
                    // again.

                    do
                    {
                        //candidateQ = rndGen_.position<label>(0, nC - 1); OLD
                        candidateQ = cloud_.randomLabel(0, nC-1);

                    } while (candidateP == candidateQ);
                }

                // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // uniform candidate selection procedure

                // // Select the first collision candidate
                // label candidateP = cloud_.randomLabel(0, nC-1);

                // // Select a possible second collision candidate
                // label candidateQ = cloud_.randomLabel(0, nC-1);

                // // If the same candidate is chosen, choose again
                // while (candidateP == candidateQ)
                // {
                //     candidateQ = cloud_.randomLabel(0, nC-1);
                // }

                // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

                dsmcParcel& parcelP = *cellParcels[candidateP];
                dsmcParcel& parcelQ = *cellParcels[candidateQ];

                scalar sigmaTcR = cloud_.binaryCollision().sigmaTcR
                (
                    parcelP,
                    parcelQ
                );

                // Update the maximum value of sigmaTcR stored, but use the
                // initial value in the acceptance-rejection criteria because
                // the number of collision candidates selected was based on this


                if (sigmaTcR > cloud_.sigmaTcRMax()[cellI])
                {
                    cloud_.sigmaTcRMax()[cellI] = sigmaTcR;
                }

                if ((sigmaTcR/sigmaTcRMax) > rndGen_.sample01<scalar>())
                {
                    // chemical reactions

                    // find which reaction model parcel p and q should use
                    label rMId = cloud_.reactions().returnModelId(parcelP, parcelQ);

//                             Info << " parcelP id: " <<  parcelP.typeId()
//                                 << " parcelQ id: " << parcelQ.typeId()
//                                 << " reaction model: " << rMId
//                                 << endl;

                    if(rMId != -1)
                    {
                        // try to react molecules
                        if(cloud_.reactions().reactions()[rMId]->reactWithLists())
                        {
                            // so far for recombination only
//                                     reactions_.reactions()[rMId]->reaction
//                                     (
//                                         parcelP,
//                                         parcelQ,
//                                         candidateList,
//                                         candidateSubList,
//                                         candidateP,
//                                         whichSubCell
//                                     );
                        }
                        else
                        {
                            cloud_.reactions().reactions()[rMId]->reaction
                            (
                                parcelP,
                                parcelQ
                            );
                        }
                        // if reaction unsuccessful use conventional collision model
                        if(cloud_.reactions().reactions()[rMId]->relax())
                        {
                            cloud_.binaryCollision().collide
                            (
                                parcelP,
                                parcelQ,
                                cellI
                            );
                        }
                    }
                    else // if reaction model not found, use conventional collision model
                    {
                        cloud_.binaryCollision().collide
                        (
                            parcelP,
                            parcelQ,
                            cellI
                        );
                    }

                    collisions++;
                }
            }
        }
    }

    reduce(collisions, sumOp<label>());

    reduce(collisionCandidates, sumOp<label>());

    cloud_.sigmaTcRMax().correctBoundaryConditions();

    if (collisionCandidates)
    {
        Info<< "    Collisions                      = "
            << collisions << nl
            << "    Acceptance rate                 = "
            << scalar(collisions)/scalar(collisionCandidates) << nl
            << endl;
    }
    else
    {
        Info<< "    No collisions" << endl;
    }
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

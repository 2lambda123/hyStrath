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

#include "polyMovingWallRitos.H"
#include "addToRunTimeSelectionTable.H"
#include "IFstream.H"
#include "graph.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(polyMovingWallRitos, 0);
addToRunTimeSelectionTable(polyStateController, polyMovingWallRitos, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
polyMovingWallRitos::polyMovingWallRitos
(
    Time& t,
    polyMoleculeCloud& molCloud,
    const dictionary& dict
)
:
    polyStateController(t, /*mesh,*/ molCloud, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    wallMotionModel_(),
    molIds_()
{
	writeInTimeDir_ = false;
    writeInCase_ = false;

//     singleValueController() = true;

    wallMotionModel_ = autoPtr<wallMotion>
    (
       wallMotion::New(t, propsDict_)
    );

    molIds_.clear();

    selectIds ids
    (
        molCloud_.cP(),
        propsDict_
    );

    molIds_ = ids.molIds();

    readProperties();
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

polyMovingWallRitos::~polyMovingWallRitos()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void polyMovingWallRitos::initialConfiguration()
{}

void polyMovingWallRitos::controlBeforeVelocityI()
{
    wallMotionModel_->updateVelocity();
    velocity_ = wallMotionModel_->velocity();

    Info << "polyMovingWallRitos: control velocity = " << velocity_ << endl;

    forAll(controlZone(), c)
    {
        const label& cellI = controlZone()[c];

        const List<polyMolecule*>& molsInCell = molCloud_.cellOccupancy()[cellI];

        forAll(molsInCell, m)
        {
            polyMolecule* molI = molsInCell[m];

            if(findIndex(molIds_, molI->id()) != -1)
            {
                molI->a() = vector::zero;
                molI->v() = velocity_;
            }
        }
    }
}

void polyMovingWallRitos::controlBeforeMove()
{}

void polyMovingWallRitos::controlBeforeForces()
{}


void polyMovingWallRitos::controlDuringForces
(
    polyMolecule* molI,
    polyMolecule* molJ
)
{}

void polyMovingWallRitos::controlAfterForces()
{}

void polyMovingWallRitos::controlAfterVelocityII()
{}


void polyMovingWallRitos::calculateProperties()
{}

void polyMovingWallRitos::output
(
    const fileName& fixedPathName,
    const fileName& timePath
)
{}

void polyMovingWallRitos::updateProperties(const dictionary& newDict)
{
    //- the main controller properties should be updated first
    updateStateControllerProperties(newDict);

    propsDict_ = newDict.subDict(typeName + "Properties");

    readProperties();
}


void polyMovingWallRitos::readProperties()
{}

} // End namespace Foam

// ************************************************************************* //

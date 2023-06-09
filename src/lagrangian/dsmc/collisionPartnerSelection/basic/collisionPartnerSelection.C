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
    collisionPartnerSelection

Description

\*----------------------------------------------------------------------------*/

#include "collisionPartnerSelection.H"
#include "dsmcCloud.H"

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(collisionPartnerSelection, 0);

defineRunTimeSelectionTable(collisionPartnerSelection, dictionary);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

collisionPartnerSelection::collisionPartnerSelection
(
    const polyMesh& mesh,
    dsmcCloud& cloud,
    const dictionary& dict
)
:
    mesh_(refCast<const fvMesh>(mesh)),
    cloud_(cloud),
    rndGen_(cloud_.rndGen())
{}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr<collisionPartnerSelection> collisionPartnerSelection::New
(
    const polyMesh& mesh,
    dsmcCloud& cloud,
    const dictionary& dict
)
{
    word collisionPartnerSelectionName
    (
        dict.lookup("collisionPartnerSelectionModel")
    );

    Info<< "Selecting collisionPartnerSelectionModel "
         << collisionPartnerSelectionName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(collisionPartnerSelectionName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalError
            << "collisionPartnerSelection::New(const dictionary&) : " << endl
            << "    unknown collisionPartnerSelectionModel type "
            << collisionPartnerSelectionName
            << ", constructor not in hash table" << endl << endl
            << "    Valid collisionPartnerSelection types are :" << endl;
        Info<< dictionaryConstructorTablePtr_->toc() << abort(FatalError);
    }

    return autoPtr<collisionPartnerSelection>
    (
        cstrIter()(mesh, cloud, dict)
    );
}
// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

collisionPartnerSelection::~collisionPartnerSelection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

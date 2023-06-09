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
    neighbourExclusions

Description

\*----------------------------------------------------------------------------*/

#include "neighbourExclusions.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(neighbourExclusions, 0);

addToRunTimeSelectionTable(exclusionModel, neighbourExclusions, dictionary);



// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from components
neighbourExclusions::neighbourExclusions
(
    const polyMesh& mesh,
    polyMoleculeCloud& molCloud,
    const dictionary& dict
)
:
    exclusionModel(mesh, molCloud, dict),
    propsDict_(dict.subDict(typeName + "Properties"))
{
}


void neighbourExclusions::initialiseExclusions()
{
    // read in tracking numbers
    List<label> molPointsA = List<label>(propsDict_.lookup("trackingNumbersA"));
    List<label> molPointsB = List<label>(propsDict_.lookup("trackingNumbersB"));

    if(molPointsA.size() != molPointsB.size())
    {
        FatalErrorIn("neighbourExclusions::neighbourExclusions()")
            << "size of trackingNumbersA = " << molPointsA
            << " is not the same as trackingNumbersB = : " << molPointsA
            << nl << "in system/potentialDict"
            << exit(FatalError);
    }

    tNsA_.setSize(molPointsA.size());
    tNsB_.setSize(molPointsB.size());

    forAll(tNsA_, i)
    {
        tNsA_[i]=molPointsA[i];
        tNsB_[i]=molPointsB[i];
    }

    label N=molCloud_.moleculeTracking().getMaxTrackingNumber();

    Info << "N = " << N << endl;

    fullTNs_.setSize(N);

    forAll(tNsA_, i)
    {
        label tNA = tNsA_[i];
        label tNB = tNsB_[i];

        fullTNs_[tNA].append(tNB);
        fullTNs_[tNB].append(tNA);
    }
}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

neighbourExclusions::~neighbourExclusions()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


bool neighbourExclusions::excludeMolecules
(
    polyMolecule* molI,
    polyMolecule* molJ
)
{
    return true;
}

bool neighbourExclusions::excludeSites
(
    polyMolecule* molI,
    polyMolecule* molJ,
    const label& siteI,
    const label& siteJ
)
{
    if(molI->trackingNumber() < fullTNs_.size())
    {
        if(findIndex(fullTNs_[molI->trackingNumber()], molJ->trackingNumber()) == -1)
        {
            return false;
        }
        else
        {
            forAll(fullTNs_[molI->trackingNumber()], i)
            {
                if(fullTNs_[molI->trackingNumber()][i] == molJ->trackingNumber())
                {
                    return true;
                }
            }

            return false;
        }
    }
    else
    {
        return true;
    }
}



// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

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

#include "pdField.H"
#include "graph.H"


namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(pdField, 0);

defineRunTimeSelectionTable(pdField, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
pdField::pdField
(
    Time& t,
    const polyMesh& mesh,
    pdCloud& cloud,
    const dictionary& dict
)
:
    mesh_(refCast<const fvMesh>(mesh)),
    cloud_(cloud),
    timeDict_(dict.subDict("timeProperties")),
    time_(t, timeDict_),
    casePath_(),
    timePath_()
{
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr<pdField> pdField::New
(
    Time& t,
    const polyMesh& mesh,
    pdCloud& cloud,
    const dictionary& dict
)
{
    word pdFieldName
    (
        dict.lookup("fieldModel")
    );

    Info<< "Selecting field: "
         << pdFieldName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(pdFieldName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalError
            << "pdField::New(const dictionary&) : " << endl
            << "    unknown pdField type "
            << pdFieldName
            << ", constructor not in hash table" << endl << endl
            << "    Valid types are :" << endl;
        Info<< dictionaryConstructorTablePtr_->toc() << abort(FatalError);
    }

    return autoPtr<pdField>
	(
		cstrIter()(t, mesh, cloud, dict)
	);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

pdField::~pdField()
{}

void pdField::updateTime()
{
    time_++;
}


void pdField::updateBasicFieldProperties
(
    const dictionary& newDict
)
{
    timeDict_ = newDict.subDict("timeProperties");

    if (timeDict_.found("resetAtOutput"))
    {
        time_.resetFieldsAtOutput() = Switch(timeDict_.lookup("resetAtOutput"));
    }
}

const fileName& pdField::casePath() const
{
    return casePath_;
}

fileName& pdField::casePath()
{
    return casePath_;
}


const fileName& pdField::timePath() const
{
    return timePath_;
}

fileName& pdField::timePath()
{
    return timePath_;
}



} // End namespace Foam

// ************************************************************************* //

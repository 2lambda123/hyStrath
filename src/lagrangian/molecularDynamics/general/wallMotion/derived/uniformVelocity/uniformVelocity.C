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
    uniformVelocity

Description

\*----------------------------------------------------------------------------*/

#include "uniformVelocity.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(uniformVelocity, 0);

addToRunTimeSelectionTable(wallMotion, uniformVelocity, dictionary);



// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from components
uniformVelocity::uniformVelocity
(
    Time& time,
    const dictionary& dict
)
:
    wallMotion(time, dict),
    propsDict_(dict.subDict(typeName + "Properties")),
    velocity_(propsDict_.lookup("velocity"))
{}



// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

uniformVelocity::~uniformVelocity()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const vector& uniformVelocity::velocity() const
{
    return velocity_;
}

void uniformVelocity::updateVelocity()
{

}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

// void uniformVelocity::operator=(const uniformVelocity& rhs)
// {
//     // Check for assignment to self
//     if (this == &rhs)
//     {
//         FatalErrorIn("uniformVelocity::operator=(const uniformVelocity&)")
//             << "Attempted assignment to self"
//             << abort(FatalError);
//     }
//
//     Map<label>::operator=(rhs);
//
//     binWidth_ = rhs.binWidth();
// }


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

// Ostream& operator<<(Ostream& os, const uniformVelocity& d)
// {
//     os  << d.binWidth_
//         << static_cast<const Map<label>&>(d);
//
//     // Check state of Ostream
//     os.check
//     (
//         "Ostream& operator<<(Ostream&, "
//         "const uniformVelocity&)"
//     );
//
//     return os;
// }

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


inline label pdReactions::returnModelId(pdParcel& p, pdParcel& q)
{
//     label modelId = -1;
//
//     forAll(reactions_, r)
//     {
//         if(reactions_[r]->tryReactMolecules(p,q))
//         {
//             modelId = r;
//         }
//     }

    return pairAddressing_[p.typeId()][q.typeId()];
}

inline List< autoPtr<pdReaction> >& pdReactions::reactions()
{
	return reactions_;
}

inline const List< autoPtr<pdReaction> >& pdReactions::reactions() const
{
    return reactions_;
}

inline const List< List<label> >& pdReactions::pairModelAddressing() const
{
    return pairAddressing_;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

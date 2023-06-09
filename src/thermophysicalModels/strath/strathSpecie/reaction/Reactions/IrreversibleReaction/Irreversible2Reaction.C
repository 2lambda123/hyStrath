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

#include "Irreversible2Reaction.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
Foam::Irreversible2Reaction<ReactionType, ReactionThermo, ReactionRate>::
Irreversible2Reaction
(
    const ReactionType<ReactionThermo>& reaction,
    const ReactionRate& k
)
:
    ReactionType<ReactionThermo>(reaction),
    k_(k)
{}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
Foam::Irreversible2Reaction<ReactionType, ReactionThermo, ReactionRate>::
Irreversible2Reaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    Istream& is
)
:
    ReactionType<ReactionThermo>(species, thermoDatabase, is),
    k_(species, is)
{}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
Foam::Irreversible2Reaction<ReactionType, ReactionThermo, ReactionRate>::
Irreversible2Reaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    ReactionType<ReactionThermo>(species, thermoDatabase, dict),
    k_(species, dict)
{}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
Foam::Irreversible2Reaction<ReactionType, ReactionThermo, ReactionRate>::
Irreversible2Reaction
(
    const Irreversible2Reaction<ReactionType, ReactionThermo,ReactionRate>& irr,
    const speciesTable& species
)
:
    ReactionType<ReactionThermo>(irr, species),
    k_(irr.k_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
Foam::scalar Foam::Irreversible2Reaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::kf
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    return k_(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
void Foam::Irreversible2Reaction<ReactionType, ReactionThermo, ReactionRate>::
write
(
    Ostream& os
) const
{
    ReactionType<ReactionThermo>::write(os);
    k_.write(os);
}


// ************************************************************************* //

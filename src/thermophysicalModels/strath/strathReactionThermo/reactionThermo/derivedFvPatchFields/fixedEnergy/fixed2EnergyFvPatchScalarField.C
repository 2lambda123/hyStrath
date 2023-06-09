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

#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "multi2Thermo.H"
#include "addToRunTimeSelectionTable.H"
#include "fixed2EnergyFvPatchScalarField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fixed2EnergyFvPatchScalarField::
fixed2EnergyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    thermo_(rho2ReactionThermo::lookup2ReactionThermo(*this))
{} // Only this constructor is used


Foam::fixed2EnergyFvPatchScalarField::
fixed2EnergyFvPatchScalarField
(
    const fixed2EnergyFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    thermo_(rho2ReactionThermo::lookup2ReactionThermo(*this))
{}


Foam::fixed2EnergyFvPatchScalarField::
fixed2EnergyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF, dict),
    thermo_(rho2ReactionThermo::lookup2ReactionThermo(*this))
{}


Foam::fixed2EnergyFvPatchScalarField::
fixed2EnergyFvPatchScalarField
(
    const fixed2EnergyFvPatchScalarField& tppsf
)
:
    fixedValueFvPatchScalarField(tppsf),
    thermo_(rho2ReactionThermo::lookup2ReactionThermo(*this))
{}


Foam::fixed2EnergyFvPatchScalarField::
fixed2EnergyFvPatchScalarField
(
    const fixed2EnergyFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(tppsf, iF),
    thermo_(rho2ReactionThermo::lookup2ReactionThermo(*this))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fixed2EnergyFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    //Info<< "fixed2Energy is used for patch called "
    //    << patch().name() << endl;

    const multi2Thermo& multiThermo = multi2Thermo::lookup2Thermo(*this);

    const label patchi = patch().index();

    const fvPatchScalarField& pw = multiThermo.p().boundaryField()[patchi];
    const fvPatchScalarField& Tw = multiThermo.T().boundaryField()[patchi];

    tmp<scalarField> thet(new scalarField(pw.size()));
    tmp<scalarField> thevel(new scalarField(pw.size()));
    
    scalarField& het = thet.ref();
    scalarField& hevel = thevel.ref();

    het = 0.0;
    hevel = 0.0;
    
    forAll(thermo_.composition().species(), speciei)
    {
        const fvPatchScalarField& spYw =
            thermo_.composition().Y(speciei).boundaryField()[patchi];
        const fvPatchScalarField& spTvw =
            thermo_.composition().Tv(speciei).boundaryField()[patchi];

        het += spYw*thermo_.composition().het(speciei, pw, Tw, patchi);
        hevel += spYw*thermo_.composition().hevel(speciei, pw, spTvw, patchi);
    }

    operator==(het + thevel);

    fixedValueFvPatchScalarField::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        fixed2EnergyFvPatchScalarField
    );
}

// ************************************************************************* //

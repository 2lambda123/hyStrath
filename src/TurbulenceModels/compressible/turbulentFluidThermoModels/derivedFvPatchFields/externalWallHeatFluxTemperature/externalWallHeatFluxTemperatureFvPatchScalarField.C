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

#include "externalWallHeatFluxTemperatureFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    template<>
    const char*
    NamedEnum
    <
        externalWallHeatFluxTemperatureFvPatchScalarField::operationMode,
        3
    >::names[] =
    {
        "fixed_heat_flux",
        "fixed_heat_transfer_coefficient",
        "unknown"
    };

} // End namespace Foam

const Foam::NamedEnum
<
    Foam::externalWallHeatFluxTemperatureFvPatchScalarField::operationMode,
    3
> Foam::externalWallHeatFluxTemperatureFvPatchScalarField::operationModeNames;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    temperatureCoupledBase(patch(), "undefined", "undefined", "undefined-K"),
    mode_(unknown),
    q_(p.size(), 0.0),
    h_(p.size(), 0.0),
    Ta_(p.size(), 0.0),
    QrPrevious_(p.size(), 0.0),
    QrRelaxation_(1),
    QrName_("undefined-Qr"),
    thicknessLayers_(),
    kappaLayers_()
{
    refValue() = 0.0;
    refGrad() = 0.0;
    valueFraction() = 1.0;
}


Foam::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
    const externalWallHeatFluxTemperatureFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    temperatureCoupledBase(patch(), ptf),
    mode_(ptf.mode_),
    q_(ptf.q_, mapper),
    h_(ptf.h_, mapper),
    Ta_(ptf.Ta_, mapper),
    QrPrevious_(ptf.QrPrevious_, mapper),
    QrRelaxation_(ptf.QrRelaxation_),
    QrName_(ptf.QrName_),
    thicknessLayers_(ptf.thicknessLayers_),
    kappaLayers_(ptf.kappaLayers_)
{}


Foam::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    temperatureCoupledBase(patch(), dict),
    mode_(unknown),
    q_(p.size(), 0.0),
    h_(p.size(), 0.0),
    Ta_(p.size(), 0.0),
    QrPrevious_(p.size(), 0.0),
    QrRelaxation_(dict.lookupOrDefault<scalar>("relaxation", 1)),
    QrName_(dict.lookupOrDefault<word>("Qr", "none")),
    thicknessLayers_(),
    kappaLayers_()
{
    if (dict.found("q") && !dict.found("h") && !dict.found("Ta"))
    {
        mode_ = fixedHeatFlux;
        q_ = scalarField("q", dict, p.size());
    }
    else if (dict.found("h") && dict.found("Ta") && !dict.found("q"))
    {
        mode_ = fixedHeatTransferCoeff;
        h_ = scalarField("h", dict, p.size());
        Ta_ = scalarField("Ta", dict, p.size());
        if (dict.found("thicknessLayers"))

        if (dict.readIfPresent("thicknessLayers", thicknessLayers_))
        {
            dict.lookup("kappaLayers") >> kappaLayers_;

            if (thicknessLayers_.size() != kappaLayers_.size())
            {
                FatalIOErrorInFunction(dict)
                    << "\n number of layers for thicknessLayers and "
                    << "kappaLayers must be the same"
                    << "\n for patch " << p.name()
                    << " of field " << internalField().name()
                    << " in file " << internalField().objectPath()
                    << exit(FatalIOError);
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "\n patch type '" << p.type()
            << "' either q or h and Ta were not found '"
            << "\n for patch " << p.name()
            << " of field " << internalField().name()
            << " in file " << internalField().objectPath()
            << exit(FatalError);
    }

    fvPatchScalarField::operator=(scalarField("value", dict, p.size()));

    if (dict.found("QrPrevious"))
    {
        QrPrevious_ = scalarField("QrPrevious", dict, p.size());
    }

    if (dict.found("refValue"))
    {
        // Full restart
        refValue() = scalarField("refValue", dict, p.size());
        refGrad() = scalarField("refGradient", dict, p.size());
        valueFraction() = scalarField("valueFraction", dict, p.size());
    }
    else
    {
        // Start from user entered data. Assume fixedValue.
        refValue() = *this;
        refGrad() = 0.0;
        valueFraction() = 1.0;
    }
}


Foam::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
    const externalWallHeatFluxTemperatureFvPatchScalarField& tppsf
)
:
    mixedFvPatchScalarField(tppsf),
    temperatureCoupledBase(tppsf),
    mode_(tppsf.mode_),
    q_(tppsf.q_),
    h_(tppsf.h_),
    Ta_(tppsf.Ta_),
    QrPrevious_(tppsf.QrPrevious_),
    QrRelaxation_(tppsf.QrRelaxation_),
    QrName_(tppsf.QrName_),
    thicknessLayers_(tppsf.thicknessLayers_),
    kappaLayers_(tppsf.kappaLayers_)
{}


Foam::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
    const externalWallHeatFluxTemperatureFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(tppsf, iF),
    temperatureCoupledBase(patch(), tppsf),
    mode_(tppsf.mode_),
    q_(tppsf.q_),
    h_(tppsf.h_),
    Ta_(tppsf.Ta_),
    QrPrevious_(tppsf.QrPrevious_),
    QrRelaxation_(tppsf.QrRelaxation_),
    QrName_(tppsf.QrName_),
    thicknessLayers_(tppsf.thicknessLayers_),
    kappaLayers_(tppsf.kappaLayers_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::externalWallHeatFluxTemperatureFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mixedFvPatchScalarField::autoMap(m);
    q_.autoMap(m);
    h_.autoMap(m);
    Ta_.autoMap(m);
    QrPrevious_.autoMap(m);
}


void Foam::externalWallHeatFluxTemperatureFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);

    const externalWallHeatFluxTemperatureFvPatchScalarField& tiptf =
        refCast<const externalWallHeatFluxTemperatureFvPatchScalarField>(ptf);

    q_.rmap(tiptf.q_, addr);
    h_.rmap(tiptf.h_, addr);
    Ta_.rmap(tiptf.Ta_, addr);
    QrPrevious_.rmap(tiptf.QrPrevious_, addr);
}


void Foam::externalWallHeatFluxTemperatureFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const scalarField Tp(*this);
    scalarField hp(patch().size(), 0.0);

    scalarField Qr(Tp.size(), 0.0);
    if (QrName_ != "none")
    {
        Qr = patch().lookupPatchField<volScalarField, scalar>(QrName_);

        Qr = QrRelaxation_*Qr + (1.0 - QrRelaxation_)*QrPrevious_;
        QrPrevious_ = Qr;
    }

    switch (mode_)
    {
        case fixedHeatFlux:
        {
            refGrad() = (q_ + Qr)/kappa(Tp);
            refValue() = 0.0;
            valueFraction() = 0.0;

            break;
        }
        case fixedHeatTransferCoeff:
        {
            scalar totalSolidRes = 0.0;
            if (thicknessLayers_.size() > 0)
            {
                forAll(thicknessLayers_, iLayer)
                {
                    const scalar l = thicknessLayers_[iLayer];
                    if (kappaLayers_[iLayer] > 0.0)
                    {
                        totalSolidRes += l/kappaLayers_[iLayer];
                    }
                }
            }
            hp = 1.0/(1.0/h_ + totalSolidRes);

            Qr /= Tp;
            refGrad() = 0.0;
            refValue() = hp*Ta_/(hp - Qr);
            valueFraction() =
                (hp - Qr)/((hp - Qr) + kappa(Tp)*patch().deltaCoeffs());

            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Illegal heat flux mode " << operationModeNames[mode_]
                << exit(FatalError);
        }
    }

    mixedFvPatchScalarField::updateCoeffs();

    if (debug)
    {
        scalar Q = gSum(kappa(Tp)*patch().magSf()*snGrad());

        Info<< patch().boundaryMesh().mesh().name() << ':'
            << patch().name() << ':'
            << internalField().name() << " :"
            << " heat transfer rate:" << Q
            << " wall temperature "
            << " min:" << gMin(*this)
            << " max:" << gMax(*this)
            << " avg:" << gAverage(*this)
            << endl;
    }
}


void Foam::externalWallHeatFluxTemperatureFvPatchScalarField::write
(
    Ostream& os
) const
{
    mixedFvPatchScalarField::write(os);
    temperatureCoupledBase::write(os);

    QrPrevious_.writeEntry("QrPrevious", os);
    os.writeKeyword("Qr")<< QrName_ << token::END_STATEMENT << nl;
    os.writeKeyword("relaxation")<< QrRelaxation_
        << token::END_STATEMENT << nl;

    switch (mode_)
    {

        case fixedHeatFlux:
        {
            q_.writeEntry("q", os);
            break;
        }
        case fixedHeatTransferCoeff:
        {
            h_.writeEntry("h", os);
            Ta_.writeEntry("Ta", os);
            thicknessLayers_.writeEntry("thicknessLayers", os);
            kappaLayers_.writeEntry("kappaLayers", os);
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Illegal heat flux mode " << operationModeNames[mode_]
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        externalWallHeatFluxTemperatureFvPatchScalarField
    );
}

// ************************************************************************* //

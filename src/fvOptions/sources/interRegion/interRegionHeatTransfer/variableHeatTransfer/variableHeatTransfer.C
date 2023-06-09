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

#include "variableHeatTransfer.H"
#include "turbulentFluidThermoModel.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{
    defineTypeNameAndDebug(variableHeatTransfer, 0);
    addToRunTimeSelectionTable
    (
        option,
        variableHeatTransfer,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fv::variableHeatTransfer::variableHeatTransfer
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    interRegionHeatTransferModel(name, modelType, dict, mesh),
    UNbrName_(coeffs_.lookupOrDefault<word>("UNbr", "U")),
    a_(0),
    b_(0),
    c_(0),
    ds_(0),
    Pr_(0),
    AoV_()
{
    if (master_)
    {
        a_ = readScalar(coeffs_.lookup("a"));
        b_ = readScalar(coeffs_.lookup("b"));
        c_ = readScalar(coeffs_.lookup("c"));
        ds_ = readScalar(coeffs_.lookup("ds"));
        Pr_ = readScalar(coeffs_.lookup("Pr"));
        AoV_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    "AoV",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh_
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fv::variableHeatTransfer::~variableHeatTransfer()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fv::variableHeatTransfer::calculateHtc()
{
    const fvMesh& nbrMesh =
        mesh_.time().lookupObject<fvMesh>(nbrRegionName());

    const compressible::turbulenceModel& nbrTurb =
        nbrMesh.lookupObject<compressible::turbulenceModel>
        (
            turbulenceModel::propertiesName
        );

    const multi2Thermo& nbrThermo =
        nbrMesh.lookupObject<multi2Thermo>(basic2Thermo::dictName);

    const volVectorField& UNbr =
        nbrMesh.lookupObject<volVectorField>(UNbrName_);

    const volScalarField ReNbr(mag(UNbr)*ds_*nbrThermo.rho()/nbrTurb.mut());

    const volScalarField NuNbr(a_*pow(ReNbr, b_)*pow(Pr_, c_));

    const scalarField htcNbr(NuNbr*nbrTurb.kappaEff()/ds_);

    const scalarField htcNbrMapped(interpolate(htcNbr));

    htc_.primitiveFieldRef() = htcNbrMapped*AoV_;
}


bool Foam::fv::variableHeatTransfer::read(const dictionary& dict)
{
    if (interRegionHeatTransferModel::read(dict))
    {
        coeffs_.readIfPresent("UNbr", UNbrName_);

        coeffs_.readIfPresent("a", a_);
        coeffs_.readIfPresent("b", b_);
        coeffs_.readIfPresent("c", c_);
        coeffs_.readIfPresent("ds", ds_);
        coeffs_.readIfPresent("Pr", Pr_);

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //

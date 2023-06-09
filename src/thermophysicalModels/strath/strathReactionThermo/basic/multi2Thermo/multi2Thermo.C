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

#include "multi2Thermo.H"

#include "zeroGradientFvPatchFields.H"

#include "fixed2TREnergyFvPatchScalarField.H"
#include "gradient2TREnergyFvPatchScalarField.H"
#include "mixed2TREnergyFvPatchScalarField.H"

#include "fixed2VELMixEnergyFvPatchScalarField.H"
#include "gradient2VELMixEnergyFvPatchScalarField.H"
#include "mixed2VELMixEnergyFvPatchScalarField.H"

#include "fixedJumpFvPatchFields.H"
#include "fixedJumpAMIFvPatchFields.H"
#include "energyJumpFvPatchScalarField.H"
#include "energyJumpAMIFvPatchScalarField.H"

#include <string>

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace Foam
{
    defineTypeNameAndDebug(multi2Thermo, 0);
    defineRunTimeSelectionTable(multi2Thermo, fvMesh);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

Foam::wordList Foam::multi2Thermo::het2BoundaryBaseTypes()
{
    const volScalarField::Boundary& tbf = this->T_.boundaryField();

    wordList hbt(tbf.size(), word::null);

    forAll(tbf, patchi)
    {
        if (isA<fixedJumpFvPatchScalarField>(tbf[patchi]))
        {
            const fixedJumpFvPatchScalarField& pf =
                dynamic_cast<const fixedJumpFvPatchScalarField&>(tbf[patchi]);

            hbt[patchi] = pf.interfaceFieldType();
        }
        else if (isA<fixedJumpAMIFvPatchScalarField>(tbf[patchi]))
        {
            const fixedJumpAMIFvPatchScalarField& pf =
                dynamic_cast<const fixedJumpAMIFvPatchScalarField&>
                (
                    tbf[patchi]
                );

            hbt[patchi] = pf.interfaceFieldType();
        }
    }

    return hbt;
}


Foam::wordList Foam::multi2Thermo::het2BoundaryTypes()
{
    const volScalarField::Boundary& tbf = this->T_.boundaryField();

    wordList hbt = tbf.types();

    forAll(tbf, patchi)
    {
        if (isA<fixedValueFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = fixed2TREnergyFvPatchScalarField::typeName;
        }
        else if
        (
            isA<zeroGradientFvPatchScalarField>(tbf[patchi])
         || isA<fixedGradientFvPatchScalarField>(tbf[patchi])
        )
        {
            hbt[patchi] = gradient2TREnergyFvPatchScalarField::typeName;
        }
        else if (isA<mixedFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = mixed2TREnergyFvPatchScalarField::typeName;
        }
        else if (isA<fixedJumpFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = energyJumpFvPatchScalarField::typeName;
        }
        else if (isA<fixedJumpAMIFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = energyJumpAMIFvPatchScalarField::typeName;
        }
        else if (tbf[patchi].type() == "energyRegionCoupledFvPatchScalarField")
        {
            hbt[patchi] = "energyRegionCoupledFvPatchScalarField";
        }
    }

    return hbt;
}


Foam::wordList Foam::multi2Thermo::hevelMix2BoundaryBaseTypes()
{
    const volScalarField::Boundary& tbf = this->Tv_.boundaryField();

    wordList hbt(tbf.size(), word::null);

    forAll(tbf, patchi)
    {
        if (isA<fixedJumpFvPatchScalarField>(tbf[patchi]))
        {
            const fixedJumpFvPatchScalarField& pf =
                dynamic_cast<const fixedJumpFvPatchScalarField&>(tbf[patchi]);

            hbt[patchi] = pf.interfaceFieldType();
        }
        else if (isA<fixedJumpAMIFvPatchScalarField>(tbf[patchi]))
        {
            const fixedJumpAMIFvPatchScalarField& pf =
                dynamic_cast<const fixedJumpAMIFvPatchScalarField&>
                (
                    tbf[patchi]
                );

            hbt[patchi] = pf.interfaceFieldType();
        }
    }

    return hbt;
}


Foam::wordList Foam::multi2Thermo::hevelMix2BoundaryTypes()
{
    const volScalarField::Boundary& tbf = this->Tv_.boundaryField();

    wordList hbt = tbf.types();

    forAll(tbf, patchi)
    {
        if (isA<fixedValueFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = fixed2VELMixEnergyFvPatchScalarField::typeName;
        }
        else if
        (
            isA<zeroGradientFvPatchScalarField>(tbf[patchi])
         || isA<fixedGradientFvPatchScalarField>(tbf[patchi])
        )
        {
            hbt[patchi] = gradient2VELMixEnergyFvPatchScalarField::typeName;
        }
        else if (isA<mixedFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = mixed2VELMixEnergyFvPatchScalarField::typeName;
        }
        else if (isA<fixedJumpFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = energyJumpFvPatchScalarField::typeName;
        }
        else if (isA<fixedJumpAMIFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = energyJumpAMIFvPatchScalarField::typeName;
        }
        else if (tbf[patchi].type() == "energyRegionCoupledFvPatchScalarField")
        {
            hbt[patchi] = "energyRegionCoupledFvPatchScalarField";
        }
    }

    return hbt;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::multi2Thermo::multi2Thermo(const fvMesh& mesh, const word& phaseName)
:
    fluid2Thermo(mesh, phaseName),

    twoTemperatureDictionary_
    (
        IOobject
        (
            phasePropertyName
            (
                fileName(lookup("twoTemperatureDictFile")).name(),
                phaseName
            ),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),

    Tov_
    (
        IOobject
        (
            phasePropertyName("Tov"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimTemperature
    ),

    Tv_
    (
        IOobject
        (
            phasePropertyName("Tv"),
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        dimTemperature
    ),

    hevMix_
    (
        IOobject
        (
            phasePropertyName("hev"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass
    ),

    heelMix_
    (
        IOobject
        (
            phasePropertyName("heel"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass
    ),

    CvtrMix_
    (
        IOobject
        (
            phasePropertyName("Cvtr"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CptrMix_
    (
        IOobject
        (
            phasePropertyName("Cptr"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CvvelMix_
    (
        IOobject
        (
            phasePropertyName("Cvvel"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CpvelMix_
    (
        IOobject
        (
            phasePropertyName("Cpvel"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    RMix_
    (
        IOobject
        (
            phasePropertyName("Rmix"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CvMix_
    (
        IOobject
        (
            phasePropertyName("Cv"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CpMix_
    (
        IOobject
        (
            phasePropertyName("Cp"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    zetar_
    (
        IOobject
        (
            phasePropertyName("zetar"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    ),

    zetav_
    (
        IOobject
        (
            phasePropertyName("zetav"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    ),

    zetael_
    (
        IOobject
        (
            phasePropertyName("zetael"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    ),

    downgradeSingleTv_(lookupOrDefault<Switch>("downgradeToSingleTv", false)),

    downgradeSingleTemperature_
    (
        lookupOrDefault<Switch>("downgradeToSingleTemperature", false)
    ),

    downgradeSingleVibMode_
    (
        lookupOrDefault<Switch>("downgradeToSingleVibMode", true)
    ),

    hyLight_(false) // TODO 2021 lookupOrDefault<Switch>("hyLight", true))
{
    //- het_ volScalaField defined in the constructor of rho2ReactionThermo
    //  for boundary condition implementation. Indeed, it will change the
    //  lookup of thermophysicalProperties from objectRegistry from
    //  multi2Thermo to rho2ReactionThermo
        
    if(downgradeSingleTv_)
    {
        //- hevelMix_ volScalaField defined in the constructor of
        //  rho2ReactionThermo for boundary condition implementation. Same
        //  reason than for het_
    }
    else
    {
        hevelMix_ = new volScalarField
        (
            IOobject
            (
                phasePropertyName("hevel"),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimEnergy/dimMass
        );
    }
}


Foam::multi2Thermo::multi2Thermo
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
:
    fluid2Thermo(mesh, dict, phaseName),

    twoTemperatureDictionary_
    (
        IOobject
        (
            phasePropertyName
            (
                fileName(lookup("twoTemperatureDictFile")).substr
                (
                    fileName(lookup("twoTemperatureDictFile"))
                        .find("constant/") + 9
                ),
                phaseName
            ),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),

    Tov_
    (
        IOobject
        (
            phasePropertyName("Tov"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimTemperature
    ),

    Tv_
    (
        IOobject
        (
            phasePropertyName("Tv"),
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        dimTemperature
    ),

    hevMix_
    (
        IOobject
        (
            phasePropertyName("hev"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass
    ),

    heelMix_
    (
        IOobject
        (
            phasePropertyName("heel"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass
    ),

    CvtrMix_
    (
        IOobject
        (
            phasePropertyName("Cvtr"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CptrMix_
    (
        IOobject
        (
            phasePropertyName("Cptr"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CvvelMix_
    (
        IOobject
        (
            phasePropertyName("Cvvel"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CpvelMix_
    (
        IOobject
        (
            phasePropertyName("Cpvel"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    RMix_
    (
        IOobject
        (
            phasePropertyName("Rmix"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CvMix_
    (
        IOobject
        (
            phasePropertyName("Cv"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    CpMix_
    (
        IOobject
        (
            phasePropertyName("Cp"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass/dimTemperature
    ),
    
    zetar_
    (
        IOobject
        (
            phasePropertyName("zetar"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    ),

    zetav_
    (
        IOobject
        (
            phasePropertyName("zetav"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    ),

    zetael_
    (
        IOobject
        (
            phasePropertyName("zetael"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    ),

    downgradeSingleTv_(lookupOrDefault<Switch>("downgradeToSingleTv", false)),

    downgradeSingleTemperature_
    (
        lookupOrDefault<Switch>("downgradeToSingleTemperature", false)
    ),

    downgradeSingleVibMode_
    (
        lookupOrDefault<Switch>("downgradeToSingleVibMode", true)
    ),

    hyLight_(lookupOrDefault<Switch>("hyLight", true))
{
    //- het_ volScalaField defined in the constructor of rho2ReactionThermo
    //  for boundary condition implementation. Indeed, it will change the
    //  lookup of thermophysicalProperties from objectRegistry from
    //  multi2Thermo to rho2ReactionThermo
        
    if(downgradeSingleTv_)
    {
        //- hevelMix_ volScalaField defined in the constructor of
        //  rho2ReactionThermo for boundary condition implementation. Same
        //  reason than for het_
    }
    else
    {
        hevelMix_ = new volScalarField
        (
            IOobject
            (
                phasePropertyName("hevel"),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimEnergy/dimMass
        );
    }
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::multi2Thermo> Foam::multi2Thermo::New
(
    const fvMesh& mesh,
    const word& phaseName
)
{
    return basic2Thermo::New<multi2Thermo>(mesh, phaseName);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::multi2Thermo::~multi2Thermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::multi2Thermo& Foam::multi2Thermo::lookup2Thermo
(
    const fvPatchScalarField& pf
)
{
    return pf.db().lookupObject<multi2Thermo>(dictName);
}


const Foam::volScalarField& Foam::multi2Thermo::CvtrMix() const
{
    return CvtrMix_;
}


Foam::volScalarField& Foam::multi2Thermo::CvtrMix()
{
    return CvtrMix_;
}


const Foam::volScalarField& Foam::multi2Thermo::CptrMix() const
{
    return CptrMix_;
}


Foam::volScalarField& Foam::multi2Thermo::CptrMix()
{
    return CptrMix_;
}


const Foam::volScalarField& Foam::multi2Thermo::CvvelMix() const
{
    return CvvelMix_;
}


Foam::volScalarField& Foam::multi2Thermo::CvvelMix()
{
    return CvvelMix_;
}


const Foam::volScalarField& Foam::multi2Thermo::CpvelMix() const
{
    return CpvelMix_;
}


Foam::volScalarField& Foam::multi2Thermo::CpvelMix()
{
    return CpvelMix_;
}


const Foam::volScalarField& Foam::multi2Thermo::CvMix() const
{
    return CvMix_;
}


Foam::volScalarField& Foam::multi2Thermo::CvMix()
{
    return CvMix_;
}


const Foam::volScalarField& Foam::multi2Thermo::CpMix() const
{
    return CpMix_;
}


const Foam::scalarField& Foam::multi2Thermo::CpMix(const label patchi) const
{
    return CpMix_.boundaryField()[patchi];
}


Foam::volScalarField& Foam::multi2Thermo::CpMix()
{
    return CpMix_;
}


Foam::tmp<Foam::volScalarField> Foam::multi2Thermo::Cp_t() const
{
    tmp<volScalarField> tCptr(CptrMix());

    return tCptr;
}


const Foam::volScalarField& Foam::multi2Thermo::Tov() const
{
    return Tov_;
}


const Foam::scalarField& Foam::multi2Thermo::Tov(const label patchi) const
{
    return Tov_.boundaryField()[patchi];
}


Foam::volScalarField& Foam::multi2Thermo::Tov()
{
    return Tov_;
}


const Foam::volScalarField& Foam::multi2Thermo::Tv() const
{
    return Tv_;
}


const Foam::scalarField& Foam::multi2Thermo::Tv(const label patchi) const
{
    return Tv_.boundaryField()[patchi];
}


Foam::volScalarField& Foam::multi2Thermo::Tv()
{
    return Tv_;
}


const Foam::volScalarField& Foam::multi2Thermo::RMix() const
{
    return RMix_;
}


const Foam::scalarField& Foam::multi2Thermo::RMix(const label patchi) const
{
    return RMix_.boundaryField()[patchi];
}


Foam::volScalarField& Foam::multi2Thermo::RMix()
{
    return RMix_;
}


const Foam::volScalarField& Foam::multi2Thermo::zetar() const
{
    return zetar_;
}


Foam::volScalarField& Foam::multi2Thermo::zetar()
{
    return zetar_;
}


const Foam::volScalarField& Foam::multi2Thermo::zetav() const
{
    return zetav_;
}


Foam::volScalarField& Foam::multi2Thermo::zetav()
{
    return zetav_;
}


const Foam::volScalarField& Foam::multi2Thermo::zetael() const
{
    return zetael_;
}


Foam::volScalarField& Foam::multi2Thermo::zetael()
{
    return zetael_;
}


bool Foam::multi2Thermo::downgradeSingleTv() const
{
    return downgradeSingleTv_;
}


bool Foam::multi2Thermo::downgradeSingleTemperature() const
{
    return downgradeSingleTemperature_;
}


bool Foam::multi2Thermo::downgradeSingleVibMode() const
{
    return downgradeSingleVibMode_;
}


bool Foam::multi2Thermo::hyLight() const
{
    return hyLight_;
}

// ************************************************************************* //

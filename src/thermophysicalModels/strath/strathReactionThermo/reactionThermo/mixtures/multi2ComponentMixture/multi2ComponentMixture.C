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

#include "multi2ComponentMixture.H"
#include "IFstream.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& Foam::multi2ComponentMixture<ThermoType>::constructSpeciesData
(
    const dictionary& thermoDict
)
{
    forAll(species_, i)
    {
        speciesData_.set
        (
            i,
            new ThermoType(thermoDict.subDict(species_[i]))
        );
    }

    return speciesData_[0];
}


template<class ThermoType>
void Foam::multi2ComponentMixture<ThermoType>::correctMassFractions()
{
    bool initialiseY = true;

    forAll(Y_, speciei)
    {
        const scalarField& YCells = Y_[speciei].internalField();
        if (YCells[0] >= 1e-4)
        {
            initialiseY = false;
            break;
        };
    }

    if (initialiseY)
    {
        // Molar fractions are given as an input in the 0/ folder: compute
        // mass fractions
        
        // Multiplication by 1.0 changes Xt patches to "calculated"
        volScalarField Xt("Xt", 1.0*X_[0]);
    
        // First ensure that the sum of molar fractions is equal to 1
        for (label n=1; n<X_.size(); n++)
        {
            Xt += X_[n];
        }

        if (mag(max(Xt).value()) < ROOTVSMALL)
        {
            FatalErrorIn
            (
                "void Foam::multi2ComponentMixture<ThermoType>::"
                    "correctMassFractions()"
            )
                << "Sum of molar fractions is zero for species "
                << this->species() << exit(FatalError);
        }

        forAll(X_, n)
        {
            X_[n] /= Xt;
        }
    
        // Then compute mass fractions for all cells and boundary patches
        forAll(Y_, speciei)
        {
            const scalarField& XCells = X_[speciei].internalField();
            scalarField& YCells = Y_[speciei].primitiveFieldRef();
            
            forAll(YCells, celli)
            {
                const scalar Wmix = molWeightMixture(celli, true);
                YCells[celli] =
                    massFractionFromMolarFraction(speciei, XCells[celli], Wmix);
            }
        }

        forAll(Y_[0].boundaryField(), patchi)
        {
            forAll(Y_, speciei)
            {
                const fvPatchScalarField& pX =
                    X_[speciei].boundaryField()[patchi];
                fvPatchScalarField& pY = Y_[speciei].boundaryFieldRef()[patchi];

                forAll(pY, facei)
                {
                    const scalar pWmix = molWeightMixture(patchi, facei, true);
                    pY[facei] =
                        massFractionFromMolarFraction
                        (
                            speciei,
                            pX[facei],
                            pWmix
                        );
                }
            }
        }
    }
    else
    {
        // Mass fractions are given as an input in the 0/ folder: compute
        // molar fractions
        
        // Multiplication by 1.0 changes Yt patches to "calculated"
        volScalarField Yt("Yt", 1.0*Y_[0]);

        // First ensure that the sum of mass fractions is equal to 1
        for (label n=1; n<Y_.size(); n++)
        {
            Yt += Y_[n];
        }

        if (mag(max(Yt).value()) < ROOTVSMALL)
        {
            FatalErrorIn
            (
                "void Foam::multi2ComponentMixture<ThermoType>::"
                    "correctMassFractions()"
            )
                << "Sum of mass fractions is zero for species "
                << this->species() << exit(FatalError);
        }

        forAll(Y_, n)
        {
            Y_[n] /= Yt;
        }
    
        // Molar fractions are updated here to move the call the 
        // correctChemFractions() in rho2ReactionThermo::initialise() to the
        // end. The reason is that the density field may differ from the one
        // computed in rho2Thermo (a-priori estimate for the single-temperature
        // model). Thus, partial densities and number densities would need to
        // be recomputed.
        
        forAll(Y_, speciei)
        {
            const scalarField& YCells = Y_[speciei].internalField();
            scalarField& XCells = X_[speciei].primitiveFieldRef();
            
            forAll(YCells, celli)
            {
                XCells[celli] =
                    molarFraction(speciei, YCells[celli], celli);
            }
        }

        forAll(Y_[0].boundaryField(), patchi)
        {
            forAll(Y_, speciei)
            {
                const fvPatchScalarField& pY =
                    Y_[speciei].boundaryField()[patchi];
                fvPatchScalarField& pX = X_[speciei].boundaryFieldRef()[patchi];

                forAll(pY, facei)
                {
                    pX[facei] =
                        molarFraction
                        (
                            speciei,
                            pY[facei],
                            patchi,
                            facei
                        );
                }
            }
        }
    }
}


template<class ThermoType>
void Foam::multi2ComponentMixture<ThermoType>::fillSpeciesTablesAndLists
(
    const label i
)
{
    if (speciesData_[i].particleCharge() != -1)
    {
        // This species is a heavy species, i.e., not free-electrons
        const label prv_size = heavySpeciesIds_.size();
        heavySpeciesIds_.setSize(prv_size + 1);
        heavySpecies_.append(species_[i]);
        heavySpeciesIds_[prv_size] = i;
        
        if (speciesData_[i].noVibrationalTemp() != 0)
        {
            // This species is either a neutral or an ionised molecule
            const label prv_size = moleculeIds_.size();
            moleculeIds_.setSize(prv_size + 1);
            molecules_.append(species_[i]);
            moleculeIds_[prv_size] = i;
        }
        else
        {
            // This species is either a neutral or an ionised atom
            const label prv_size = atomIds_.size();
            atomIds_.setSize(prv_size + 1);
            atoms_.append(species_[i]);
            atomIds_[prv_size] = i;
        }
    }
    else
    {
        electronId_ = i;
    }
    
    if (speciesData_[i].particleCharge() == 1)
    {
        // This species is ionised
        const label prv_size = ionIds_.size();
        ionIds_.setSize(prv_size + 1);
        ions_.append(species_[i]);
        ionIds_[prv_size] = i;
    }
}


template<class ThermoType>
void Foam::multi2ComponentMixture<ThermoType>::correctVibTempAssociativity
(
    const dictionary& dict
)
{
    const dictionary chemDict
    (
        IFstream
        (
            fileName(dict.lookup("foamChemistryFile")).expand()
        )()
    );

    labelList vTA(chemDict.lookup("vibTempAssociativity"));
    label solvedVibEqCounter = 0;

    forAll(Y_, speciei)
    {
        if (vTA[speciei] < 1)
        {
            if (speciesData_[speciei].particleType() != 2)
            {
                FatalErrorIn
                (
                    "multi2ComponentMixture<ThermoType>::"
                        "correctVibTempAssociativity"
                )   << "The vibTempAssociativity table is not correctly "
                    << "defined \n in " << chemDict.name() << "\nfor the "
                    << "element no " << speciei+1 << "." << nl << "The value "
                    << "'0' can be assigned to molecules/ions only." << nl;
                FatalError<< exit(FatalError);
            }
            else
            {
                vibTempAssociativity_[speciei] = -1;
                solvedVibEqCounter += 1;
            }
        }
        else
        {
            vibTempAssociativity_[speciei] = vTA[speciei] - 1;
        }
    }

    forAll(Y_, speciei)
    {
        if (vTA[speciei] > solvedVibEqCounter)
        {
            FatalErrorIn
            (
                "multi2ComponentMixture<ThermoType>::"
                    "correctVibTempAssociativity"
            )   << "The vibTempAssociativity table is not correctly defined \n"
                << "in " << chemDict.name() << "\nfor the element no "
                << speciei+1 << "." << nl << "The value is greater than the "
                << "number of solved vib. eqn. (i.e., " << solvedVibEqCounter
                <<")" << nl;
            FatalError<< exit(FatalError);
        }
    }
}


template<class ThermoType>
void Foam::multi2ComponentMixture<ThermoType>::fillSolvedVibEqSpeciesTable()
{
    forAll(Y_, speciei)
    {
        if (vibTempAssociativity_[speciei] == -1)
        {
            solvedVibEqSpecies_.append(species_[speciei]);
        }
    }
}


template<class ThermoType>
void
Foam::multi2ComponentMixture<ThermoType>::fillHackOfSolvedVibEqSpeciesTable()
{
    forAll(Y_, speciei)
    {
        // This species is either a neutral or an ionised molecule
        if (speciesData_[speciei].noVibrationalTemp() != 0)
        {
            solvedVibEqSpecies_.append(species_[speciei]);
        }
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
Foam::multi2ComponentMixture<ThermoType>::multi2ComponentMixture
(
    const dictionary& thermoDict,
    const wordList& specieNames,
    const HashPtrTable<ThermoType>& thermoData,
    const fvMesh& mesh
)
:
    basic2MultiComponentMixture(thermoDict, specieNames, mesh),
    speciesData_(species_.size()),
    mixture_("mixture", *thermoData[specieNames[0]]),
    mixtureVol_("volMixture", *thermoData[specieNames[0]])
{
    forAll(species_, i)
    {
        speciesData_.set
        (
            i,
            new ThermoType(*thermoData[species_[i]])
        );
        
        fillSpeciesTablesAndLists(i);
    }
    
    correctMassFractions();

    if
    (
        not
        (
            thermoDict.lookupOrDefault<bool>
            (
                "downgradeToSingleTemperature",
                false
            )
         or thermoDict.lookupOrDefault<bool>("downgradeToSingleTv", false)
        )
    )
    {
        // Two-temperature solver with multiple vibrational pools
        correctVibTempAssociativity(thermoDict);
        fillSolvedVibEqSpeciesTable();
    }
    else if (thermoDict.lookupOrDefault<bool>("downgradeToSingleTv", true))
    {
        // Two-temperature solver with a single vibrational pool
        fillHackOfSolvedVibEqSpeciesTable();
    }
}


template<class ThermoType>
Foam::multi2ComponentMixture<ThermoType>::multi2ComponentMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh
)
:
    basic2MultiComponentMixture(thermoDict, thermoDict.lookup("species"), mesh),
    speciesData_(species_.size()),
    mixture_("mixture", constructSpeciesData(thermoDict)),
    mixtureVol_("volMixture", speciesData_[0])
{
    forAll(species_, i)
    {
        fillSpeciesTablesAndLists(i);
    }
    
    correctMassFractions();

    if
    (
        not
        (
            thermoDict.lookupOrDefault<bool>
            (
                "downgradeToSingleTemperature",
                false
            )
         or thermoDict.lookupOrDefault<bool>("downgradeToSingleTv", true)
        )
    )
    {
        // Two-temperature solver with multiple vibrational energy pools
        correctVibTempAssociativity(thermoDict);
        fillSolvedVibEqSpeciesTable();
    }
    else if (thermoDict.lookupOrDefault<bool>("downgradeToSingleTv", true))
    {
        // Two-temperature solver with a single vibrational energy pool
        fillHackOfSolvedVibEqSpeciesTable();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& Foam::multi2ComponentMixture<ThermoType>::cellMixture
(
    const label celli
) const
{
    mixture_ = Y_[0][celli]/speciesData_[0].W()*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixture_ += Y_[n][celli]/speciesData_[n].W()*speciesData_[n];
    }

    return mixture_;
}


template<class ThermoType>
const ThermoType& Foam::multi2ComponentMixture<ThermoType>::patchFaceMixture
(
    const label patchi,
    const label facei
) const
{
    mixture_ =
        Y_[0].boundaryField()[patchi][facei]
       /speciesData_[0].W()*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixture_ +=
            Y_[n].boundaryField()[patchi][facei]
           /speciesData_[n].W()*speciesData_[n];
    }

    return mixture_;
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::patchFaceMixture
(
    scalar (ThermoType::*F)(const scalar, const scalar) const,
    const label patchi,
    const label facei,
    const scalar p,
    const scalar T
) const
{
    scalar quantity = 0;

    forAll(Y_, i)
    {
        quantity += Y_[i].boundaryField()[patchi][facei]
           /speciesData_[i].W()*(speciesData_[i].*F)(p, T);
    }

    return quantity;
}


template<class ThermoType>
const ThermoType& Foam::multi2ComponentMixture<ThermoType>::cellVolMixture
(
    const scalar p,
    const scalar T,
    const label celli
) const
{
    scalar rhoInv = 0.0;
    forAll(speciesData_, i)
    {
        rhoInv += Y_[i][celli]/speciesData_[i].rho(p, T);
    }

    mixtureVol_ =
        Y_[0][celli]/speciesData_[0].rho(p, T)/rhoInv*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixtureVol_ +=
            Y_[n][celli]/speciesData_[n].rho(p, T)/rhoInv*speciesData_[n];
    }

    return mixtureVol_;
}


template<class ThermoType>
const ThermoType& Foam::multi2ComponentMixture<ThermoType>::
patchFaceVolMixture
(
    const scalar p,
    const scalar T,
    const label patchi,
    const label facei
) const
{
    scalar rhoInv = 0.0;
    forAll(speciesData_, i)
    {
        rhoInv +=
            Y_[i].boundaryField()[patchi][facei]/speciesData_[i].rho(p, T);
    }

    mixtureVol_ =
        Y_[0].boundaryField()[patchi][facei]/speciesData_[0].rho(p, T)/rhoInv
      * speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixtureVol_ +=
            Y_[n].boundaryField()[patchi][facei]/speciesData_[n].rho(p,T)
          / rhoInv*speciesData_[n];
    }

    return mixtureVol_;
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::molWeightMixture
(
    const label celli,
    const bool fromMolarFrac
) const
{
    if (fromMolarFrac)
    {
        scalar molWmix = 0.0;
        forAll(speciesData_, i)
        {
            molWmix += X_[i][celli]*speciesData_[i].W();
        }

        return molWmix;
    }
    else
    {
        scalar invMolWmix = 0.0;
        forAll(speciesData_, i)
        {
            invMolWmix += Y_[i][celli]/speciesData_[i].W();
        }
        
        return 1.0/invMolWmix;
    }
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::molWeightMixture
(
    const label patchi,
    const label facei,
    const bool fromMolarFrac
) const
{
    if (fromMolarFrac)
    {
        scalar molWmix = 0.0;
        forAll(speciesData_, i)
        {
            molWmix +=
                X_[i].boundaryField()[patchi][facei]*speciesData_[i].W();
        }

        return molWmix;
    }
    else
    {
        scalar invMolWmix = 0.0;
        forAll(speciesData_, i)
        {
            invMolWmix +=
                Y_[i].boundaryField()[patchi][facei]/speciesData_[i].W();
        }

        return 1.0/invMolWmix;
    }
}


template<class ThermoType>
Foam::tmp<Foam::volScalarField>
Foam::multi2ComponentMixture<ThermoType>::molWeightMixture() const
{
    const fvMesh& mesh = Y_[0].mesh();

    tmp<volScalarField> tmolWeightMixture
    (
        new volScalarField
        (
            IOobject
            (
                "molWeightMixture",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionedScalar("molWeightMixture", dimMass/dimMoles, 0.0)
        )
    );

    volScalarField& molWeightMixture = tmolWeightMixture.ref();
    scalarField& molWeightMixtureCells = molWeightMixture.primitiveFieldRef();

    forAll(molWeightMixtureCells, celli)
    {
        molWeightMixtureCells[celli] = this->molWeightMixture(celli);
    }

    forAll(molWeightMixture.boundaryField(), patchi)
    {
        fvPatchScalarField& pmolWeightMixture =
            molWeightMixture.boundaryFieldRef()[patchi];

        forAll(pmolWeightMixture, facei)
        {
            pmolWeightMixture[facei] = this->molWeightMixture(patchi, facei);
        }
    }

    return tmolWeightMixture;
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::RspecificMixture
(
    const label celli
) const
{
    scalar invMolWmix = 0.0;
    forAll(speciesData_, i)
    {
        invMolWmix += Y_[i][celli]/speciesData_[i].W();
    }

    return constant::physicoChemical::R.value()*invMolWmix;
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::RspecificMixture
(
    const label patchi,
    const label facei
) const
{
    scalar invMolWmix = 0.0;
    forAll(speciesData_, i)
    {
        invMolWmix += Y_[i].boundaryField()[patchi][facei]/speciesData_[i].W();
    }

    return constant::physicoChemical::R.value()*invMolWmix;
}


template<class ThermoType>
Foam::scalar
Foam::multi2ComponentMixture<ThermoType>::massFractionFromMolarFraction
(
    const label speciei,
    const scalar Xi,
    const scalar Wmix
)
{
    return Xi*speciesData_[speciei].W()/Wmix;
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::molarFraction
(
    const label speciei,
    const scalar Yi,
    const label celli
)
{
    return max(Yi*molWeightMixture(celli)/speciesData_[speciei].W(), 0.0);
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::molarFraction
(
    const label speciei,
    const scalar Yi,
    const label patchi,
    const label facei
)
{
    return
        max
        (
            Yi*molWeightMixture(patchi, facei)/speciesData_[speciei].W(),
            0.0
        );
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::numberDensity
(
    const label speciei,
    const scalar Yi,
    const scalar rho
)
{
    return 
        max
        (
            rho*Yi*Foam::constant::physicoChemical::NA.value()
                /(1.0e-3*speciesData_[speciei].W()),
            0.0
        );
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::partialPressureEoS
(
    const label speciei,
    const scalar rhoi,
    const scalar Ti
)
{
    return rhoi*speciesData_[speciei].R()*Ti;
}


template<class ThermoType>
Foam::scalar Foam::multi2ComponentMixture<ThermoType>::partialDensity
(
    const scalar Yi,
    const scalar rho
)
{
    return max(Yi*rho, 0.0);
}


template<class ThermoType>
void Foam::multi2ComponentMixture<ThermoType>::read
(
    const dictionary& thermoDict
)
{
    forAll(species_, i)
    {
        speciesData_[i] = ThermoType(thermoDict.subDict(species_[i]));
    }
}


// ************************************************************************* //

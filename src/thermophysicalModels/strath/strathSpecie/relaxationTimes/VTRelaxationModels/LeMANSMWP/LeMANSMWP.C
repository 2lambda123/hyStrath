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

#include "LeMANSMWP.H"
#include "addToRunTimeSelectionTable.H"
#include "DynamicList.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    namespace VTRelaxationModels
    {
        defineTypeNameAndDebug(LeMANSMWP, 0);
        addToRunTimeSelectionTable
        (
            VTRelaxationModel,
            LeMANSMWP,
            dictionary
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::VTRelaxationModels::LeMANSMWP::LeMANSMWP
(
    const word& name1,
    const word& name2,
    const label& lname1,
    const label& lname2,
    const dictionary& dict2T,
    const dictionary& dictThermoPhy,
    const volScalarField& p,
    const volScalarField& T,
    const PtrList<volScalarField>& Tv,
    const PtrList<volScalarField>& nD
)
:
    VTRelaxationModel
    (
        name1,
        name2,
        lname1,
        lname2,
        dict2T,
        dictThermoPhy,
        p,
        T,
        Tv,
        nD
    )
{
    species1_ = lname1;
    species2_ = lname2;
    W1_ =
        readScalar
        (
            dictThermoPhy.subDict(name1).subDict("specie").lookup("molWeight")
        );

    word subDictName = word::null;
    
    const word collPair = name1 + "_" + name2;
    const word invCollPair = name2 + "_" + name1;

    if (not VTFullCoeffsForm_)
    {
        const scalar W2 =
            readScalar
            (
                dictThermoPhy.subDict(name2).subDict("specie")
                    .lookup("molWeight")
            );
        DynamicList<scalar> vibData
        (
            dictThermoPhy.subDict(name1).subDict("thermodynamics")
                .lookup("vibrationalList")
        );
        const scalar thetav1 = vibData[1];

        scalar W12 = (W1_ * W2) / (W1_ + W2);
        A12_ = sqrt(W12) * pow(thetav1, 4.0/3.0);
        B12_ = pow(W12, 0.25);
        scalar preAij = 0.0;
        scalar preMij = 0.0;

        if (not VTOverwriteDefault_)
        {
            preAij  = 1.16e-3;
            preMij  = 0.015;
            offset_ = 18.42;
            sigma1_ = 1.0e-21;
            sigma2_ = 5.0e4;
        }
        else
        {
            if (VTSpeciesDependent_ and VTCollidingPartner_)
            {
                if (dict2T.subDict("ParkCoefficients").isDict(collPair))
                {
                    subDictName = collPair;
                }
                else if (dict2T.subDict("ParkCoefficients").isDict(invCollPair))
                {
                    subDictName = invCollPair;
                }
                else if (dict2T.subDict("ParkCoefficients").isDict(name1))
                {
                    subDictName = name1;
                }
                else
                {
                    subDictName = "allSpecies";
                }
            }
            else if
            (
                VTSpeciesDependent_ 
             && dict2T.subDict("ParkCoefficients").isDict(name1)
            )
            {
                subDictName = name1;
            }
            else
            {
                subDictName = "allSpecies";
            }

            preAij =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("preAij")
                );
            preMij =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("preMij")
                );
            A12_ *= preAij;
            B12_ *= preMij;
            offset_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("offset")
                );
            sigma1_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("sigma1")
                );
            sigma2_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("sigma2")
                );
        }
    }
    else
    {
        if (not VTOverwriteDefault_)
        {
            A12_  = 221.53;
            B12_  = 0.029;
            offset_ = 18.42;
            sigma1_ = 1.0e-21;
            sigma2_ = 5.0e4;
        }
        else
        {
            if (VTSpeciesDependent_ and VTCollidingPartner_)
            {
                if (dict2T.subDict("ParkCoefficients").isDict(collPair))
                {
                    subDictName = collPair;
                }
                else if (dict2T.subDict("ParkCoefficients").isDict(invCollPair))
                {
                    subDictName = invCollPair;
                }
                else if (dict2T.subDict("ParkCoefficients").isDict(name1))
                {
                    subDictName = name1;
                }
                else
                {
                    subDictName = "allSpecies";
                }
            }
            else if
            (
                VTSpeciesDependent_
             && dict2T.subDict("ParkCoefficients").isDict(name1)
            )
            {
                subDictName = name1;
            }
            else
            {
                subDictName = "allSpecies";
            }

            A12_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("Aij")
                );
            B12_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("Bij")
                );
            offset_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("offset")
                );
            sigma1_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("sigma1")
                );
            sigma2_ =
                readScalar
                (
                    dict2T.subDict("ParkCoefficients").subDict(subDictName)
                        .lookup("sigma2")
                );
        }
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::VTRelaxationModels::LeMANSMWP::tauVT() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> ttauVT
    (
        new volScalarField
        (
            IOobject
            (
                "tauVT_" + name1_ + "_" + name2_,
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionSet(0, 0, 1, 0, 0)
        )
    );

    volScalarField& tauVT = ttauVT.ref();

    forAll(this->T_, celli)
    {
        const scalar T = this->T_[celli];
        
        tauVT[celli] =
            1.01325e5 / this->p_[celli]
          * exp(A12_*(pow(T, -1.0/3.0) - B12_) - offset_);
    }


    forAll(this->T_.boundaryField(), patchi)
    {
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        fvPatchScalarField& ptauVT = tauVT.boundaryFieldRef()[patchi];

        forAll(pT, facei)
        {
            const scalar T = pT[facei];
            
            ptauVT[facei] =
                1.01325e5 / pp[facei]
              * exp(A12_*(pow(T, -1.0/3.0) - B12_) - offset_);
        }
    }

    return ttauVT;
}


Foam::tmp<Foam::volScalarField>
Foam::VTRelaxationModels::LeMANSMWP::tauVTcorr() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> ttauVTcorr
    (
        new volScalarField
        (
            IOobject
            (
                "tauVTcorr_" + name1_,
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionSet(0, 0, 1, 0, 0)
        )
    );

    volScalarField& tauVTcorr = ttauVTcorr.ref();

    forAll(this->T_, celli)
    {
        const scalar T = this->T_[celli];
        const scalar ndMix =
            this->p_[celli]/(constant::physicoChemical::k.value()*T);
        
        tauVTcorr[celli] =
            1.0
            /
              (
                  sqrt
                  (
                      8.0*constant::physicoChemical::R.value()*1000.0
                    * T/(constant::mathematical::pi*W1_)
                  ) 
                * sigma1_*sqr(sigma2_/T)*ndMix
              );
    }

    forAll(this->T_.boundaryField(), patchi)
    {
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        fvPatchScalarField& ptauVTcorr = tauVTcorr.boundaryFieldRef()[patchi];

        forAll(pT, facei)
        {
            const scalar T = pT[facei];
            const scalar ndMix =
                pp[facei]/(constant::physicoChemical::k.value()*T);
            
            ptauVTcorr[facei] =
                1.0
                /
                  (
                      sqrt
                      (
                          8.0*constant::physicoChemical::R.value()*1000.0
                        * T/(constant::mathematical::pi*W1_)
                      )
                    * sigma1_*sqr(sigma2_/T)*ndMix
                  );
        }
    }

    return ttauVTcorr;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

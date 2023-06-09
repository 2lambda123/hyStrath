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

#include "advancedSpecie.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Thermo>
inline void Foam::SutherlandEuckenTransport<Thermo>::calcCoeffs
(
    const scalar mu1, const scalar T1,
    const scalar mu2, const scalar T2
)
{
    scalar rootT1 = sqrt(T1);
    scalar mu1rootT2 = mu1*sqrt(T2);
    scalar mu2rootT1 = mu2*rootT1;

    Ts_ = (mu2rootT1 - mu1rootT2)/(mu1rootT2/T1 - mu2rootT1/T2);

    As_ = mu1*(1.0 + Ts_/T1)/rootT1;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo>::SutherlandEuckenTransport
(
    const Thermo& t,
    const scalar As,
    const scalar Ts,
    const scalar eta_s
)
:
    Thermo(t),
    As_(As),
    Ts_(Ts),
    eta_s_(eta_s)
{}


template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo>::SutherlandEuckenTransport
(
    const Thermo& t,
    const scalar mu1, const scalar T1,
    const scalar mu2, const scalar T2
)
:
    Thermo(t)
{
    calcCoeffs(mu1, T1, mu2, T2);
}


template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo>::SutherlandEuckenTransport
(
    const word& name,
    const SutherlandEuckenTransport& st
)
:
    Thermo(name, st),
    As_(st.As_),
    Ts_(st.Ts_),
    eta_s_(st.eta_s_)
{}


template<class Thermo>
inline Foam::autoPtr<Foam::SutherlandEuckenTransport<Thermo> >
Foam::SutherlandEuckenTransport<Thermo>::clone() const
{
    return autoPtr<SutherlandEuckenTransport<Thermo> >
    (
        new SutherlandEuckenTransport<Thermo>(*this)
    );
}


template<class Thermo>
inline Foam::autoPtr<Foam::SutherlandEuckenTransport<Thermo> >
Foam::SutherlandEuckenTransport<Thermo>::New
(
    Istream& is
)
{
    return autoPtr<SutherlandEuckenTransport<Thermo> >
    (
        new SutherlandEuckenTransport<Thermo>(is)
    );
}


template<class Thermo>
inline Foam::autoPtr<Foam::SutherlandEuckenTransport<Thermo> >
Foam::SutherlandEuckenTransport<Thermo>::New
(
    const dictionary& dict
)
{
    return autoPtr<SutherlandEuckenTransport<Thermo> >
    (
        new SutherlandEuckenTransport<Thermo>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::scalar Foam::SutherlandEuckenTransport<Thermo>::mu
(
    const scalar p,
    const scalar T
) const
{
    return As_*::sqrt(T)/(1.0 + Ts_/T);
}


template<class Thermo>
inline Foam::scalar Foam::SutherlandEuckenTransport<Thermo>::kappatrans
(
    const scalar p,
    const scalar Tt
) const
{
    return kappatr(p, Tt) - kappar(p, Tt);
}


template<class Thermo>
inline Foam::scalar Foam::SutherlandEuckenTransport<Thermo>::kappar
(
    const scalar p,
    const scalar Tt
) const
{
    return mu(p, Tt)*this->Cv_r(p, Tt);
}


template<class Thermo>
inline Foam::scalar Foam::SutherlandEuckenTransport<Thermo>::kappatr
(
    const scalar p,
    const scalar Tt
) const
{
    scalar Cv = this->Cv_t(p, Tt);
    if (Cv > Foam::SMALL)
    {
        return mu(p, Tt)*Cv*(1.32 + 1.77*this->R()/Cv);
    }
    else
    {
        return 0.0;
    }
}


template<class Thermo>
inline Foam::scalar Foam::SutherlandEuckenTransport<Thermo>::kappave
(
    const scalar p,
    const scalar Tt,
    const scalar Tve
) const
{
    return eta_s_*mu(p, Tt)*this->Cv_vel(p, Tve);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo>&
Foam::SutherlandEuckenTransport<Thermo>::operator=
(
    const SutherlandEuckenTransport<Thermo>& st
)
{
    Thermo::operator=(st);

    As_ = st.As_;
    Ts_ = st.Ts_;
    eta_s_ = st.eta_s_;

    return *this;
}


template<class Thermo>
inline void Foam::SutherlandEuckenTransport<Thermo>::operator+=
(
    const SutherlandEuckenTransport<Thermo>& st
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator+=(st);

    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();

    As_ = molr1*As_ + molr2*st.As_;
    Ts_ = molr1*Ts_ + molr2*st.Ts_;
    eta_s_ = molr1*eta_s_ + molr2*st.eta_s_;
}


template<class Thermo>
inline void Foam::SutherlandEuckenTransport<Thermo>::operator-=
(
    const SutherlandEuckenTransport<Thermo>& st
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator-=(st);

    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();

    As_ = molr1*As_ - molr2*st.As_;
    Ts_ = molr1*Ts_ - molr2*st.Ts_;
    eta_s_ = molr1*eta_s_ - molr2*st.eta_s_;
}


template<class Thermo>
inline void Foam::SutherlandEuckenTransport<Thermo>::operator*=
(
    const scalar s
)
{
    Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo> Foam::operator+
(
    const SutherlandEuckenTransport<Thermo>& st1,
    const SutherlandEuckenTransport<Thermo>& st2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)
    );

    scalar molr1 = st1.nMoles()/t.nMoles();
    scalar molr2 = st2.nMoles()/t.nMoles();

    return SutherlandEuckenTransport<Thermo>
    (
        t,
        molr1*st1.As_ + molr2*st2.As_,
        molr1*st1.Ts_ + molr2*st2.Ts_,
        molr1*st1.eta_s_ + molr2*st2.eta_s_
    );
}


template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo> Foam::operator-
(
    const SutherlandEuckenTransport<Thermo>& st1,
    const SutherlandEuckenTransport<Thermo>& st2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(st1) - static_cast<const Thermo&>(st2)
    );

    scalar molr1 = st1.nMoles()/t.nMoles();
    scalar molr2 = st2.nMoles()/t.nMoles();

    return SutherlandEuckenTransport<Thermo>
    (
        t,
        molr1*st1.As_ - molr2*st2.As_,
        molr1*st1.Ts_ - molr2*st2.Ts_,
        molr1*st1.eta_s_ - molr2*st2.eta_s_
    );
}


template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo> Foam::operator*
(
    const scalar s,
    const SutherlandEuckenTransport<Thermo>& st
)
{
    return SutherlandEuckenTransport<Thermo>
    (
        s*static_cast<const Thermo&>(st),
        st.As_,
        st.Ts_,
        st.eta_s_
    );
}


template<class Thermo>
inline Foam::SutherlandEuckenTransport<Thermo> Foam::operator==
(
    const SutherlandEuckenTransport<Thermo>& st1,
    const SutherlandEuckenTransport<Thermo>& st2
)
{
    return st2 - st1;
}


// ************************************************************************* //

//------------------------------------------------------------------------------
// Kurganov flux function
//------------------------------------------------------------------------------
void KurganovFlux
(
    const bool& downgradeSingleT,
    const bool& downgradeSingleTv,
    const bool& meshMoving,
    const scalar& mesh_phi,
    const vector& Sf,
    const scalar& cSf_pos,
    const scalar& cSf_neg,
    const scalar& rho_pos,
    const scalar& rho_neg,
    const PtrList<scalar>& pD_pos,
    const PtrList<scalar>& pD_neg,
    const vector& U_pos,
    const vector& U_neg,
    const vector& rhoU_pos,
    const vector& rhoU_neg,
    const scalar& p_pos,
    const scalar& p_neg,
    const PtrList<scalar>& evk_pos,
    const PtrList<scalar>& evk_neg,
    const scalar& e_pos,
    const scalar& e_neg,
    scalar& amaxSf,
    vector& aByU,
    scalar& rhoFlux,
    vector& rhoUFlux,
    PtrList<scalar>& rhoEvkFlux,
    scalar& rhoEFlux
)
{
    scalar phiv_pos = U_pos & Sf; //- Eq in the text after Eq.6, + direc
    scalar phiv_neg = U_neg & Sf; //- Eq in the text after Eq.6, - direc

    //- Make fluxes relative to mesh-motion
    if (meshMoving)
    {
        phiv_pos -= mesh_phi;
        phiv_neg -= mesh_phi;
    }

    //- Eq.8a
    const scalar ap = max(max(phiv_pos + cSf_pos, phiv_neg + cSf_neg), 0.0);

    //- Eq.8b
    const scalar am = min(min(phiv_pos - cSf_pos, phiv_neg - cSf_neg), 0.0);

    amaxSf = max(mag(am), mag(ap));

    const scalar a_pos = ap/(ap - am); //- Eq.9 KNP
    const scalar a_neg = 1.0 - a_pos; //- (1-alpha) with alpha as in Eq.9 KNP

    const scalar aSf = am*a_pos;

    aByU = (a_pos*U_pos + a_neg*U_neg); //- To be introduced into sigmaDotU

    phiv_pos *= a_pos; //- First factor in the RHS of Eq.7
    phiv_neg *= a_neg; //- Second factor in the RHS of Eq.7

    //- Factor of Psi_f+ in factorised Eq.7
    const scalar aphiv_pos = phiv_pos - aSf;
    //- Factor of Psi_f- in factorised Eq.7
    const scalar aphiv_neg = phiv_neg + aSf;

    // Reuse amaxSf for the maximum positive and negative fluxes
    // estimated by the central scheme
    amaxSf = max(mag(aphiv_pos), mag(aphiv_neg));


    // --- Fluxes

    //- Eq.7 applied for Phi = rho
    rhoFlux = aphiv_pos*rho_pos + aphiv_neg*rho_neg;

    //- Eq.7 applied for Phi = rhoU and extra term for pressure
    rhoUFlux =
        (aphiv_pos*rhoU_pos + aphiv_neg*rhoU_neg)
      + (a_pos*p_pos + a_neg*p_neg)*Sf;

    //- Eq.7 applied for Phi = rhoE and decomposed extra term for pressure
    //  (no omega_f for pressure terms, see def of aphiv_pos)
    rhoEFlux =
        aphiv_pos*(rho_pos*(e_pos + 0.5*magSqr(U_pos)) + p_pos)
      + aphiv_neg*(rho_neg*(e_neg + 0.5*magSqr(U_neg)) + p_neg)
      + aSf*(p_pos - p_neg);
      
    //- Make flux for pressure-work absolute
    if (meshMoving)
    {
        rhoEFlux += mesh_phi*(a_pos*p_pos + a_neg*p_neg);
    }

    if (downgradeSingleT)
    {
        rhoEvkFlux.clear();
    }
    else if (downgradeSingleTv)
    {
        rhoEvkFlux[0] =
            aphiv_pos*rho_pos*evk_pos[0]
          + aphiv_neg*rho_neg*evk_neg[0];
    }
    else
    {
        forAll(rhoEvkFlux, speciei)
        {
            rhoEvkFlux[speciei] =
                aphiv_pos*(pD_pos[speciei]*evk_pos[speciei])
              + aphiv_neg*(pD_neg[speciei]*evk_neg[speciei]);
        }
    }
}


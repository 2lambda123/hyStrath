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

Class
    Foam::mhd::electricalConductivityModel::SpitzerHarm

Description
    Spitzer-Harm electricalConductivity model for MHD terms

SourceFiles
    SpitzerHarm.C

\*---------------------------------------------------------------------------*/

#ifndef SpitzerHarm_H
#define SpitzerHarm_H

#include "electricalConductivityModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace mhd
{

/*---------------------------------------------------------------------------*\
                         Class SpitzerHarm Declaration
\*---------------------------------------------------------------------------*/

class SpitzerHarm
:
    public electricalConductivityModel
{
    // Private data
    
        //- Const reference to the electron pressure field
        const volScalarField& pe_;
        
        //- Local copy of the Boltzmann constant
        const scalar localkB_;


public:

    //- Runtime type information
    TypeName("SpitzerHarm");


    // Constructors

        //- Construct from components
        SpitzerHarm(const mhdModel& dict, const fvMesh& mesh);


    //- Destructor
    virtual ~SpitzerHarm();


    // Member Functions

        // Edit

          //- Update electrical conductivity field
          void update();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace mhd
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

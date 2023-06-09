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
    Foam::fv::constantHeatTransfer

Group
    grpFvOptionsSources

Description
    Constant heat transfer model. htcConst [W/m2/K] and area/volume [1/m]
    must be provided.

\*---------------------------------------------------------------------------*/

#ifndef constantHeatTransfer_H
#define constantHeatTransfer_H

#include "interRegionHeatTransferModel.H"
#include "autoPtr.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                    Class constantHeatTransfer Declaration
\*---------------------------------------------------------------------------*/

class constantHeatTransfer
:
    public interRegionHeatTransferModel
{
private:

    // Private data

        //- Constant heat transfer coefficient [W/m2/K]
        autoPtr<volScalarField> htcConst_;

        //- Area per unit volume of heat exchanger [1/m]
        autoPtr<volScalarField> AoV_;


public:

    //- Runtime type information
      TypeName("constantHeatTransfer");


    // Constructors

        //- Construct from dictionary
        constantHeatTransfer
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~constantHeatTransfer();


    // Public Functions

        //- Calculate the heat transfer coefficient
        virtual void calculateHtc();


        // IO

            //- Read dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    dissociationExchangeQK

Description


SourceFiles
    dissociationExchangeQK.C


\*---------------------------------------------------------------------------*/

#ifndef dissociationExchangeQK_H
#define dissociationExchangeQK_H

#include "dissociationQK.H"
#include "exchangeQK.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class dissociationExchangeQK Declaration
\*---------------------------------------------------------------------------*/

class dissociationExchangeQK
:
   public dissociationQK,
   public exchangeQK
{

protected:

    // Protected data

        //- Sub-dictionary containing the reaction properties
        //dictionary propsDict_;


    // Protected Member Functions

        virtual void setProperties();


public:

    //- Runtime type information
    TypeName("dissociationExchangeQK");

    // Constructors

        //- Construct from components
        dissociationExchangeQK
        (
            Time& t,
            dsmcCloud& cloud,
            const dictionary& dict
        );


    // Destructor

        ~dissociationExchangeQK();


    // Member Functions

        //- Initial configuration
        void initialConfiguration();

        //- Apply a reaction between parcel p and q
        void reaction
        (
            dsmcParcel& p,
            dsmcParcel& q,
            const DynamicList<label>& candidateList,
            const List<DynamicList<label> >& candidateSubList,
            const label& candidateP,
            const List<label>& whichSubCell
        );

        void reaction(dsmcParcel& p, dsmcParcel& q);

        //- Return true if both particles belong to this reaction model
        virtual bool tryReactMolecules
        (
            const label& typeIdP,
            const label& typeIdQ
        );


      // Access

        virtual label nReactionsPerTimeStep() const;

        virtual void outputResults(const label& counterIndex);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

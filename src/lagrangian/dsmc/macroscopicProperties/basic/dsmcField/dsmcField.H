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
    dsmcField

Description

SourceFiles
    dsmcFieldI.H
    dsmcField.C
    dsmcFieldIO.C

\*---------------------------------------------------------------------------*/

#ifndef dsmcField_H
#define dsmcField_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "vector.H"
#include "volFields.H"

#include "Random.H"
#include "dsmcParcel.H"
#include "timeDataMeas.H"
#include "writeTimeData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class dsmcField Declaration
\*---------------------------------------------------------------------------*/

class dsmcField
{

protected:

    // Protected data

        const fvMesh& mesh_;

        dsmcCloud& cloud_;

        //- subDictionary containing the time properties
        dictionary timeDict_;

        timeDataMeas time_;

        fileName casePath_;

        fileName timePath_;


    // Private Member Functions

public:

    //- Runtime type information
    TypeName("dsmcField");

    // Declare runtime constructor selection table
        declareRunTimeSelectionTable
        (
            autoPtr,
            dsmcField,
            dictionary,
            (
                Time& t,
                const polyMesh& mesh,
                dsmcCloud& cloud,
                const dictionary& dict
            ),
            (t, mesh, cloud, dict)
        );

    // Constructors

        //- Construct from components
        dsmcField
        (
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud,
            const dictionary& dict
        );


    // Selectors

        static autoPtr<dsmcField> New
        (
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud,
            const dictionary& dict
        );


    // Destructor

        virtual ~dsmcField();


    // Member Functions


        void updateTime();

        const fileName& casePath() const;
        fileName& casePath();

        const fileName& timePath() const;
        fileName& timePath();

        //- create initial field
        virtual void createField() = 0;

        //- calculate field
        virtual void calculateField() = 0;

        //reset field if mesh has changed
        virtual void resetField() = 0;

        //- write field
        virtual void writeField() = 0;

        void updateBasicFieldProperties(const dictionary&);

        virtual void updateProperties(const dictionary&) = 0;

        virtual scalar translationalT(const label cellI) = 0;

        virtual scalar overallT(const label cellI) = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    pdCyclicBoundary

Description

    Used for applying instantaneous actions on molecules colliding with a cyclic
    boundary.

SourceFiles
    pdCyclicBoundaryI.H
    pdCyclicBoundary.C
    pdCyclicBoundaryIO.C

\*---------------------------------------------------------------------------*/

#ifndef pdCyclicBoundary_H
#define pdCyclicBoundary_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "vector.H"
#include "volFields.H"
#include "polyMesh.H"
#include "pdParcel.H"
#include "Time.H"
#include "Random.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class pdCyclicBoundary Declaration
\*---------------------------------------------------------------------------*/

class pdCyclicBoundary
{

protected:

    // Protected data

        const fvMesh& mesh_;

        pdCloud& cloud_;

        Time& time_;

//         Random& rndGen_;

        //- subDictionary containing the properties
        dictionary boundaryDict_;

        //- control on cyclic patch
        word patchName_;
        label patchId_;

        //- corresponding/neighbouring coupled patch
        word neighbPatchName_;
        label neighbPatchId_;


        //- faces on the pdCyclicBoundary
        labelList faces_;

        labelList coupledFacesA_;
        labelList coupledFacesB_;

        //- cells corresponding to the pdCyclicBoundary faces
//         labelList cells_;

        labelList cellsA_;
        labelList cellsB_;

        //- set all the properties below from model if required

            scalarField densities_;
            vectorField velocities_;
            scalarField temperatures_;
            /**************************************************************/
            //scalarField charge_;
            /**************************************************************/
            tensor strainRate_;

            //- writing out properties
            bool writeInTimeDir_;
            bool writeInCase_;


    // Private Member Functions

        void getCoupledFaces(const polyMesh& mesh);

public:

    //- Runtime type information
    TypeName("pdCyclicBoundary");

    // Declare runtime constructor selection table
        declareRunTimeSelectionTable
        (
            autoPtr,
            pdCyclicBoundary,
            dictionary,
            (
                Time& t,
                const polyMesh& mesh,
                pdCloud& cloud,
                const dictionary& dict
            ),
            (t, mesh, cloud, dict)
        );

    // Constructors

        //- Construct from components
        pdCyclicBoundary
        (
            Time& t,
            const polyMesh& mesh,
            pdCloud& cloud,
            const dictionary& dict
        );


    // Selectors

        static autoPtr<pdCyclicBoundary> New
        (
            Time& t,
            const polyMesh& mesh,
            pdCloud& cloud,
            const dictionary& dict
        );


    // Destructor

        virtual ~pdCyclicBoundary();


    // Member Functions

        virtual void calculateProperties() = 0;

        //- create an initial configuration
        virtual void initialConfiguration() = 0;

        //- control the molecule
        virtual void controlMol
        (
            pdParcel& mol,
            pdParcel::trackingData& td
        ) = 0;

        //- output data
        virtual void output
        (
            const fileName& fixedPathName,
            const fileName& timePath
        ) = 0;

        //- update properties from a modified dictionary
        virtual void updateProperties(const dictionary&) = 0;

        void updateBoundaryProperties
        (
            const dictionary& newDict
        );

        //- scalar field
        void writeTimeData
        (
            const fileName& pathName,
            const word& nameFile,
            const scalarField& xData,
            const scalarField& yData
        );

        //- vector field
        void writeTimeData
        (
            const fileName& pathName,
            const word& nameFile,
            const scalarField& xData,
            const vectorField& yData
        );

        // Access

            //- retrun faces on patch
            const labelList& controlPatch() const;


            //- return the adjacent cells
            const labelList& controlZone() const;

            const labelList& allFaces() const;

            //- return the name of the control patch
            const word& patchName() const;
            const label& patchId() const;

            //- return the targeted fields
            const scalarField& densityField() const;
            scalarField& densityField();

            const vectorField& velocityField() const;
            vectorField& velocityField();

            const scalarField& temperatureField() const;
            scalarField& temperatureField();

            /**************************************************************/
            //const scalarField& chargeField() const;
            //scalarField& chargeField();
            /**************************************************************/

            const tensor& strainRate() const;
            tensor& strainRate();

            const bool& writeInTimeDir() const;
            const bool& writeInCase() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    dsmcCoordinateSystem

Description

SourceFiles
    dsmcCoordinateSystemI.H
    dsmcCoordinateSystem.C

\*---------------------------------------------------------------------------*/

#ifndef dsmcCoordinateSystem_H
#define dsmcCoordinateSystem_H

#include "fvMesh.H"
#include "dsmcParcel.H"
#include "dsmcTimeStepModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class dsmcCoordinateSystem Declaration
\*---------------------------------------------------------------------------*/


class dsmcCoordinateSystem
{

protected:

    // Protected data

        //- Const reference to the mesh
        const fvMesh& mesh_;

        //- Number of real particles represented by a DSMC parcel as given by
        //  the constant/dsmcProperties dictionary
        scalar nParticlesOrg_;

        //- Time-step model: constant, variable time-step method
        autoPtr<dsmcTimeStepModel> timeStepModel_;



private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        dsmcCoordinateSystem(const dsmcCoordinateSystem&);

        //- Disallow default bitwise assignment
        void operator=(const dsmcCoordinateSystem&);



public:

    friend class dsmcParcel;

    //- Runtime type information
    TypeName("dsmcCoordinateSystem");


    // Declare run-time constructor selection table
    #ifndef SWIG
    declareRunTimeSelectionTable
    (
        autoPtr,
        dsmcCoordinateSystem,
        fvMesh,
        (
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud
        ),
        (t, mesh, cloud)
    );
    #endif


    // Constructors

        //- Constructor
        dsmcCoordinateSystem
        (
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud
        );


    // Selectors

        //- Return a reference to the selected coordinate system
        static autoPtr<dsmcCoordinateSystem> New
        (
            Time& t,
            const polyMesh& mesh,
            dsmcCloud& cloud
        );


    // Destructor

        virtual ~dsmcCoordinateSystem();


    // Member Functions

      // Access

        //- Return const access to the time-step value given in the
        //  controlDict dictionary
        inline scalar deltaTValueOrg() const;

        //- Return const reference to time-step model
        inline const dsmcTimeStepModel& dtModel() const;

        //- Return non-const reference to time-step model
        inline dsmcTimeStepModel& dtModel();

        //- Return non-const access to the time-step for a given cell
        inline scalar deltaTValue(const label celli) const;

        //- Return const access to the number of real particles represented by
        //  a DSMC parcel as given by the constant/dsmcProperties dictionary
        inline scalar nParticlesOrg() const;

        //- Return const access to the number of real particles represented by
        //  a DSMC parcel
        virtual inline const volScalarField& nParticles() const;

        //- Return const access to the number of real particles represented by
        //  a DSMC parcel for a given cell
        virtual inline scalar nParticles(const label celli) const;

        //- Return const access to the number of real particles represented by
        //  a parcel for a given patch/face couple
        virtual inline scalar nParticles
        (
            const label patchi,
            const label facei
        ) const;

        //- Return the local RWF for a given cell
        virtual inline scalar RWF(const label celli) const;

        //- Return the local RWF for a given face
        virtual inline scalar pRWF
        (
            const label patchi,
            const label facei
        ) const;


      // Check

        //- Check the validity of the coordinate system inputs present in
        //  constant/dsmcProperties and assign inputs to data members if
        //  correct. Pass init as true if used in the constructor of
        //  the dsmcInitialise application
        virtual void checkCoordinateSystemInputs(const bool init = false) = 0;


      // Edit

        //- Evolve function
        virtual void evolve() = 0;


      // Write

        //- Write coordinate system info
        virtual void writeCoordinateSystemInfo() const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "dsmcCoordinateSystemI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

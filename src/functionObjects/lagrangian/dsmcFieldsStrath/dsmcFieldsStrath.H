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
    Foam::dsmcFieldsStrath

Description
    Calculate intensive fields:
    - UMean
    - translationalT
    - rotationalT
    from averaged extensive fields from a DSMC calculation.

SourceFiles
    dsmcFieldsStrath.C
    IOdsmcFieldsStrath.H

\*---------------------------------------------------------------------------*/

#ifndef dsmcFieldsStrath_H
#define dsmcFieldsStrath_H

#include "pointFieldFwd.H"
#include "typeInfo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                           Class dsmcFieldsStrath Declaration
\*---------------------------------------------------------------------------*/

class dsmcFieldsStrath
{
    // Private data

        //- Name of this set of dsmcFieldsStrath objects
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        dsmcFieldsStrath(const dsmcFieldsStrath&);

        //- Disallow default bitwise assignment
        void operator=(const dsmcFieldsStrath&);


public:

    //- Runtime type information
    TypeName("dsmcFieldsStrath");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        dsmcFieldsStrath
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~dsmcFieldsStrath();


    // Member Functions

        //- Return name of the set of dsmcFieldsStrath
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the dsmcFieldsStrath data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the dsmcFieldsStrath and write
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh
        virtual void movePoints(const polyMesh&)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

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
    Foam::functionObjects::wallShearStress

Group
    grpForcesFunctionObjects

Description
    Calculates and write the shear-stress at wall patches as
    the volVectorField field 'wallShearStress'.

        \f[
            Stress = R \dot n
        \f]

    where
    \vartable
        R       | stress tensor
        n       | patch normal vector (into the domain)
    \endvartable

    The shear-stress symmetric tensor field is retrieved from the turbulence
    model.  All wall patches are included by default; to restrict the
    calculation to certain patches, use the optional 'patches' entry.

Usage
    Example of function object specification:
    \verbatim
    wallShearStress1
    {
        type        wallShearStress;
        libs        ("libstrathFieldFunctionObjects.so");
        ...
        patches     (".*Wall");
    }
    \endverbatim

    Where the entries comprise:
    \table
        Property | Description                | Required   | Default value
        type     | type name: wallShearStress | yes        |
        resultName | Name of wall shear stress field | no  | \<function name\>
        patches  | list of patches to process | no         | all wall patches
        log      | Log to standard output     | no         | yes
    \endtable

See also
    Foam::functionObject
    Foam::functionObjects::fvMeshFunctionObject
    Foam::functionObjects::writeFile
    Foam::functionObjects::timeControl

SourceFiles
    wallShearStress.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_wallShearStress_H
#define functionObjects_wallShearStress_H

#include "fvMeshFunctionObject.H"
#include "writeFile.H"
#include "volFieldsFwd.H"
#include "HashSet.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class wallShearStress Declaration
\*---------------------------------------------------------------------------*/

class wallShearStress
:
    public fvMeshFunctionObject,
    public writeFile
{

protected:

    // Protected data

        //- Optional list of patches to process
        labelHashSet patchSet_;


    // Protected Member Functions

        //- File header information
        virtual void writeFileHeader(Ostream& os) const;

        //- Calculate the shear-stress
        void calcShearStress
        (
            const volSymmTensorField& Reff,
            volVectorField& shearStress
        );


private:

    // Private member functions

        //- Disallow default bitwise copy construct
        wallShearStress(const wallShearStress&);

        //- Disallow default bitwise assignment
        void operator=(const wallShearStress&);


public:

    //- Runtime type information
    TypeName("wallShearStress");


    // Constructors

        //- Construct from Time and dictionary
        wallShearStress
        (
            const word& name,
            const Time& runTime,
            const dictionary&
        );


    //- Destructor
    virtual ~wallShearStress();


    // Member Functions

        //- Read the wallShearStress data
        virtual bool read(const dictionary&);

        //- Calculate the wall shear-stress
        virtual bool execute();

        //- Write the wall shear-stress
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

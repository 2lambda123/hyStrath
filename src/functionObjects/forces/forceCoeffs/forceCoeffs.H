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
    Foam::functionObjects::forceCoeffs

Group
    grpForcesFunctionObjects

Description
    Extends the forces functionObject by providing lift, drag and moment
    coefficients.  The data can optionally be output into bins, defined in a
    given direction.

    The binned data provides the total and consitituent components per bin:
    - total coefficient
    - pressure coefficient contribution
    - viscous coefficient contribution
    - porous coefficient contribution

    Data is written into multiple files in the
    postProcessing/\<functionObjectName\> directory:
    - coefficient.dat   : integrated coefficients over all geometries
    - CmBin.dat         : moment coefficient bins
    - CdBin.dat         : drag coefficient bins
    - ClBin.dat         : lift coefficient bins

Usage
    Example of function object specification:
    \verbatim
    forceCoeffs1
    {
        type        forceCoeffs;
        libs        ("libstrathforces.so");
        ...
        log         yes;
        writeFields yes;
        patches     (walls);
        liftDir     (0 1 0);
        dragDir     (-1 0 0);
        pitchAxis   (0 0 1);
        magUInf     100;
        lRef        3.5;
        Aref        2.2;

        binData
        {
            nBin        20;
            direction   (1 0 0);
            cumulative  yes;
        }
    }
    \endverbatim

    Where the entries comprise:
    \table
        Property     | Description             | Required    | Default value
        type         | Type name: forces       | yes         |
        log          | Write force data to standard output | no | no
        writeFields  | Write the force and moment coefficient fields | no | no
        patches      | Patches included in the forces calculation | yes |
        liftDir      | Lift direction          | yes         |
        dragDir      | Drag direction          | yes         |
        pitchAxis    | Picth axis              | yes         |
        magUInf      | Free stream velocity magnitude | yes  |
        lRef         | Reference length scale for moment calculations | yes |
        Aref         | Reference area          | yes |
        porosity     | Flag to include porosity contributions | no | no
    \endtable

    Bin data is optional, but if the dictionary is present, the entries must
    be defined according o
    \table
        nBin         | number of data bins     | yes         |
        direction    | direction along which bins are defined | yes |
        cumulative   | bin data accumulated with incresing distance | yes |
    \endtable

See also
    Foam::functionObject
    Foam::functionObjects::timeControl
    Foam::functionObjects::forces

SourceFiles
    forceCoeffs.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_forceCoeffs_H
#define functionObjects_forceCoeffs_H

#include "forces.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class forceCoeffs Declaration
\*---------------------------------------------------------------------------*/

class forceCoeffs
:
    public forces
{
    // Private data

        // Force coefficient geometry

            //- Lift
            vector liftDir_;

            //- Drag
            vector  dragDir_;

            //- Pitch
            vector pitchAxis_;


        // Free-stream conditions

            //- Velocity magnitude
            scalar magUInf_;


        // Reference scales

            //- Length
            scalar lRef_;

            //- Area
            scalar Aref_;


        // File streams

            //- Integrated coefficients
            autoPtr<OFstream> coeffFilePtr_;

            //- Moment coefficient
            autoPtr<OFstream> CmBinFilePtr_;

            //- Drag coefficient
            autoPtr<OFstream> CdBinFilePtr_;

            //- Lift coefficient
            autoPtr<OFstream> ClBinFilePtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        forceCoeffs(const forceCoeffs&);

        //- Disallow default bitwise assignment
        void operator=(const forceCoeffs&);


protected:

    // Protected Member Functions

        //- Create the output files
        void createFiles();

        //- Write header for integrated data
        void writeIntegratedHeader(const word& header, Ostream& os) const;

        //- Write header for binned data
        void writeBinHeader(const word& header, Ostream& os) const;

        //- Write integrated data
        void writeIntegratedData
        (
            const word& title,
            const List<Field<scalar>>& coeff
        ) const;

        //- Write binned data
        void writeBinData(const List<Field<scalar>> coeffs, Ostream& os) const;


public:

    //- Runtime type information
    TypeName("forceCoeffs");


    // Constructors

        //- Construct from Time and dictionary
        forceCoeffs
        (
            const word& name,
            const Time& runTime,
            const dictionary&
        );


    //- Destructor
    virtual ~forceCoeffs();


    // Member Functions

        //- Read the forces data
        virtual bool read(const dictionary&);

        //- Execute, currently does nothing
        virtual bool execute();

        //- Write the forces
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

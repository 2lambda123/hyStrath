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
    Foam::AveragingMethods::Basic

Description
    Basic lagrangian averaging procedure.

    This is a cell-volume based average. Point values are summed over the
    computational cells and the result is divided by the cell volume.

    Interpolation is done assuming a constant value over each cells. Cell
    gradients are calculated by the default fvc::grad scheme, and are also
    assumed constant when interpolated.

SourceFiles
    Basic.C

\*---------------------------------------------------------------------------*/

#ifndef Basic_H
#define Basic_H

#include "AveragingMethod.H"

#include "fvCFD.H"
#include "pointPatchField.H"

#include "pointMesh.H"
#include "tetIndices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace AveragingMethods
{

/*---------------------------------------------------------------------------*\
                        Class Basic Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Basic
:
    public AveragingMethod<Type>
{
    public:

        //- Public typedefs


            //- Gradient type
            typedef typename AveragingMethod<Type>::TypeGrad TypeGrad;


    private:

        //- Private data

            //- Cell average field
            Field<Type>& data_;

            //- Gradient field
            Field<TypeGrad> dataGrad_;

        //- Private member functions

            //- Re-calculate gradient
            virtual void updateGrad();


    public:



        //- Runtime type information
        TypeName("Basic");


        //- Constructors

            //- Construct from components
            Basic
            (
                const IOobject& io,
                const dictionary& dict,
                const fvMesh &mesh
            );

            //- Construct a copy
            Basic(const Basic<Type>& am);

            //- Construct and return a clone
            virtual autoPtr<AveragingMethod<Type> > clone() const
            {
                return autoPtr<AveragingMethod<Type> >
                (
                    new Basic<Type>(*this)
                );
            }


        //- Destructor
        virtual ~Basic();


        //- Member Functions

            void resetFields();

            //- Add point value to interpolation
            void add
            (
                const point position,
                const tetIndices& tetIs,
                const Type& value
            );

            //- Output stencil for point
            List<List<scalar> > stencil
            (
                const point position,
                const tetIndices& tetIs
            );

            //- Interpolate
            Type interpolate
            (
                const point position,
                const tetIndices& tetIs
            );

            //- Interpolate gradient
            TypeGrad interpolateGrad
            (
                const point position,
                const tetIndices& tetIs
            ) const;

            //- Return an internal field of the average
            //tmp<Field<Type> > internalField() const;

            tmp<Field<Type> > internalField() const;

            //- Return an internal field of the gradient
            tmp<Field<TypeGrad> > internalFieldGrad() const;

            void updateField
            (
               GeometricField<Type, fvPatchField, volMesh> cField
            );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace AveragingMethods
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "Basic.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

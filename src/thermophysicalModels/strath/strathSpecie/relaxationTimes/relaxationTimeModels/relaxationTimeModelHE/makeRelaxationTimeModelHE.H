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

InClass
    Foam::basicPsiThermo

Description
    Macros for creating 'basic' heavy-particle - free electrons relaxation
    source term in the vibrational equation

\*---------------------------------------------------------------------------*/

#ifndef makeRelaxationTimeModelHE_H
#define makeRelaxationTimeModelHE_H

#include "addToRunTimeSelectionTable.H"
#include "relaxationTimeModelHE.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeRelaxationTimeModelHE(RelaxationTimeModelHE,Thermo)               \
                                                                              \
typedef RelaxationTimeModelHE<Thermo>                                         \
    RelaxationTimeModelHE##Thermo;                                            \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    RelaxationTimeModelHE##Thermo,                                            \
    #RelaxationTimeModelHE                                                    \
        "<"#Thermo">",                                                        \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    relaxationTimeModelHE,                                                    \
    RelaxationTimeModelHE##Thermo,                                            \
    fvMesh                                                                    \
)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

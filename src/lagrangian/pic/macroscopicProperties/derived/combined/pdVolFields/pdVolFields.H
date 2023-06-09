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
    pdVolFields

Description
    Geometric field for temperature, density, velocity, pressure.



SourceFiles
    pdVolFieldsI.H
    pdVolFields.C
    pdVolFieldsIO.C

\*---------------------------------------------------------------------------*/

#ifndef pdVolFields_H
#define pdVolFields_H

#include "pdField.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "pdCloud.H"

#include "autoPtr.H"
#include "polyMeshTetDecomposition.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class pdVolFields Declaration
\*---------------------------------------------------------------------------*/

class pdVolFields
:
	public pdField
{

private:

    // Private data
        dictionary propsDict_;

        scalar mfpReferenceTemperature_;
        word fieldName_;

        //- pointer to averaging method, scalar only
        autoPtr<AveragingMethod<scalar> > AveragePtr_;

        volScalarField pdRhoN_;
        volScalarField rhoN_;
        volScalarField rhoQ_;
        volScalarField rhoM_;
        volScalarField p_;
        volScalarField translationalT_;
        volScalarField rotationalT_;
        volScalarField vibrationalT_;
        volScalarField overallT_;
        volScalarField q_;
        volScalarField tau_;
        volScalarField meanFreePath_;
        volScalarField mfpCellRatio_;
        volScalarField meanCollisionRate_;
        volScalarField meanCollisionTime_;
        volScalarField meanCollisionTimeTimeStepRatio_;
        volScalarField Ma_;
        volScalarField classIDistribution_;
        volScalarField classIIDistribution_;
        volScalarField classIIIDistribution_;
        volScalarField densityError_;
        volScalarField velocityError_;
        volScalarField temperatureError_;
        volScalarField pressureError_;
        volScalarField phiE_;
        volScalarField phiEMean_;
        volVectorField E_;
        volVectorField EMean_;
        volVectorField U_;
        volVectorField fD_;
        volVectorField fEM_;
        volVectorField Jp_;
        volVectorField heatFluxVector_;
        volTensorField maxwellTensor_;
        volTensorField pressureTensor_;
        volTensorField shearStressTensor_;

        scalar nTimeSteps_;
        labelList typeIds_;
        scalarField rhoNMean_;
        scalarField rhoQMean_;
        scalarField rhoNMeanInt_;
        scalarField rhoMMean_;
        scalarField linearKEMean_;
        scalarField rotationalEMean_;
        scalarField rotationalDofMean_;
        //vectorField EMean_;
        vectorField JpMean_;
        scalarField muu_;
        scalarField muv_;
        scalarField muw_;
        scalarField mvv_;
        scalarField mvw_;
        scalarField mww_;
        scalarField mcc_;
        scalarField mccu_;
        scalarField mccv_;
        scalarField mccw_;
        scalarField eu_;
        scalarField ev_;
        scalarField ew_;
        scalarField e_;
        scalarField totalvDof_;
        scalarField nClassI_;
        scalarField nClassII_;
        scalarField nClassIII_;
        vectorField momentumMean_;
        List<labelList> boundaryCells_;
        List<scalarField> vibrationalETotal_;
        List<scalarField> nParcels_;
        List<scalarField> vibT_;

        // effective degrees of freedom
        List<scalarField> vDof_;
        List<scalarField> mfp_; // mean free path
        List<scalarField> mcr_; // mean collision rate


        // boundary measurements

        //List<scalarField> phiEBF_;
        List<scalarField> rhoNBF_;
        List<scalarField> rhoQBF_;
        List<scalarField> rhoMBF_;
        List<scalarField> linearKEBF_;
        List<scalarField> rotationalEBF_;
        List<scalarField> rotationalDofBF_;
        List<scalarField> qBF_;
        List<scalarField> vibTxvDofBF_;
        List<scalarField> totalvDofBF_;
        List<scalarField> speciesRhoNIntBF_;
        List<vectorField> momentumBF_;
        List<vectorField> EBF_;
        List<vectorField> fEMBF_;
        List<vectorField> fDBF_;
        List<vectorField> JpBF_;

        List< List < scalarField > > vibrationalEBF_;
        List< List < scalarField > > speciesRhoNBF_;
        List< List < scalarField > > vibTBF_;
        List< List < scalarField > > vDofBF_;

        bool averagingAcrossManyRuns_;
        bool measureClassifications_;
        bool measureMeanFreePath_;
        bool measureErrors_;


    // Private Member Functions

        void readIn();

        void writeOut();


public:

    //- Runtime type information
    TypeName("pdVolFields");

    // Constructors

        //- Construct from components
    pdVolFields
    (
        Time& t,
        const polyMesh& mesh,
        pdCloud& cloud,
        const dictionary& dict
    );

    // Destructor

    ~pdVolFields();


    // Member Functions

    //- create initial field
    void createField();

    //- calculate field
    void calculateField();

    //- write field
    void writeField();

//         const propertyField& fields() const;
    void updateProperties(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //


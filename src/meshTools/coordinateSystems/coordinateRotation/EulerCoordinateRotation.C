/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2022 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

\*---------------------------------------------------------------------------*/

#include "EulerCoordinateRotation.H"

#include "mathematicalConstants.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(EulerCoordinateRotation, 0);
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        EulerCoordinateRotation,
        dictionary
    );
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        EulerCoordinateRotation,
        points
    );
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::EulerCoordinateRotation::calcTransform
(
    const scalar phiAngle,
    const scalar thetaAngle,
    const scalar psiAngle,
    const bool inDegrees
)
{
    scalar phi   = phiAngle;
    scalar theta = thetaAngle;
    scalar psi   = psiAngle;

    if (inDegrees)
    {
        phi   *= constant::mathematical::pi/180.0;
        theta *= constant::mathematical::pi/180.0;
        psi   *= constant::mathematical::pi/180.0;
    }

    R_ =
    (
        tensor
        (
            cos(phi)*cos(psi) - sin(phi)*sin(psi)*cos(theta),
            -sin(phi)*cos(psi)*cos(theta) - cos(phi)*sin(psi),
            sin(phi)*sin(theta),

            cos(phi)*sin(psi)*cos(theta) + sin(phi)*cos(psi),
            cos(phi)*cos(psi)*cos(theta) - sin(phi)*sin(psi),
            -cos(phi)*sin(theta),

            sin(psi)*sin(theta),
            cos(psi)*sin(theta),
            cos(theta)
        )
    );

    Rtr_ = R_.T();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::EulerCoordinateRotation::EulerCoordinateRotation
(
    const dictionary& dict
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    vector rotation(dict.lookup("rotation"));

    calcTransform
    (
        rotation.component(vector::X),
        rotation.component(vector::Y),
        rotation.component(vector::Z),
        dict.lookupOrDefault("degrees", true)
    );
}


Foam::EulerCoordinateRotation::EulerCoordinateRotation
(
    const dictionary& dict,
    const UList<vector>& points
)
:
    EulerCoordinateRotation(dict)
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::vector Foam::EulerCoordinateRotation::transform(const vector& v) const
{
    return (R_ & v);
}


Foam::vector Foam::EulerCoordinateRotation::invTransform(const vector& v) const
{
    return (Rtr_ & v);
}


Foam::tmp<Foam::vectorField> Foam::EulerCoordinateRotation::transform
(
    const vectorField& vf
) const
{
    NotImplemented;
    return tmp<vectorField>(nullptr);
}


Foam::tmp<Foam::vectorField> Foam::EulerCoordinateRotation::invTransform
(
    const vectorField& vf
) const
{
    NotImplemented;
    return tmp<vectorField>(nullptr);
}


Foam::tmp<Foam::tensorField> Foam::EulerCoordinateRotation::transform
(
    const tensorField& volSymmTensorField
) const
{
     NotImplemented;
    return tmp<tensorField>(nullptr);
}


Foam::tensor Foam::EulerCoordinateRotation::transform
(
    const vector& p,
    const tensor& t
) const
{
    return (R_ & t & Rtr_);
}


Foam::tmp<Foam::symmTensorField> Foam::EulerCoordinateRotation::
transformDiagTensor
(
    const vectorField& vf
) const
{
    tmp<symmTensorField> tfld(new symmTensorField(vf.size()));
    symmTensorField& fld = tfld.ref();

    forAll(fld, i)
    {
        fld[i] = transformVectorDiagTensor(R_, vf[i]);
    }
    return tfld;
}


Foam::symmTensor Foam::EulerCoordinateRotation::transformDiagTensor
(
    const vector& p,
    const vector& v
) const
{
    return transformVectorDiagTensor(R_, v);
}


void Foam::EulerCoordinateRotation::write(Ostream& os) const
{
     writeEntry(os, "e1", e1());
     writeEntry(os, "e2", e2());
     writeEntry(os, "e3", e3());
}


// ************************************************************************* //

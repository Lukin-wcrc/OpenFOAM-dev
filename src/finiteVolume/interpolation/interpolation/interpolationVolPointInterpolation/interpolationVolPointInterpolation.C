/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2022 OpenFOAM Foundation
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

#include "interpolationVolPointInterpolation.H"
#include "volPointInterpolation.H"

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
Foam::interpolationVolPointInterpolation<Type>::
interpolationVolPointInterpolation
(
    const VolField<Type>& psi
)
:
    psip_
    (
        volPointInterpolation::New(psi.mesh()).interpolate
        (
            psi,
            "volPointInterpolate(" + psi.name() + ')',
            true        // use cache
        )
    )
{}


template<class Type>
Foam::interpolationVolPointInterpolation<Type>::
interpolationVolPointInterpolation
(
    const interpolationVolPointInterpolation<Type>& i
)
:
    psip_(i.psip_.clone())
{}


template<class Type>
Foam::interpolationVolPointInterpolation<Type>::
interpolationVolPointInterpolation
(
    const VolField<Type>& psi,
    tmp<PointField<Type>> psip
)
:
    psip_(psip)
{}


// ************************************************************************* //

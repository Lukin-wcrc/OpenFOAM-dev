/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2023 OpenFOAM Foundation
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

#include "psiuMulticomponentThermo.H"
#include "FieldListSlice.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool Foam::psiuMulticomponentThermo::containsSpecie
(
    const word& specieName
) const
{
    return species().found(specieName);
}


inline Foam::volScalarField& Foam::psiuMulticomponentThermo::Y
(
    const word& specieName
)
{
    return Y()[species()[specieName]];
}


inline const Foam::volScalarField& Foam::psiuMulticomponentThermo::Y
(
    const word& specieName
) const
{
    return Y()[species()[specieName]];
}


inline Foam::scalarFieldListSlice
Foam::psiuMulticomponentThermo::implementation::cellComposition
(
    const label celli
) const
{
    return Yslicer_.slice(celli);
}


inline Foam::scalarFieldListSlice
Foam::psiuMulticomponentThermo::implementation::patchFaceComposition
(
    const label patchi,
    const label facei
) const
{
    return Yslicer_.patchSlice(patchi, facei);
}


// ************************************************************************* //

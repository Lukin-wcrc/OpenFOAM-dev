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

template<class ParcelType>
template<class TrackCloudType>
inline Foam::MomentumParcel<ParcelType>::trackingData::trackingData
(
    const TrackCloudType& cloud
)
:
    ParcelType::trackingData(cloud),
    rhoInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.rho()
        )
    ),
    UInterp_
    (
        interpolation<vector>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.U()
        )
    ),
    muInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.mu()
        )
    ),
    rhoc_(Zero),
    Uc_(Zero),
    muc_(Zero),
    g_(cloud.g().value()),
    trackTime_(cloud.solution().trackTime()),
    stepFractionRange_(0, 1)
{}


template<class ParcelType>
inline const Foam::interpolation<Foam::scalar>&
Foam::MomentumParcel<ParcelType>::trackingData::rhoInterp() const
{
    return rhoInterp_();
}


template<class ParcelType>
inline const Foam::interpolation<Foam::vector>&
Foam::MomentumParcel<ParcelType>::trackingData::UInterp() const
{
    return UInterp_();
}


template<class ParcelType>
inline const Foam::interpolation<Foam::scalar>&
Foam::MomentumParcel<ParcelType>::trackingData::muInterp() const
{
    return muInterp_();
}


template<class ParcelType>
inline Foam::scalar
Foam::MomentumParcel<ParcelType>::trackingData::rhoc() const
{
    return rhoc_;
}


template<class ParcelType>
inline Foam::scalar& Foam::MomentumParcel<ParcelType>::trackingData::rhoc()
{
    return rhoc_;
}


template<class ParcelType>
inline const Foam::vector&
Foam::MomentumParcel<ParcelType>::trackingData::Uc() const
{
    return Uc_;
}


template<class ParcelType>
inline Foam::vector& Foam::MomentumParcel<ParcelType>::trackingData::Uc()
{
    return Uc_;
}


template<class ParcelType>
inline Foam::scalar Foam::MomentumParcel<ParcelType>::trackingData::muc() const
{
    return muc_;
}


template<class ParcelType>
inline Foam::scalar& Foam::MomentumParcel<ParcelType>::trackingData::muc()
{
    return muc_;
}


template<class ParcelType>
inline const Foam::vector&
Foam::MomentumParcel<ParcelType>::trackingData::g() const
{
    return g_;
}


template<class ParcelType>
inline Foam::scalar
Foam::MomentumParcel<ParcelType>::trackingData::trackTime() const
{
    return trackTime_;
}


template<class ParcelType>
inline Foam::scalar&
Foam::MomentumParcel<ParcelType>::trackingData::trackTime()
{
    return trackTime_;
}


template<class ParcelType>
inline Foam::Pair<Foam::scalar>
Foam::MomentumParcel<ParcelType>::trackingData::stepFractionRange() const
{
    return stepFractionRange_;
}


template<class ParcelType>
inline Foam::Pair<Foam::scalar>&
Foam::MomentumParcel<ParcelType>::trackingData::stepFractionRange()
{
    return stepFractionRange_;
}


// ************************************************************************* //

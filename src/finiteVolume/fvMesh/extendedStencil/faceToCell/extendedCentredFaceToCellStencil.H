/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2023 OpenFOAM Foundation
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

Class
    Foam::extendedCentredFaceToCellStencil

Description

SourceFiles
    extendedCentredFaceToCellStencil.C

\*---------------------------------------------------------------------------*/

#ifndef extendedCentredFaceToCellStencil_H
#define extendedCentredFaceToCellStencil_H

#include "extendedFaceToCellStencil.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class faceToCellStencil;

/*---------------------------------------------------------------------------*\
              Class extendedCentredFaceToCellStencil Declaration
\*---------------------------------------------------------------------------*/

class extendedCentredFaceToCellStencil
:
    public extendedFaceToCellStencil
{
    // Private Data

        //- Swap map for getting neighbouring data
        autoPtr<distributionMap> mapPtr_;

        //- Per face the stencil.
        labelListList stencil_;


public:

    // Constructors

        //- Construct from uncompacted face stencil
        explicit extendedCentredFaceToCellStencil(const faceToCellStencil&);

        //- Disallow default bitwise copy construction
        extendedCentredFaceToCellStencil
        (
            const extendedCentredFaceToCellStencil&
        ) = delete;


    // Member Functions

        //- Return reference to the parallel distribution map
        const distributionMap& map() const
        {
            return mapPtr_();
        }

        //- Return reference to the stencil
        const labelListList& stencil() const
        {
            return stencil_;
        }

        //- After removing elements from the stencil adapt the schedule (map).
        void compact();

        //- Use map to get the data into stencil order
        template<class Type>
        void collectData
        (
            const SurfaceField<Type>& fld,
            List<List<Type>>& stencilFld
        ) const
        {
            extendedFaceToCellStencil::collectData
            (
                map(),
                stencil(),
                fld,
                stencilFld
            );
        }

        //- Sum surface field contributions to create cell values
        template<class Type>
        tmp<VolField<Type>> weightedSum
        (
            const SurfaceField<Type>& fld,
            const List<List<scalar>>& stencilWeights
        ) const
        {
            return extendedFaceToCellStencil::weightedSum
            (
                map(),
                stencil(),
                fld,
                stencilWeights
            );
        }


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const extendedCentredFaceToCellStencil&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

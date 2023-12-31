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

#include "edgeMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "addToMemberFunctionSelectionTable.H"
#include "ListOps.H"
#include "EdgeMap.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(edgeMesh, 0);
    defineRunTimeSelectionTable(edgeMesh, fileExtension);
    defineMemberFunctionSelectionTable(edgeMesh,write,fileExtension);
}


Foam::wordHashSet Foam::edgeMesh::readTypes()
{
    return wordHashSet(*fileExtensionConstructorTablePtr_);
}


Foam::wordHashSet Foam::edgeMesh::writeTypes()
{
    return wordHashSet(*writefileExtensionMemberFunctionTablePtr_);
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

bool Foam::edgeMesh::canReadType
(
    const word& ext,
    const bool verbose
)
{
    return checkSupport
    (
        readTypes(),
        ext,
        verbose,
        "reading"
   );
}


bool Foam::edgeMesh::canWriteType
(
    const word& ext,
    const bool verbose
)
{
    return checkSupport
    (
        writeTypes(),
        ext,
        verbose,
        "writing"
    );
}


bool Foam::edgeMesh::canRead
(
    const fileName& name,
    const bool verbose
)
{
    word ext = name.ext();
    if (ext == "gz")
    {
        ext = name.lessExt().ext();
    }
    return canReadType(ext, verbose);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::edgeMesh::calcPointEdges() const
{
    if (pointEdgesPtr_.valid())
    {
        FatalErrorInFunction
            << "pointEdges already calculated." << abort(FatalError);
    }

    pointEdgesPtr_.reset(new labelListList(points_.size()));
    labelListList& pointEdges = pointEdgesPtr_();

    invertManyToMany(pointEdges.size(), edges_, pointEdges);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::edgeMesh::edgeMesh()
:
    fileFormats::edgeMeshFormatsCore(),
    points_(0),
    edges_(0),
    pointEdgesPtr_(nullptr)
{}


Foam::edgeMesh::edgeMesh
(
    const pointField& points,
    const edgeList& edges
)
:
    fileFormats::edgeMeshFormatsCore(),
    points_(points),
    edges_(edges),
    pointEdgesPtr_(nullptr)
{}


Foam::edgeMesh::edgeMesh
(
    pointField&& pointLst,
    edgeList&& edgeLst
)
:
    fileFormats::edgeMeshFormatsCore(),
    points_(move(pointLst)),
    edges_(move(edgeLst)),
    pointEdgesPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::edgeMesh::~edgeMesh()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::edgeMesh::clear()
{
    points_.clear();
    edges_.clear();
    pointEdgesPtr_.clear();
}


void Foam::edgeMesh::reset
(
    pointField&& pointLst,
    edgeList&& edgeLst
)
{
    // Take over new primitive data.
    // Optimised to avoid overwriting data at all
    if (notNull(pointLst))
    {
        points_.transfer(pointLst);
    }

    if (notNull(edgeLst))
    {
        edges_.transfer(edgeLst);

        // connectivity likely changed
        pointEdgesPtr_.clear();
    }
}


void Foam::edgeMesh::transfer(edgeMesh& mesh)
{
    points_.transfer(mesh.points_);
    edges_.transfer(mesh.edges_);
    pointEdgesPtr_ = mesh.pointEdgesPtr_;
}


Foam::label Foam::edgeMesh::regions(labelList& edgeRegion) const
{
    edgeRegion.setSize(edges_.size());
    edgeRegion = -1;

    label startEdgeI = 0;
    label currentRegion = 0;

    while (true)
    {
        while (startEdgeI < edges_.size() && edgeRegion[startEdgeI] != -1)
        {
            startEdgeI++;
        }

        if (startEdgeI == edges_.size())
        {
            break;
        }

        // Found edge that has not yet been assigned a region.
        // Mark connected region with currentRegion starting at startEdgeI.

        edgeRegion[startEdgeI] = currentRegion;
        labelList edgesToVisit(1, startEdgeI);

        while (edgesToVisit.size())
        {
            // neighbours of current edgesToVisit
            DynamicList<label> newEdgesToVisit(edgesToVisit.size());

            // Mark all point connected edges with current region.
            forAll(edgesToVisit, i)
            {
                label edgeI = edgesToVisit[i];

                // Mark connected edges
                const edge& e = edges_[edgeI];

                forAll(e, fp)
                {
                    const labelList& pEdges = pointEdges()[e[fp]];

                    forAll(pEdges, pEdgeI)
                    {
                        label nbrEdgeI = pEdges[pEdgeI];

                        if (edgeRegion[nbrEdgeI] == -1)
                        {
                            edgeRegion[nbrEdgeI] = currentRegion;
                            newEdgesToVisit.append(nbrEdgeI);
                        }
                    }
                }
            }

            edgesToVisit.transfer(newEdgesToVisit);
        }

        currentRegion++;
    }
    return currentRegion;
}


void Foam::edgeMesh::scalePoints(const scalar scaleFactor)
{
    // avoid bad scaling
    if (scaleFactor > 0 && scaleFactor != 1.0)
    {
        points_ *= scaleFactor;
    }
}


void Foam::edgeMesh::mergeEdges()
{
    EdgeMap<label> existingEdges(2*edges_.size());

    label curEdgeI = 0;
    forAll(edges_, edgeI)
    {
        const edge& e = edges_[edgeI];

        if (existingEdges.insert(e, curEdgeI))
        {
            curEdgeI++;
        }
    }

    if (debug)
    {
        Info<< "Merging duplicate edges: "
            << edges_.size() - existingEdges.size()
            << " edges will be deleted." << endl;
    }

    edges_.setSize(existingEdges.size());

    forAllConstIter(EdgeMap<label>, existingEdges, iter)
    {
        edges_[iter()] = iter.key();
    }

    // connectivity changed
    pointEdgesPtr_.clear();
}


// ************************************************************************* //

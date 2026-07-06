#include "osi/core/ShapeStatistics.h"

namespace osi::core {

ShapeStatistics::ShapeStatistics() = default;

int ShapeStatistics::compoundCount() const
{
    return compoundCount_;
}

void ShapeStatistics::setCompoundCount(int count)
{
    compoundCount_ = count;
}

int ShapeStatistics::solidCount() const
{
    return solidCount_;
}

void ShapeStatistics::setSolidCount(int count)
{
    solidCount_ = count;
}

int ShapeStatistics::shellCount() const
{
    return shellCount_;
}

void ShapeStatistics::setShellCount(int count)
{
    shellCount_ = count;
}

int ShapeStatistics::faceCount() const
{
    return faceCount_;
}

void ShapeStatistics::setFaceCount(int count)
{
    faceCount_ = count;
}

int ShapeStatistics::wireCount() const
{
    return wireCount_;
}

void ShapeStatistics::setWireCount(int count)
{
    wireCount_ = count;
}

int ShapeStatistics::edgeCount() const
{
    return edgeCount_;
}

void ShapeStatistics::setEdgeCount(int count)
{
    edgeCount_ = count;
}

int ShapeStatistics::vertexCount() const
{
    return vertexCount_;
}

void ShapeStatistics::setVertexCount(int count)
{
    vertexCount_ = count;
}

bool ShapeStatistics::hasValidBoundingBox() const
{
    return hasValidBoundingBox_;
}

void ShapeStatistics::setHasValidBoundingBox(bool valid)
{
    hasValidBoundingBox_ = valid;
}

const BoundingPoint& ShapeStatistics::boundingBoxMin() const
{
    return boundingBoxMin_;
}

void ShapeStatistics::setBoundingBoxMin(BoundingPoint point)
{
    boundingBoxMin_ = point;
}

const BoundingPoint& ShapeStatistics::boundingBoxMax() const
{
    return boundingBoxMax_;
}

void ShapeStatistics::setBoundingBoxMax(BoundingPoint point)
{
    boundingBoxMax_ = point;
}

} // namespace osi::core

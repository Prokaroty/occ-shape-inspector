#include "osi/core/IssueLocation.h"

#include <utility>

namespace osi::core {

std::string toString(IssueLocationKind kind)
{
    switch (kind) {
    case IssueLocationKind::Global:
        return "Global";
    case IssueLocationKind::Shape:
        return "Shape";
    case IssueLocationKind::Compound:
        return "Compound";
    case IssueLocationKind::Solid:
        return "Solid";
    case IssueLocationKind::Shell:
        return "Shell";
    case IssueLocationKind::Face:
        return "Face";
    case IssueLocationKind::Wire:
        return "Wire";
    case IssueLocationKind::Edge:
        return "Edge";
    case IssueLocationKind::Vertex:
        return "Vertex";
    case IssueLocationKind::Unknown:
    default:
        return "Unknown";
    }
}

IssueLocation::IssueLocation() = default;

IssueLocation::IssueLocation(std::string shapePath, int faceIndex, int edgeIndex, int vertexIndex,
                             IssueLocationKind kind)
    : shapePath_(std::move(shapePath)), faceIndex_(faceIndex), edgeIndex_(edgeIndex),
      vertexIndex_(vertexIndex), kind_(kind)
{
}

bool IssueLocation::isValid() const
{
    return !shapePath_.empty() || faceIndex_ >= 0 || edgeIndex_ >= 0 || vertexIndex_ >= 0 ||
           kind_ != IssueLocationKind::Unknown;
}

const std::string& IssueLocation::shapePath() const
{
    return shapePath_;
}

void IssueLocation::setShapePath(std::string shapePath)
{
    shapePath_ = std::move(shapePath);
}

int IssueLocation::faceIndex() const
{
    return faceIndex_;
}

void IssueLocation::setFaceIndex(int faceIndex)
{
    faceIndex_ = faceIndex;
}

int IssueLocation::edgeIndex() const
{
    return edgeIndex_;
}

void IssueLocation::setEdgeIndex(int edgeIndex)
{
    edgeIndex_ = edgeIndex;
}

int IssueLocation::vertexIndex() const
{
    return vertexIndex_;
}

void IssueLocation::setVertexIndex(int vertexIndex)
{
    vertexIndex_ = vertexIndex;
}

IssueLocationKind IssueLocation::kind() const
{
    return kind_;
}

void IssueLocation::setKind(IssueLocationKind kind)
{
    kind_ = kind;
}

} // namespace osi::core

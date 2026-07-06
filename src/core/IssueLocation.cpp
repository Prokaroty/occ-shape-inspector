#include "osi/core/IssueLocation.h"

#include <utility>

namespace osi::core {

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

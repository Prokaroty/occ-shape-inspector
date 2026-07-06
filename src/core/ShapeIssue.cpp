#include "osi/core/ShapeIssue.h"

#include <utility>

namespace osi::core {

std::string toString(ShapeIssueType type)
{
    switch (type) {
    case ShapeIssueType::InvalidShape:
        return "InvalidShape";
    case ShapeIssueType::FreeEdge:
        return "FreeEdge";
    case ShapeIssueType::SmallEdge:
        return "SmallEdge";
    case ShapeIssueType::SmallFace:
        return "SmallFace";
    case ShapeIssueType::DegeneratedEdge:
        return "DegeneratedEdge";
    case ShapeIssueType::OpenWire:
        return "OpenWire";
    case ShapeIssueType::NonManifoldEdge:
        return "NonManifoldEdge";
    case ShapeIssueType::Unknown:
    default:
        return "Unknown";
    }
}

std::string toString(ShapeIssueSeverity severity)
{
    switch (severity) {
    case ShapeIssueSeverity::Warning:
        return "Warning";
    case ShapeIssueSeverity::Error:
        return "Error";
    case ShapeIssueSeverity::Info:
    default:
        return "Info";
    }
}

ShapeIssue::ShapeIssue() = default;

ShapeIssue::ShapeIssue(ShapeIssueType type, ShapeIssueSeverity severity, std::string message,
                       IssueLocation location)
    : type_(type), severity_(severity), message_(std::move(message)), location_(std::move(location))
{
}

ShapeIssueType ShapeIssue::type() const
{
    return type_;
}

void ShapeIssue::setType(ShapeIssueType type)
{
    type_ = type;
}

ShapeIssueSeverity ShapeIssue::severity() const
{
    return severity_;
}

void ShapeIssue::setSeverity(ShapeIssueSeverity severity)
{
    severity_ = severity;
}

const std::string& ShapeIssue::message() const
{
    return message_;
}

void ShapeIssue::setMessage(std::string message)
{
    message_ = std::move(message);
}

const IssueLocation& ShapeIssue::location() const
{
    return location_;
}

void ShapeIssue::setLocation(IssueLocation location)
{
    location_ = std::move(location);
}

const std::string& ShapeIssue::shapePath() const
{
    return location_.shapePath();
}

void ShapeIssue::setShapePath(std::string shapePath)
{
    location_.setShapePath(std::move(shapePath));
}

int ShapeIssue::faceIndex() const
{
    return location_.faceIndex();
}

void ShapeIssue::setFaceIndex(int faceIndex)
{
    location_.setFaceIndex(faceIndex);
}

int ShapeIssue::edgeIndex() const
{
    return location_.edgeIndex();
}

void ShapeIssue::setEdgeIndex(int edgeIndex)
{
    location_.setEdgeIndex(edgeIndex);
}

const std::string& ShapeIssue::locationId() const
{
    return locationId_;
}

void ShapeIssue::setLocationId(std::string locationId)
{
    locationId_ = std::move(locationId);
}

} // namespace osi::core

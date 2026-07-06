#include "osi/report/ShapeReport.h"

#include <sstream>
#include <utility>

namespace osi::report {

ShapeReport::ShapeReport() = default;

const std::string& ShapeReport::sourceFilePath() const
{
    return sourceFilePath_;
}

void ShapeReport::setSourceFilePath(std::string sourceFilePath)
{
    sourceFilePath_ = std::move(sourceFilePath);
}

const osi::core::ShapeStatistics& ShapeReport::statistics() const
{
    return statistics_;
}

void ShapeReport::setStatistics(osi::core::ShapeStatistics statistics)
{
    statistics_ = statistics;
}

const std::vector<osi::core::ShapeIssue>& ShapeReport::issues() const
{
    return issues_;
}

void ShapeReport::addIssue(osi::core::ShapeIssue issue)
{
    issues_.push_back(std::move(issue));
}

bool ShapeReport::success() const
{
    return success_;
}

void ShapeReport::setSuccess(bool success)
{
    success_ = success;
}

const std::string& ShapeReport::errorMessage() const
{
    return errorMessage_;
}

void ShapeReport::setErrorMessage(std::string errorMessage)
{
    errorMessage_ = std::move(errorMessage);
}

std::string ShapeReport::summary() const
{
    std::ostringstream stream;
    stream << "Source: " << (sourceFilePath_.empty() ? "<none>" : sourceFilePath_) << '\n';
    stream << "Status: " << (success_ ? "success" : "error") << '\n';
    if (!errorMessage_.empty()) {
        stream << "Error: " << errorMessage_ << '\n';
    }
    stream << "Topology: compounds=" << statistics_.compoundCount()
           << ", solids=" << statistics_.solidCount() << ", shells=" << statistics_.shellCount()
           << ", faces=" << statistics_.faceCount() << ", wires=" << statistics_.wireCount()
           << ", edges=" << statistics_.edgeCount()
           << ", vertices=" << statistics_.vertexCount() << '\n';
    stream << "Bounding box: " << (statistics_.hasValidBoundingBox() ? "valid" : "not available")
           << '\n';
    stream << "Issues: " << issues_.size();
    return stream.str();
}

} // namespace osi::report

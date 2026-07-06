#include "osi/core/ShapeDocument.h"

#include <utility>

namespace osi::core {

ShapeDocument::ShapeDocument() = default;

ShapeDocument::ShapeDocument(std::string filePath, std::string displayName)
    : filePath_(std::move(filePath)), displayName_(std::move(displayName))
{
}

const std::string& ShapeDocument::filePath() const
{
    return filePath_;
}

void ShapeDocument::setFilePath(std::string filePath)
{
    filePath_ = std::move(filePath);
}

const std::string& ShapeDocument::displayName() const
{
    return displayName_;
}

void ShapeDocument::setDisplayName(std::string displayName)
{
    displayName_ = std::move(displayName);
}

ShapeLoadStatus ShapeDocument::loadStatus() const
{
    return loadStatus_;
}

void ShapeDocument::setLoadStatus(ShapeLoadStatus status)
{
    loadStatus_ = status;
}

const std::string& ShapeDocument::errorMessage() const
{
    return errorMessage_;
}

void ShapeDocument::setErrorMessage(std::string errorMessage)
{
    errorMessage_ = std::move(errorMessage);
}

const ShapeStatistics& ShapeDocument::statistics() const
{
    return statistics_;
}

ShapeStatistics& ShapeDocument::statistics()
{
    return statistics_;
}

void ShapeDocument::setStatistics(ShapeStatistics statistics)
{
    statistics_ = statistics;
}

} // namespace osi::core

#pragma once

#include "osi/core/ShapeStatistics.h"

#include <string>

namespace osi::core {

/**
 * @brief Load state for a shape document.
 */
enum class ShapeLoadStatus {
    NotLoaded,
    Loaded,
    Failed
};

/**
 * @brief CAD document metadata and statistics without direct OCCT ownership.
 *
 * This class deliberately avoids TopoDS_Shape in Phase 0 so core remains buildable
 * without OCCT. Later phases can add a PIMPL or OCCT-specific adapter outside this
 * boundary.
 */
class ShapeDocument {
public:
    /**
     * @brief Creates an empty document in NotLoaded state.
     */
    ShapeDocument();

    /**
     * @brief Creates a document with a source file path and display name.
     */
    ShapeDocument(std::string filePath, std::string displayName);

    /**
     * @brief Returns the source file path.
     */
    const std::string& filePath() const;

    /**
     * @brief Sets the source file path.
     */
    void setFilePath(std::string filePath);

    /**
     * @brief Returns the display name used by CLI and GUI.
     */
    const std::string& displayName() const;

    /**
     * @brief Sets the display name used by CLI and GUI.
     */
    void setDisplayName(std::string displayName);

    /**
     * @brief Returns the current load status.
     */
    ShapeLoadStatus loadStatus() const;

    /**
     * @brief Sets the current load status.
     */
    void setLoadStatus(ShapeLoadStatus status);

    /**
     * @brief Returns the load or analysis error message.
     */
    const std::string& errorMessage() const;

    /**
     * @brief Sets the load or analysis error message.
     */
    void setErrorMessage(std::string errorMessage);

    /**
     * @brief Returns the document statistics.
     */
    const ShapeStatistics& statistics() const;

    /**
     * @brief Returns mutable document statistics.
     */
    ShapeStatistics& statistics();

    /**
     * @brief Replaces the document statistics.
     */
    void setStatistics(ShapeStatistics statistics);

private:
    std::string filePath_;
    std::string displayName_;
    ShapeLoadStatus loadStatus_ = ShapeLoadStatus::NotLoaded;
    std::string errorMessage_;
    ShapeStatistics statistics_;
};

} // namespace osi::core

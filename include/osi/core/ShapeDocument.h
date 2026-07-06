#pragma once

#include "osi/core/ShapeStatistics.h"

#include <memory>
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

    /**
     * @brief Returns true when a native geometry handle is attached.
     */
    bool hasNativeShape() const;

    /**
     * @brief Returns the native shape type identifier, for example "TopoDS_Shape".
     */
    const std::string& nativeShapeType() const;

    /**
     * @brief Returns an opaque native shape handle.
     *
     * The handle is intentionally type-erased so core does not expose OCCT headers.
     * OCCT-aware modules may cast it after checking nativeShapeType().
     */
    std::shared_ptr<const void> nativeShape() const;

    /**
     * @brief Attaches an opaque native shape handle.
     */
    void setNativeShape(std::shared_ptr<void> nativeShape, std::string nativeShapeType);

    /**
     * @brief Clears any attached native shape handle.
     */
    void clearNativeShape();

private:
    std::string filePath_;
    std::string displayName_;
    ShapeLoadStatus loadStatus_ = ShapeLoadStatus::NotLoaded;
    std::string errorMessage_;
    ShapeStatistics statistics_;
    std::shared_ptr<void> nativeShape_;
    std::string nativeShapeType_;
};

} // namespace osi::core

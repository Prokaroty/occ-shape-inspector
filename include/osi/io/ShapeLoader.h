#pragma once

#include "osi/core/ShapeDocument.h"

#include <string>

namespace osi::io {

/**
 * @brief Result state for loading a CAD shape document.
 */
enum class ShapeLoadResultStatus {
    Success,
    Failed,
    UnsupportedFormat,
    OcctDisabled
};

/**
 * @brief Result object returned by ShapeLoader.
 */
class ShapeLoadResult {
public:
    /**
     * @brief Creates a failed result with an empty document.
     */
    ShapeLoadResult();

    /**
     * @brief Creates a result with status, document, and message.
     */
    ShapeLoadResult(ShapeLoadResultStatus status, osi::core::ShapeDocument document,
                    std::string message);

    /**
     * @brief Returns the loader result status.
     */
    ShapeLoadResultStatus status() const;

    /**
     * @brief Returns true when loading succeeded.
     */
    bool success() const;

    /**
     * @brief Returns the loaded or partially initialized document.
     */
    const osi::core::ShapeDocument& document() const;

    /**
     * @brief Returns mutable loaded or partially initialized document.
     */
    osi::core::ShapeDocument& document();

    /**
     * @brief Returns a human-readable loader message.
     */
    const std::string& message() const;

private:
    ShapeLoadResultStatus status_ = ShapeLoadResultStatus::Failed;
    osi::core::ShapeDocument document_;
    std::string message_;
};

/**
 * @brief Loads STEP and BREP documents through OCCT when available.
 */
class ShapeLoader {
public:
    /**
     * @brief Creates a lightweight loader.
     */
    ShapeLoader();

    /**
     * @brief Loads a model file into a ShapeDocument.
     *
     * Supported extensions are .step, .stp, .brep, and .rle.
     */
    ShapeLoadResult load(const std::string& filePath) const;
};

/**
 * @brief Converts a loader status into a stable English identifier.
 */
std::string toString(ShapeLoadResultStatus status);

} // namespace osi::io

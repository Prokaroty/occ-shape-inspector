#pragma once

#include "osi/analyzer/ShapeAnalyzerOptions.h"
#include "osi/core/ShapeDocument.h"
#include "osi/core/ShapeIssue.h"

#include <vector>

namespace osi::analyzer {

/**
 * @brief Detects first-pass CAD shape issues from a loaded document.
 *
 * The detector is analyzer-owned logic. It does not format reports, parse CLI arguments,
 * drive a viewer, or attempt repair.
 */
class ShapeIssueDetector {
public:
    /**
     * @brief Creates a lightweight issue detector.
     */
    ShapeIssueDetector();

    /**
     * @brief Detects issues from a loaded shape document using the provided options.
     */
    std::vector<osi::core::ShapeIssue> detect(const osi::core::ShapeDocument& document,
                                              const ShapeAnalyzerOptions& options) const;
};

} // namespace osi::analyzer

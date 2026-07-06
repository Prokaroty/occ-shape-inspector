#pragma once

#include "osi/analyzer/ShapeAnalyzerOptions.h"
#include "osi/core/ShapeDocument.h"
#include "osi/report/ShapeReport.h"

namespace osi::analyzer {

/**
 * @brief Runs shape analysis and produces viewer-independent reports.
 *
 * Phase 0 keeps this class free of OCCT and Qt. Later implementations may use OCCT
 * internally, but the analyzer must still avoid Qt and viewer dependencies.
 */
class ShapeAnalyzer {
public:
    /**
     * @brief Creates an analyzer with no heavyweight work.
     */
    ShapeAnalyzer();

    /**
     * @brief Analyzes a shape document with explicit options.
     */
    osi::report::ShapeReport analyze(const osi::core::ShapeDocument& document,
                                     const ShapeAnalyzerOptions& options) const;
};

} // namespace osi::analyzer

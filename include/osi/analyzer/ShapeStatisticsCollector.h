#pragma once

#include "osi/core/ShapeDocument.h"
#include "osi/core/ShapeStatistics.h"

#include <string>

namespace osi::analyzer {

/**
 * @brief Result object returned by ShapeStatisticsCollector.
 */
class ShapeStatisticsResult {
public:
    /**
     * @brief Creates a failed statistics result.
     */
    ShapeStatisticsResult();

    /**
     * @brief Creates a statistics result with success state, statistics, and message.
     */
    ShapeStatisticsResult(bool success, osi::core::ShapeStatistics statistics, std::string message);

    /**
     * @brief Returns true when statistics collection succeeded.
     */
    bool success() const;

    /**
     * @brief Returns collected statistics.
     */
    const osi::core::ShapeStatistics& statistics() const;

    /**
     * @brief Returns a human-readable status or error message.
     */
    const std::string& message() const;

private:
    bool success_ = false;
    osi::core::ShapeStatistics statistics_;
    std::string message_;
};

/**
 * @brief Collects topology statistics from loaded shape documents.
 *
 * The collector owns OCCT traversal logic when OCCT is available. It deliberately
 * has no Qt, GUI, or viewer dependencies so CLI and future GUI can reuse it.
 */
class ShapeStatisticsCollector {
public:
    /**
     * @brief Creates a lightweight statistics collector.
     */
    ShapeStatisticsCollector();

    /**
     * @brief Collects topology statistics from the document native shape.
     */
    ShapeStatisticsResult collect(const osi::core::ShapeDocument& document) const;
};

} // namespace osi::analyzer

#pragma once

#include "osi/core/ShapeIssue.h"
#include "osi/core/ShapeStatistics.h"

#include <string>
#include <vector>

namespace osi::report {

/**
 * @brief Analysis report exchanged between analyzer, CLI, and future GUI.
 */
class ShapeReport {
public:
    /**
     * @brief Creates an empty successful report.
     */
    ShapeReport();

    /**
     * @brief Returns the source file path used to create the report.
     */
    const std::string& sourceFilePath() const;

    /**
     * @brief Sets the source file path used to create the report.
     */
    void setSourceFilePath(std::string sourceFilePath);

    /**
     * @brief Returns topology and bounding-box statistics.
     */
    const osi::core::ShapeStatistics& statistics() const;

    /**
     * @brief Replaces topology and bounding-box statistics.
     */
    void setStatistics(osi::core::ShapeStatistics statistics);

    /**
     * @brief Returns all detected issues.
     */
    const std::vector<osi::core::ShapeIssue>& issues() const;

    /**
     * @brief Adds one detected issue to the report.
     */
    void addIssue(osi::core::ShapeIssue issue);

    /**
     * @brief Returns whether analysis/report creation succeeded.
     */
    bool success() const;

    /**
     * @brief Sets whether analysis/report creation succeeded.
     */
    void setSuccess(bool success);

    /**
     * @brief Returns the report-level error message.
     */
    const std::string& errorMessage() const;

    /**
     * @brief Sets the report-level error message.
     */
    void setErrorMessage(std::string errorMessage);

    /**
     * @brief Builds a short human-readable summary for CLI output.
     */
    std::string summary() const;

private:
    std::string sourceFilePath_;
    osi::core::ShapeStatistics statistics_;
    std::vector<osi::core::ShapeIssue> issues_;
    bool success_ = true;
    std::string errorMessage_;
};

} // namespace osi::report

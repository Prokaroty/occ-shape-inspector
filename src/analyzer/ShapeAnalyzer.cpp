#include "osi/analyzer/ShapeAnalyzer.h"

#include "osi/analyzer/ShapeIssueDetector.h"
#include "osi/analyzer/ShapeStatisticsCollector.h"

#include <utility>

namespace osi::analyzer {

bool ShapeAnalyzerOptions::enableBasicChecks() const
{
    return enableBasicChecks_;
}

void ShapeAnalyzerOptions::setEnableBasicChecks(bool enabled)
{
    enableBasicChecks_ = enabled;
}

bool ShapeAnalyzerOptions::enableInvalidShapeCheck() const
{
    return enableBasicChecks_ && enableInvalidShapeCheck_;
}

void ShapeAnalyzerOptions::setEnableInvalidShapeCheck(bool enabled)
{
    enableInvalidShapeCheck_ = enabled;
}

bool ShapeAnalyzerOptions::enableFreeEdgeCheck() const
{
    return enableBasicChecks_ && enableFreeEdgeCheck_;
}

void ShapeAnalyzerOptions::setEnableFreeEdgeCheck(bool enabled)
{
    enableFreeEdgeCheck_ = enabled;
}

bool ShapeAnalyzerOptions::enableSmallEdgeCheck() const
{
    return enableBasicChecks_ && enableSmallEdgeCheck_;
}

void ShapeAnalyzerOptions::setEnableSmallEdgeCheck(bool enabled)
{
    enableSmallEdgeCheck_ = enabled;
}

bool ShapeAnalyzerOptions::enableSmallFaceCheck() const
{
    return enableBasicChecks_ && enableSmallFaceCheck_;
}

void ShapeAnalyzerOptions::setEnableSmallFaceCheck(bool enabled)
{
    enableSmallFaceCheck_ = enabled;
}

bool ShapeAnalyzerOptions::enableDegeneratedEdgeCheck() const
{
    return enableBasicChecks_ && enableDegeneratedEdgeCheck_;
}

void ShapeAnalyzerOptions::setEnableDegeneratedEdgeCheck(bool enabled)
{
    enableDegeneratedEdgeCheck_ = enabled;
}

double ShapeAnalyzerOptions::smallEdgeLengthThreshold() const
{
    return smallEdgeLengthThreshold_;
}

void ShapeAnalyzerOptions::setSmallEdgeLengthThreshold(double threshold)
{
    smallEdgeLengthThreshold_ = threshold;
}

double ShapeAnalyzerOptions::smallFaceAreaThreshold() const
{
    return smallFaceAreaThreshold_;
}

void ShapeAnalyzerOptions::setSmallFaceAreaThreshold(double threshold)
{
    smallFaceAreaThreshold_ = threshold;
}

ShapeAnalyzer::ShapeAnalyzer() = default;

osi::report::ShapeReport ShapeAnalyzer::analyze(const osi::core::ShapeDocument& document,
                                                const ShapeAnalyzerOptions& options) const
{
    osi::report::ShapeReport report;
    report.setSourceFilePath(document.filePath());
    report.setStatistics(document.statistics());

    if (document.loadStatus() == osi::core::ShapeLoadStatus::Failed) {
        report.setSuccess(false);
        report.setErrorMessage(document.errorMessage());
        return report;
    }

    if (document.loadStatus() != osi::core::ShapeLoadStatus::Loaded) {
        report.setSuccess(false);
        report.setErrorMessage("Document is not loaded.");
        return report;
    }

    ShapeStatisticsCollector collector;
    const ShapeStatisticsResult statisticsResult = collector.collect(document);
    if (!statisticsResult.success()) {
        report.setSuccess(false);
        report.setErrorMessage(statisticsResult.message());
        return report;
    }

    report.setStatistics(statisticsResult.statistics());
    ShapeIssueDetector issueDetector;
    for (auto issue : issueDetector.detect(document, options)) {
        report.addIssue(std::move(issue));
    }
    return report;
}

} // namespace osi::analyzer

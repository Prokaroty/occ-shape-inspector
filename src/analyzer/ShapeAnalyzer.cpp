#include "osi/analyzer/ShapeAnalyzer.h"

namespace osi::analyzer {

bool ShapeAnalyzerOptions::enableBasicChecks() const
{
    return enableBasicChecks_;
}

void ShapeAnalyzerOptions::setEnableBasicChecks(bool enabled)
{
    enableBasicChecks_ = enabled;
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
    (void)options;

    osi::report::ShapeReport report;
    report.setSourceFilePath(document.filePath());
    report.setStatistics(document.statistics());

    if (document.loadStatus() == osi::core::ShapeLoadStatus::Failed) {
        report.setSuccess(false);
        report.setErrorMessage(document.errorMessage());
    }

    return report;
}

} // namespace osi::analyzer

#include "osi/core/ShapeIssue.h"
#include "osi/core/ShapeStatistics.h"
#include "osi/report/ShapeReport.h"
#include "osi/analyzer/ShapeAnalyzerOptions.h"

#include <cstdlib>
#include <iostream>

namespace {

void require(bool condition, const char* message)
{
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(1);
    }
}

} // namespace

int main()
{
    osi::core::ShapeIssue defaultIssue;
    require(defaultIssue.type() == osi::core::ShapeIssueType::Unknown,
            "ShapeIssue default type should be Unknown");
    require(defaultIssue.severity() == osi::core::ShapeIssueSeverity::Info,
            "ShapeIssue default severity should be Info");
    require(defaultIssue.faceIndex() == -1, "ShapeIssue default face index should be invalid");
    require(defaultIssue.edgeIndex() == -1, "ShapeIssue default edge index should be invalid");
    require(osi::core::toString(osi::core::ShapeIssueType::FreeEdge) == "FreeEdge",
            "ShapeIssueType string conversion should be stable");
    require(osi::core::toString(osi::core::ShapeIssueSeverity::Warning) == "Warning",
            "ShapeIssueSeverity string conversion should be stable");

    osi::core::IssueLocation defaultLocation;
    require(!defaultLocation.isValid(), "IssueLocation should default to invalid");
    require(defaultLocation.kind() == osi::core::IssueLocationKind::Unknown,
            "IssueLocation kind should default to Unknown");
    require(osi::core::toString(osi::core::IssueLocationKind::Edge) == "Edge",
            "IssueLocationKind string conversion should be stable");

    osi::analyzer::ShapeAnalyzerOptions options;
    require(options.enableInvalidShapeCheck(), "Invalid shape check should default to enabled");
    require(options.enableFreeEdgeCheck(), "Free edge check should default to enabled");
    require(options.enableSmallEdgeCheck(), "Small edge check should default to enabled");
    require(options.enableSmallFaceCheck(), "Small face check should default to enabled");
    require(options.enableDegeneratedEdgeCheck(),
            "Degenerated edge check should default to enabled");
    require(options.smallEdgeLengthThreshold() > 0.0,
            "Small edge threshold should default to positive");
    require(options.smallFaceAreaThreshold() > 0.0,
            "Small face threshold should default to positive");

    osi::core::ShapeStatistics statistics;
    require(statistics.compoundCount() == 0, "ShapeStatistics compound count should default to 0");
    require(statistics.compSolidCount() == 0,
            "ShapeStatistics compsolid count should default to 0");
    require(statistics.solidCount() == 0, "ShapeStatistics solid count should default to 0");
    require(!statistics.hasValidBoundingBox(),
            "ShapeStatistics bounding box should default to invalid");

    statistics.setCompSolidCount(2);
    statistics.setFaceCount(12);

    osi::report::ShapeReport report;
    report.setStatistics(statistics);
    report.addIssue(osi::core::ShapeIssue(
        osi::core::ShapeIssueType::FreeEdge, osi::core::ShapeIssueSeverity::Warning,
        "free edge placeholder",
        osi::core::IssueLocation("/shape/0", -1, 3, -1, osi::core::IssueLocationKind::Edge)));

    require(report.issues().size() == 1, "ShapeReport should store added issues");
    require(report.statistics().compSolidCount() == 2,
            "ShapeReport should store supplied statistics");
    require(report.statistics().faceCount() == 12, "ShapeReport should store face count");
    require(report.summary().find("Issues: 1") != std::string::npos,
            "ShapeReport summary should include issue count");

    return 0;
}

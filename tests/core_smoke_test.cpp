#include "osi/core/ShapeIssue.h"
#include "osi/core/ShapeStatistics.h"
#include "osi/report/ShapeReport.h"

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

    osi::core::ShapeStatistics statistics;
    require(statistics.compoundCount() == 0, "ShapeStatistics compound count should default to 0");
    require(statistics.solidCount() == 0, "ShapeStatistics solid count should default to 0");
    require(!statistics.hasValidBoundingBox(),
            "ShapeStatistics bounding box should default to invalid");

    osi::report::ShapeReport report;
    report.setStatistics(statistics);
    report.addIssue(osi::core::ShapeIssue(
        osi::core::ShapeIssueType::FreeEdge, osi::core::ShapeIssueSeverity::Warning,
        "free edge placeholder",
        osi::core::IssueLocation("/shape/0", -1, 3, -1, osi::core::IssueLocationKind::Edge)));

    require(report.issues().size() == 1, "ShapeReport should store added issues");
    require(report.summary().find("Issues: 1") != std::string::npos,
            "ShapeReport summary should include issue count");

    return 0;
}

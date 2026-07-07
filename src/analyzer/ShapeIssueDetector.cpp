#include "osi/analyzer/ShapeIssueDetector.h"

#include "osi/osi_config.h"

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#if OSI_HAS_OCCT
#include <BRepCheck_Analyzer.hxx>
#include <BRepGProp.hxx>
#include <BRep_Tool.hxx>
#include <GProp_GProps.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#endif

namespace osi::analyzer {
namespace {

#if OSI_HAS_OCCT
std::shared_ptr<const TopoDS_Shape> nativeShapeFromDocument(
    const osi::core::ShapeDocument& document)
{
    if (!document.hasNativeShape() || document.nativeShapeType() != "TopoDS_Shape") {
        return {};
    }

    return std::static_pointer_cast<const TopoDS_Shape>(document.nativeShape());
}

std::string indexedPath(const char* prefix, int index)
{
    std::ostringstream stream;
    stream << prefix << ':' << index;
    return stream.str();
}

std::string formatDouble(double value)
{
    std::ostringstream stream;
    stream << std::setprecision(12) << value;
    return stream.str();
}

osi::core::ShapeIssue makeIssue(osi::core::ShapeIssueType type,
                                osi::core::ShapeIssueSeverity severity,
                                std::string message,
                                osi::core::IssueLocation location)
{
    osi::core::ShapeIssue issue(type, severity, std::move(message), std::move(location));
    issue.setLocationId(issue.shapePath());
    return issue;
}

void detectInvalidShape(const TopoDS_Shape& shape, std::vector<osi::core::ShapeIssue>& issues)
{
    // Phase 2 keeps invalid-shape detection coarse: one global issue records that the
    // OCCT validity check failed. Detailed sub-shape diagnostics can be expanded later.
    BRepCheck_Analyzer analyzer(shape);
    if (analyzer.IsValid()) {
        return;
    }

    issues.push_back(makeIssue(
        osi::core::ShapeIssueType::InvalidShape, osi::core::ShapeIssueSeverity::Error,
        "OCCT shape validity check failed.",
        osi::core::IssueLocation("global", -1, -1, -1, osi::core::IssueLocationKind::Global)));
}

void detectFreeEdges(const TopoDS_Shape& shape, std::vector<osi::core::ShapeIssue>& issues)
{
    TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;
    TopExp::MapShapesAndAncestors(shape, TopAbs_EDGE, TopAbs_FACE, edgeFaceMap);

    for (Standard_Integer index = 1; index <= edgeFaceMap.Extent(); ++index) {
        const int edgeIndex = static_cast<int>(index);
        if (edgeFaceMap.FindFromIndex(index).Extent() != 1) {
            continue;
        }

        const std::string path = indexedPath("edge", edgeIndex);
        std::ostringstream message;
        message << "Free edge detected, edge index=" << edgeIndex;
        issues.push_back(makeIssue(
            osi::core::ShapeIssueType::FreeEdge, osi::core::ShapeIssueSeverity::Warning,
            message.str(),
            osi::core::IssueLocation(path, -1, edgeIndex, -1,
                                     osi::core::IssueLocationKind::Edge)));
    }
}

void detectSmallEdges(const TopoDS_Shape& shape, const ShapeAnalyzerOptions& options,
                      std::vector<osi::core::ShapeIssue>& issues)
{
    int edgeIndex = 0;
    for (TopExp_Explorer explorer(shape, TopAbs_EDGE); explorer.More(); explorer.Next()) {
        ++edgeIndex;
        try {
            GProp_GProps properties;
            BRepGProp::LinearProperties(explorer.Current(), properties);
            const double length = properties.Mass();
            if (length >= options.smallEdgeLengthThreshold()) {
                continue;
            }

            const std::string path = indexedPath("edge", edgeIndex);
            std::ostringstream message;
            message << "Small edge detected, edge index=" << edgeIndex
                    << ", length=" << formatDouble(length)
                    << ", threshold=" << formatDouble(options.smallEdgeLengthThreshold());
            issues.push_back(makeIssue(
                osi::core::ShapeIssueType::SmallEdge, osi::core::ShapeIssueSeverity::Warning,
                message.str(),
                osi::core::IssueLocation(path, -1, edgeIndex, -1,
                                         osi::core::IssueLocationKind::Edge)));
        } catch (...) {
            // Length failures are local to one edge. Keep analysis running.
        }
    }
}

void detectSmallFaces(const TopoDS_Shape& shape, const ShapeAnalyzerOptions& options,
                      std::vector<osi::core::ShapeIssue>& issues)
{
    int faceIndex = 0;
    for (TopExp_Explorer explorer(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
        ++faceIndex;
        try {
            GProp_GProps properties;
            BRepGProp::SurfaceProperties(explorer.Current(), properties);
            const double area = properties.Mass();
            if (area >= options.smallFaceAreaThreshold()) {
                continue;
            }

            const std::string path = indexedPath("face", faceIndex);
            std::ostringstream message;
            message << "Small face detected, face index=" << faceIndex
                    << ", area=" << formatDouble(area)
                    << ", threshold=" << formatDouble(options.smallFaceAreaThreshold());
            issues.push_back(makeIssue(
                osi::core::ShapeIssueType::SmallFace, osi::core::ShapeIssueSeverity::Warning,
                message.str(),
                osi::core::IssueLocation(path, faceIndex, -1, -1,
                                         osi::core::IssueLocationKind::Face)));
        } catch (...) {
            // Area failures are local to one face. Keep analysis running.
        }
    }
}

void detectDegeneratedEdges(const TopoDS_Shape& shape,
                            std::vector<osi::core::ShapeIssue>& issues)
{
    int edgeIndex = 0;
    for (TopExp_Explorer explorer(shape, TopAbs_EDGE); explorer.More(); explorer.Next()) {
        ++edgeIndex;
        try {
            const TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
            if (!BRep_Tool::Degenerated(edge)) {
                continue;
            }

            const std::string path = indexedPath("edge", edgeIndex);
            std::ostringstream message;
            message << "Degenerated edge detected, edge index=" << edgeIndex;
            issues.push_back(makeIssue(
                osi::core::ShapeIssueType::DegeneratedEdge,
                osi::core::ShapeIssueSeverity::Warning, message.str(),
                osi::core::IssueLocation(path, -1, edgeIndex, -1,
                                         osi::core::IssueLocationKind::Edge)));
        } catch (...) {
            // Degenerated-edge checks should not stop the remaining detectors.
        }
    }
}
#endif

} // namespace

ShapeIssueDetector::ShapeIssueDetector() = default;

std::vector<osi::core::ShapeIssue> ShapeIssueDetector::detect(
    const osi::core::ShapeDocument& document, const ShapeAnalyzerOptions& options) const
{
    std::vector<osi::core::ShapeIssue> issues;

#if !OSI_HAS_OCCT
    (void)document;
    (void)options;
    return issues;
#else
    const std::shared_ptr<const TopoDS_Shape> shape = nativeShapeFromDocument(document);
    if (!shape || shape->IsNull()) {
        return issues;
    }

    try {
        if (options.enableInvalidShapeCheck()) {
            detectInvalidShape(*shape, issues);
        }
        if (options.enableFreeEdgeCheck()) {
            detectFreeEdges(*shape, issues);
        }
        if (options.enableSmallEdgeCheck()) {
            detectSmallEdges(*shape, options, issues);
        }
        if (options.enableSmallFaceCheck()) {
            detectSmallFaces(*shape, options, issues);
        }
        if (options.enableDegeneratedEdgeCheck()) {
            detectDegeneratedEdges(*shape, issues);
        }
    } catch (...) {
        issues.push_back(makeIssue(
            osi::core::ShapeIssueType::Unknown, osi::core::ShapeIssueSeverity::Warning,
            "Issue detection stopped because an unexpected OCCT error occurred.",
            osi::core::IssueLocation("global", -1, -1, -1,
                                     osi::core::IssueLocationKind::Global)));
    }

    return issues;
#endif
}

} // namespace osi::analyzer

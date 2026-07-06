#include "osi/analyzer/ShapeStatisticsCollector.h"

#include "osi/osi_config.h"

#include <memory>
#include <utility>

#if OSI_HAS_OCCT
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#endif

namespace osi::analyzer {
namespace {

#if OSI_HAS_OCCT
int countSubShapes(const TopoDS_Shape& shape, TopAbs_ShapeEnum shapeType)
{
    int count = 0;
    for (TopExp_Explorer explorer(shape, shapeType); explorer.More(); explorer.Next()) {
        ++count;
    }
    return count;
}
#endif

} // namespace

ShapeStatisticsResult::ShapeStatisticsResult() = default;

ShapeStatisticsResult::ShapeStatisticsResult(bool success, osi::core::ShapeStatistics statistics,
                                             std::string message)
    : success_(success), statistics_(statistics), message_(std::move(message))
{
}

bool ShapeStatisticsResult::success() const
{
    return success_;
}

const osi::core::ShapeStatistics& ShapeStatisticsResult::statistics() const
{
    return statistics_;
}

const std::string& ShapeStatisticsResult::message() const
{
    return message_;
}

ShapeStatisticsCollector::ShapeStatisticsCollector() = default;

ShapeStatisticsResult ShapeStatisticsCollector::collect(
    const osi::core::ShapeDocument& document) const
{
#if !OSI_HAS_OCCT
    (void)document;
    return ShapeStatisticsResult(false, osi::core::ShapeStatistics(),
                                 "OCCT support is disabled; topology statistics are unavailable.");
#else
    if (document.loadStatus() != osi::core::ShapeLoadStatus::Loaded) {
        return ShapeStatisticsResult(false, osi::core::ShapeStatistics(),
                                     "Document is not loaded.");
    }

    if (!document.hasNativeShape() || document.nativeShapeType() != "TopoDS_Shape") {
        return ShapeStatisticsResult(false, osi::core::ShapeStatistics(),
                                     "Document does not contain an OCCT TopoDS_Shape.");
    }

    const std::shared_ptr<const void> nativeShape = document.nativeShape();
    const auto shape = std::static_pointer_cast<const TopoDS_Shape>(nativeShape);
    if (!shape || shape->IsNull()) {
        return ShapeStatisticsResult(false, osi::core::ShapeStatistics(),
                                     "Document contains an empty OCCT shape.");
    }

    osi::core::ShapeStatistics statistics;
    statistics.setCompoundCount(countSubShapes(*shape, TopAbs_COMPOUND));
    statistics.setCompSolidCount(countSubShapes(*shape, TopAbs_COMPSOLID));
    statistics.setSolidCount(countSubShapes(*shape, TopAbs_SOLID));
    statistics.setShellCount(countSubShapes(*shape, TopAbs_SHELL));
    statistics.setFaceCount(countSubShapes(*shape, TopAbs_FACE));
    statistics.setWireCount(countSubShapes(*shape, TopAbs_WIRE));
    statistics.setEdgeCount(countSubShapes(*shape, TopAbs_EDGE));
    statistics.setVertexCount(countSubShapes(*shape, TopAbs_VERTEX));

    Bnd_Box box;
    BRepBndLib::Add(*shape, box);
    if (!box.IsVoid()) {
        Standard_Real xmin = 0.0;
        Standard_Real ymin = 0.0;
        Standard_Real zmin = 0.0;
        Standard_Real xmax = 0.0;
        Standard_Real ymax = 0.0;
        Standard_Real zmax = 0.0;
        box.Get(xmin, ymin, zmin, xmax, ymax, zmax);
        statistics.setHasValidBoundingBox(true);
        statistics.setBoundingBoxMin({xmin, ymin, zmin});
        statistics.setBoundingBoxMax({xmax, ymax, zmax});
    }

    return ShapeStatisticsResult(true, statistics, "Topology statistics collected.");
#endif
}

} // namespace osi::analyzer

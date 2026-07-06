#include "osi/analyzer/ShapeAnalyzer.h"
#include "osi/core/ShapeDocument.h"
#include "osi/io/ShapeLoader.h"

#include <iomanip>
#include <iostream>
#include <string>

namespace {

void printHelp()
{
    std::cout << "Usage:\n"
              << "  osi-inspect --help\n"
              << "  osi-inspect <model-file>\n\n"
              << "Supported formats: .step, .stp, .brep, .rle\n"
              << "The CLI loads the model through OCCT when support is enabled at configure "
                 "time.\n";
}

void printStatistics(const osi::core::ShapeStatistics& statistics)
{
    std::cout << "Shape statistics:\n";
    std::cout << "  Compounds: " << statistics.compoundCount() << '\n';
    std::cout << "  CompSolids: " << statistics.compSolidCount() << '\n';
    std::cout << "  Solids: " << statistics.solidCount() << '\n';
    std::cout << "  Shells: " << statistics.shellCount() << '\n';
    std::cout << "  Faces: " << statistics.faceCount() << '\n';
    std::cout << "  Wires: " << statistics.wireCount() << '\n';
    std::cout << "  Edges: " << statistics.edgeCount() << '\n';
    std::cout << "  Vertices: " << statistics.vertexCount() << "\n\n";

    std::cout << "Bounding box:\n";
    if (!statistics.hasValidBoundingBox()) {
        std::cout << "  Not available\n\n";
        return;
    }

    const osi::core::BoundingPoint& min = statistics.boundingBoxMin();
    const osi::core::BoundingPoint& max = statistics.boundingBoxMax();
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  Min: " << min.x << ", " << min.y << ", " << min.z << '\n';
    std::cout << "  Max: " << max.x << ", " << max.y << ", " << max.z << "\n\n";
}

} // namespace

int main(int argc, char** argv)
{
    if (argc != 2) {
        printHelp();
        return argc == 1 ? 0 : 1;
    }

    const std::string argument = argv[1];
    if (argument == "--help" || argument == "-h") {
        printHelp();
        return 0;
    }

    osi::io::ShapeLoader loader;
    osi::io::ShapeLoadResult loadResult = loader.load(argument);

    std::cout << "File: " << argument << '\n';
    std::cout << "Load status: " << osi::io::toString(loadResult.status()) << "\n\n";

    if (!loadResult.success()) {
        std::cout << "Error: " << loadResult.message() << '\n';
        return 2;
    }

    const osi::core::ShapeDocument& document = loadResult.document();
    osi::analyzer::ShapeAnalyzer analyzer;
    osi::analyzer::ShapeAnalyzerOptions options;
    osi::report::ShapeReport report = analyzer.analyze(document, options);

    if (!report.success()) {
        std::cout << "Analysis status: error\n";
        std::cout << "Error: " << report.errorMessage() << '\n';
        return 3;
    }

    printStatistics(report.statistics());
    std::cout << "Issues: " << report.issues().size() << '\n';
    return report.success() ? 0 : 2;
}

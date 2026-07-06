#include "osi/analyzer/ShapeAnalyzer.h"
#include "osi/core/ShapeDocument.h"

#include <iostream>
#include <string>

namespace {

void printHelp()
{
    std::cout << "Usage:\n"
              << "  osi-inspect --help\n"
              << "  osi-inspect <model-file>\n\n"
              << "Phase 0 builds the CLI/report pipeline only. STEP/BREP loading will be "
                 "implemented through the OCCT loader in a later phase.\n";
}

std::string displayNameFromPath(const std::string& path)
{
    const std::string::size_type slash = path.find_last_of("/\\");
    if (slash == std::string::npos) {
        return path;
    }
    return path.substr(slash + 1);
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

    osi::core::ShapeDocument document(argument, displayNameFromPath(argument));
    document.setLoadStatus(osi::core::ShapeLoadStatus::NotLoaded);

    osi::analyzer::ShapeAnalyzer analyzer;
    osi::analyzer::ShapeAnalyzerOptions options;
    osi::report::ShapeReport report = analyzer.analyze(document, options);

    std::cout << "occ-shape-inspector placeholder report\n";
    std::cout << report.summary() << "\n\n";
    std::cout << "Model loading is not implemented yet. The OCCT ShapeLoader will be added "
                 "in a later phase.\n";

    return report.success() ? 0 : 2;
}

#include "osi/io/ShapeLoader.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
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
    osi::io::ShapeLoader loader;

    const osi::io::ShapeLoadResult missing = loader.load("definitely_missing_model.step");
    require(!missing.success(), "Missing file should not load");
    require(missing.status() == osi::io::ShapeLoadResultStatus::Failed,
            "Missing file should return failed status");
    require(missing.message().find("does not exist") != std::string::npos,
            "Missing file should return a clear message");

    const std::filesystem::path unsupportedPath =
        std::filesystem::temp_directory_path() / "osi_loader_unsupported.txt";
    {
        std::ofstream stream(unsupportedPath);
        stream << "not a cad model\n";
    }

    const osi::io::ShapeLoadResult unsupported = loader.load(unsupportedPath.string());
    require(!unsupported.success(), "Unsupported extension should not load");
    require(unsupported.status() == osi::io::ShapeLoadResultStatus::UnsupportedFormat,
            "Unsupported extension should return unsupported status");
    require(unsupported.message().find("Unsupported") != std::string::npos,
            "Unsupported extension should return a clear message");
    std::filesystem::remove(unsupportedPath);

    const std::filesystem::path invalidStepPath =
        std::filesystem::temp_directory_path() / "osi_loader_invalid.step";
    {
        std::ofstream stream(invalidStepPath);
        stream << "not a valid STEP model\n";
    }

    const osi::io::ShapeLoadResult invalidStep = loader.load(invalidStepPath.string());
    require(!invalidStep.success(), "Invalid STEP should not load");
    require(invalidStep.status() == osi::io::ShapeLoadResultStatus::Failed ||
                invalidStep.status() == osi::io::ShapeLoadResultStatus::OcctDisabled,
            "Invalid STEP should fail or report disabled OCCT");
    require(!invalidStep.message().empty(), "Invalid STEP should return a clear message");
    std::filesystem::remove(invalidStepPath);

    return 0;
}

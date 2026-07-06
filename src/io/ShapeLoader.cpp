#include "osi/io/ShapeLoader.h"

#include "osi/osi_config.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <filesystem>
#include <memory>
#include <system_error>
#include <utility>

#if OSI_HAS_OCCT
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#endif

namespace osi::io {
namespace {

std::string lowerExtension(const std::filesystem::path& path)
{
    std::string extension = path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return extension;
}

std::string displayNameFromPath(const std::filesystem::path& path)
{
    return path.filename().string();
}

bool isStepExtension(const std::string& extension)
{
    return extension == ".step" || extension == ".stp";
}

bool isBrepExtension(const std::string& extension)
{
    return extension == ".brep" || extension == ".rle";
}

ShapeLoadResult makeFailure(ShapeLoadResultStatus status, osi::core::ShapeDocument document,
                            const std::string& message)
{
    document.setLoadStatus(osi::core::ShapeLoadStatus::Failed);
    document.setErrorMessage(message);
    return ShapeLoadResult(status, std::move(document), message);
}

} // namespace

ShapeLoadResult::ShapeLoadResult() = default;

ShapeLoadResult::ShapeLoadResult(ShapeLoadResultStatus status, osi::core::ShapeDocument document,
                                 std::string message)
    : status_(status), document_(std::move(document)), message_(std::move(message))
{
}

ShapeLoadResultStatus ShapeLoadResult::status() const
{
    return status_;
}

bool ShapeLoadResult::success() const
{
    return status_ == ShapeLoadResultStatus::Success;
}

const osi::core::ShapeDocument& ShapeLoadResult::document() const
{
    return document_;
}

osi::core::ShapeDocument& ShapeLoadResult::document()
{
    return document_;
}

const std::string& ShapeLoadResult::message() const
{
    return message_;
}

ShapeLoader::ShapeLoader() = default;

ShapeLoadResult ShapeLoader::load(const std::string& filePath) const
{
    const std::filesystem::path path(filePath);
    osi::core::ShapeDocument document(path.string(), displayNameFromPath(path));

    std::error_code fileError;
    if (!std::filesystem::exists(path, fileError)) {
        const std::string suffix = fileError ? " (" + fileError.message() + ")" : "";
        return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                           "File does not exist: " + path.string() + suffix);
    }

    if (!std::filesystem::is_regular_file(path, fileError)) {
        const std::string suffix = fileError ? " (" + fileError.message() + ")" : "";
        return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                           "Path is not a regular file: " + path.string() + suffix);
    }

    const std::string extension = lowerExtension(path);
    if (!isStepExtension(extension) && !isBrepExtension(extension)) {
        return makeFailure(ShapeLoadResultStatus::UnsupportedFormat, std::move(document),
                           "Unsupported model format: " + extension);
    }

#if !OSI_HAS_OCCT
    return makeFailure(ShapeLoadResultStatus::OcctDisabled, std::move(document),
                       "OCCT support is disabled or Open CASCADE was not found at configure time.");
#else
    try {
        TopoDS_Shape shape;

        if (isStepExtension(extension)) {
            STEPControl_Reader reader;
            const IFSelect_ReturnStatus readStatus = reader.ReadFile(path.string().c_str());
            if (readStatus != IFSelect_RetDone) {
                return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                                   "OCCT failed to read STEP file.");
            }

            const Standard_Integer transferredRoots = reader.TransferRoots();
            if (transferredRoots <= 0) {
                return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                                   "OCCT did not transfer any STEP roots.");
            }

            shape = reader.OneShape();
        } else {
            BRep_Builder builder;
            if (!BRepTools::Read(shape, path.string().c_str(), builder)) {
                return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                                   "OCCT failed to read BREP file.");
            }
        }

        if (shape.IsNull()) {
            return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                               "OCCT returned an empty shape.");
        }

        document.setLoadStatus(osi::core::ShapeLoadStatus::Loaded);
        document.setNativeShape(std::make_shared<TopoDS_Shape>(shape), "TopoDS_Shape");
        return ShapeLoadResult(ShapeLoadResultStatus::Success, std::move(document),
                               "Model loaded successfully.");
    } catch (const std::exception& exception) {
        return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                           std::string("Exception while loading model: ") + exception.what());
    } catch (...) {
        return makeFailure(ShapeLoadResultStatus::Failed, std::move(document),
                           "Unknown exception while loading model.");
    }
#endif
}

std::string toString(ShapeLoadResultStatus status)
{
    switch (status) {
    case ShapeLoadResultStatus::Success:
        return "success";
    case ShapeLoadResultStatus::UnsupportedFormat:
        return "unsupported";
    case ShapeLoadResultStatus::OcctDisabled:
        return "occt-disabled";
    case ShapeLoadResultStatus::Failed:
    default:
        return "failed";
    }
}

} // namespace osi::io

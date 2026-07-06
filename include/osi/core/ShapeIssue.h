#pragma once

#include "osi/core/IssueLocation.h"

#include <string>

namespace osi::core {

/**
 * @brief Supported first-phase CAD shape issue categories.
 */
enum class ShapeIssueType {
    Unknown,
    InvalidShape,
    FreeEdge,
    SmallEdge,
    SmallFace,
    DegeneratedEdge,
    OpenWire,
    NonManifoldEdge
};

/**
 * @brief Severity used by reports, CLI output, and later GUI issue lists.
 */
enum class ShapeIssueSeverity {
    Info,
    Warning,
    Error
};

/**
 * @brief Converts an issue type into a stable English identifier.
 */
std::string toString(ShapeIssueType type);

/**
 * @brief Converts an issue severity into a stable English identifier.
 */
std::string toString(ShapeIssueSeverity severity);

/**
 * @brief A detected model issue with viewer-independent location metadata.
 */
class ShapeIssue {
public:
    /**
     * @brief Creates an informational unknown issue with invalid location indices.
     */
    ShapeIssue();

    /**
     * @brief Creates an issue with type, severity, message, and location.
     */
    ShapeIssue(ShapeIssueType type, ShapeIssueSeverity severity, std::string message,
               IssueLocation location);

    /**
     * @brief Returns the issue type.
     */
    ShapeIssueType type() const;

    /**
     * @brief Sets the issue type.
     */
    void setType(ShapeIssueType type);

    /**
     * @brief Returns the issue severity.
     */
    ShapeIssueSeverity severity() const;

    /**
     * @brief Sets the issue severity.
     */
    void setSeverity(ShapeIssueSeverity severity);

    /**
     * @brief Returns the human-readable diagnostic message.
     */
    const std::string& message() const;

    /**
     * @brief Sets the human-readable diagnostic message.
     */
    void setMessage(std::string message);

    /**
     * @brief Returns the issue location.
     */
    const IssueLocation& location() const;

    /**
     * @brief Sets the issue location.
     */
    void setLocation(IssueLocation location);

    /**
     * @brief Returns the shape path mirrored from the location for simple consumers.
     */
    const std::string& shapePath() const;

    /**
     * @brief Sets the shape path on the issue location.
     */
    void setShapePath(std::string shapePath);

    /**
     * @brief Returns the face index, or -1 when not applicable.
     */
    int faceIndex() const;

    /**
     * @brief Sets the face index on the issue location.
     */
    void setFaceIndex(int faceIndex);

    /**
     * @brief Returns the edge index, or -1 when not applicable.
     */
    int edgeIndex() const;

    /**
     * @brief Sets the edge index on the issue location.
     */
    void setEdgeIndex(int edgeIndex);

    /**
     * @brief Returns a future-proof stable location identifier.
     */
    const std::string& locationId() const;

    /**
     * @brief Sets a future-proof stable location identifier.
     */
    void setLocationId(std::string locationId);

private:
    ShapeIssueType type_ = ShapeIssueType::Unknown;
    ShapeIssueSeverity severity_ = ShapeIssueSeverity::Info;
    std::string message_;
    IssueLocation location_;
    std::string locationId_;
};

} // namespace osi::core

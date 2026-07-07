#pragma once

#include <string>

namespace osi::core {

/**
 * @brief Describes which kind of model sub-shape an issue is attached to.
 */
enum class IssueLocationKind {
    Unknown,
    Global,
    Shape,
    Compound,
    Solid,
    Shell,
    Face,
    Wire,
    Edge,
    Vertex
};

/**
 * @brief Converts an issue location kind into a stable English identifier.
 */
std::string toString(IssueLocationKind kind);

/**
 * @brief Lightweight issue location that is independent from OCCT, Qt, and viewer code.
 *
 * The first phase stores stable path/index style identifiers only. Later OCCT-specific
 * implementations can map these identifiers back to TopoDS_Shape objects without changing
 * GUI-facing issue contracts.
 */
class IssueLocation {
public:
    /**
     * @brief Creates an unknown location with invalid indices.
     */
    IssueLocation();

    /**
     * @brief Creates a location with explicit shape path and sub-shape indices.
     */
    IssueLocation(std::string shapePath, int faceIndex, int edgeIndex, int vertexIndex,
                  IssueLocationKind kind);

    /**
     * @brief Returns true when the location has at least one useful identifier.
     */
    bool isValid() const;

    /**
     * @brief Returns the logical path to the owning shape or sub-shape.
     */
    const std::string& shapePath() const;

    /**
     * @brief Sets the logical path to the owning shape or sub-shape.
     */
    void setShapePath(std::string shapePath);

    /**
     * @brief Returns the face index, or -1 when not applicable.
     */
    int faceIndex() const;

    /**
     * @brief Sets the face index, using -1 when not applicable.
     */
    void setFaceIndex(int faceIndex);

    /**
     * @brief Returns the edge index, or -1 when not applicable.
     */
    int edgeIndex() const;

    /**
     * @brief Sets the edge index, using -1 when not applicable.
     */
    void setEdgeIndex(int edgeIndex);

    /**
     * @brief Returns the vertex index, or -1 when not applicable.
     */
    int vertexIndex() const;

    /**
     * @brief Sets the vertex index, using -1 when not applicable.
     */
    void setVertexIndex(int vertexIndex);

    /**
     * @brief Returns the issue location kind.
     */
    IssueLocationKind kind() const;

    /**
     * @brief Sets the issue location kind.
     */
    void setKind(IssueLocationKind kind);

private:
    std::string shapePath_;
    int faceIndex_ = -1;
    int edgeIndex_ = -1;
    int vertexIndex_ = -1;
    IssueLocationKind kind_ = IssueLocationKind::Unknown;
};

} // namespace osi::core

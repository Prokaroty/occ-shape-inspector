#pragma once

namespace osi::core {

/**
 * @brief Three-dimensional point used for bounding box values without geometry dependencies.
 */
struct BoundingPoint {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

/**
 * @brief Topology and bounding-box statistics for a loaded CAD shape.
 */
class ShapeStatistics {
public:
    /**
     * @brief Creates empty statistics with zero counts and invalid bounding box.
     */
    ShapeStatistics();

    /**
     * @brief Returns the number of compounds.
     */
    int compoundCount() const;

    /**
     * @brief Sets the number of compounds.
     */
    void setCompoundCount(int count);

    /**
     * @brief Returns the number of compsolids.
     */
    int compSolidCount() const;

    /**
     * @brief Sets the number of compsolids.
     */
    void setCompSolidCount(int count);

    /**
     * @brief Returns the number of solids.
     */
    int solidCount() const;

    /**
     * @brief Sets the number of solids.
     */
    void setSolidCount(int count);

    /**
     * @brief Returns the number of shells.
     */
    int shellCount() const;

    /**
     * @brief Sets the number of shells.
     */
    void setShellCount(int count);

    /**
     * @brief Returns the number of faces.
     */
    int faceCount() const;

    /**
     * @brief Sets the number of faces.
     */
    void setFaceCount(int count);

    /**
     * @brief Returns the number of wires.
     */
    int wireCount() const;

    /**
     * @brief Sets the number of wires.
     */
    void setWireCount(int count);

    /**
     * @brief Returns the number of edges.
     */
    int edgeCount() const;

    /**
     * @brief Sets the number of edges.
     */
    void setEdgeCount(int count);

    /**
     * @brief Returns the number of vertices.
     */
    int vertexCount() const;

    /**
     * @brief Sets the number of vertices.
     */
    void setVertexCount(int count);

    /**
     * @brief Returns whether the bounding box values are valid.
     */
    bool hasValidBoundingBox() const;

    /**
     * @brief Sets whether the bounding box values are valid.
     */
    void setHasValidBoundingBox(bool valid);

    /**
     * @brief Returns the minimum bounding box point.
     */
    const BoundingPoint& boundingBoxMin() const;

    /**
     * @brief Sets the minimum bounding box point.
     */
    void setBoundingBoxMin(BoundingPoint point);

    /**
     * @brief Returns the maximum bounding box point.
     */
    const BoundingPoint& boundingBoxMax() const;

    /**
     * @brief Sets the maximum bounding box point.
     */
    void setBoundingBoxMax(BoundingPoint point);

private:
    int compoundCount_ = 0;
    int compSolidCount_ = 0;
    int solidCount_ = 0;
    int shellCount_ = 0;
    int faceCount_ = 0;
    int wireCount_ = 0;
    int edgeCount_ = 0;
    int vertexCount_ = 0;
    bool hasValidBoundingBox_ = false;
    BoundingPoint boundingBoxMin_;
    BoundingPoint boundingBoxMax_;
};

} // namespace osi::core

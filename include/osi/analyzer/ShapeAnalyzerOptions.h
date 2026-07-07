#pragma once

namespace osi::analyzer {

/**
 * @brief Options controlling shape analysis thresholds and enabled checks.
 */
class ShapeAnalyzerOptions {
public:
    /**
     * @brief Creates default analysis options.
     */
    ShapeAnalyzerOptions() = default;

    /**
     * @brief Returns whether all basic issue checks are enabled.
     */
    bool enableBasicChecks() const;

    /**
     * @brief Enables or disables all basic issue checks.
     */
    void setEnableBasicChecks(bool enabled);

    /**
     * @brief Returns whether invalid shape checks are enabled.
     */
    bool enableInvalidShapeCheck() const;

    /**
     * @brief Enables or disables invalid shape checks.
     */
    void setEnableInvalidShapeCheck(bool enabled);

    /**
     * @brief Returns whether free edge checks are enabled.
     */
    bool enableFreeEdgeCheck() const;

    /**
     * @brief Enables or disables free edge checks.
     */
    void setEnableFreeEdgeCheck(bool enabled);

    /**
     * @brief Returns whether small edge checks are enabled.
     */
    bool enableSmallEdgeCheck() const;

    /**
     * @brief Enables or disables small edge checks.
     */
    void setEnableSmallEdgeCheck(bool enabled);

    /**
     * @brief Returns whether small face checks are enabled.
     */
    bool enableSmallFaceCheck() const;

    /**
     * @brief Enables or disables small face checks.
     */
    void setEnableSmallFaceCheck(bool enabled);

    /**
     * @brief Returns whether degenerated edge checks are enabled.
     */
    bool enableDegeneratedEdgeCheck() const;

    /**
     * @brief Enables or disables degenerated edge checks.
     */
    void setEnableDegeneratedEdgeCheck(bool enabled);

    /**
     * @brief Returns the small edge length threshold in model units.
     *
     * CAD files do not always carry normalized units. This threshold is interpreted in the
     * loaded model's native length unit.
     */
    double smallEdgeLengthThreshold() const;

    /**
     * @brief Sets the small edge length threshold in model units.
     */
    void setSmallEdgeLengthThreshold(double threshold);

    /**
     * @brief Returns the small face area threshold in squared model units.
     *
     * CAD files do not always carry normalized units. This threshold is interpreted in the
     * loaded model's native area unit.
     */
    double smallFaceAreaThreshold() const;

    /**
     * @brief Sets the small face area threshold in squared model units.
     */
    void setSmallFaceAreaThreshold(double threshold);

private:
    bool enableBasicChecks_ = true;
    bool enableInvalidShapeCheck_ = true;
    bool enableFreeEdgeCheck_ = true;
    bool enableSmallEdgeCheck_ = true;
    bool enableSmallFaceCheck_ = true;
    bool enableDegeneratedEdgeCheck_ = true;
    double smallEdgeLengthThreshold_ = 1.0e-6;
    double smallFaceAreaThreshold_ = 1.0e-8;
};

} // namespace osi::analyzer

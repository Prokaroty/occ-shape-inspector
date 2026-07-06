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
     * @brief Returns whether placeholder checks should be enabled.
     */
    bool enableBasicChecks() const;

    /**
     * @brief Enables or disables placeholder basic checks.
     */
    void setEnableBasicChecks(bool enabled);

    /**
     * @brief Returns the small edge length threshold reserved for future geometry checks.
     */
    double smallEdgeLengthThreshold() const;

    /**
     * @brief Sets the small edge length threshold reserved for future geometry checks.
     */
    void setSmallEdgeLengthThreshold(double threshold);

    /**
     * @brief Returns the small face area threshold reserved for future geometry checks.
     */
    double smallFaceAreaThreshold() const;

    /**
     * @brief Sets the small face area threshold reserved for future geometry checks.
     */
    void setSmallFaceAreaThreshold(double threshold);

private:
    bool enableBasicChecks_ = true;
    double smallEdgeLengthThreshold_ = 1.0e-6;
    double smallFaceAreaThreshold_ = 1.0e-8;
};

} // namespace osi::analyzer

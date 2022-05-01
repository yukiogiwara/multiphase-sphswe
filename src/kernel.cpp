/**
 * @file kernel.cpp
 * @brief Implementation of kernel functions
 * @author Yuki Ogiwara
 * @date 2022-05-02
 */

#include "kernel.hpp"

/**
 * @brief Poly6 kernel (2D)
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return scalar value
 */
float Poly6(float r, float h) {
    if(r < 0 || h < r) return 0;
    float coef = 4.0f / (glm::pi<float>() * powf(h, 8.0f));
    float tmp = powf(h, 2.0f) - powf(r, 2.0f);
    float val = coef * (powf(tmp, 3.0f));
    return val;
}

/**
 * @brief gradient of Poly6 kernel (2D)
 * @param[in] r_ij displacement of two particles
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return vector value
 */
glm::vec2 GradPoly6(const glm::vec2 &r_ij, float r, float h) {
    if(r < 0 || h < r) return glm::vec2(0.0f, 0.0f);
    float coef = -24.0f / (glm::pi<float>() * powf(h, 8.0f));
    float tmp = powf(h, 2.0f) - powf(r, 2.0f);
    glm::vec2 val = coef * (powf(tmp, 2.0f) * r_ij);
    return val;
}

/**
 * @brief laplacian of Poly6 kernel (2D)
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return scalar value
 */
float LaplacePoly6(float r, float h) {
    if(r < 0 || h < r) return 0.0f;
    float coef = -24.0f / (glm::pi<float>() * powf(h, 8.0f));
    float tmp = powf(h, 2.0f) - powf(r, 2.0f);
    float val = coef * (3 * powf(tmp, 2.0f) - 4 * powf(r, 2.0f) * tmp);
    return val;
}

/**
 * @brief Spiky kernel (2D)
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return scalar value
 */
float Spiky(float r, float h) {
    if(r < 0 || h < r) return 0.0f;
    float coef = 10.0f / (glm::pi<float>() * powf(h, 5.0f));
    float tmp = h - r;
    float val = coef * (powf(tmp, 3.0f));
    return val;
}

/**
 * @brief gradient of Spiky kernel (2D)
 * @param[in] r_ij displacement of two particles
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return vector value
 */
glm::vec2 GradSpiky(const glm::vec2 &r_ij, float r, float h) {
    if(r < 0 || h < r) return glm::vec2(0.0f, 0.0f);
    float coef = -30.0f / (glm::pi<float>() * powf(h, 5.0f));
    float tmp = h - r;
    glm::vec2 val = coef * (powf(tmp, 2.0f) * r_ij / r);
    return val;
}

/**
 * @brief laplacian of Spiky kernel (2D)
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return scalar value
 */
float LaplaceSpiky(float r, float h) {
    if(r < 0 || h < r) return 0.0f;
    float coef = -60.0f / (glm::pi<float>() * powf(h, 5.0f));
    float tmp = h - r;
    float val = coef * (powf(tmp, 2.0f) / r - tmp);
    return val;
}

/**
 * @brief Viscosity kernel (2D)
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return scalar value
 */
float Viscosity(float r, float h) {
    if(r < 0 || h < r) return 0.0f;
    float coef = 10.0f / (3 * glm::pi<float>() * powf(h, 2.0f));
    float val = coef * (-powf(r, 3.0f) / (2 * powf(h, 3.0f)) + powf(r, 2.0f) / powf(h, 2.0f) + h / (2 * r) - 1);
    return val;
}

/**
 * @brief gradient of Viscosity kernel (2D)
 * @param[in] r_ij displacement of two particles
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return vector value
 */
glm::vec2 GradViscosity(const glm::vec2 &r_ij, float r, float h) {
    if(r < 0 || h < r) return glm::vec2(0.0f, 0.0f);
    float coef = 10.0f / (3 * glm::pi<float>() * powf(h, 4.0f));
    glm::vec2 val = coef * ((-3 * r / (2 * h) + 2 - powf(h, 3.0f) / (2 * powf(r, 3.0f))) * r_ij);
    return val;
}

/**
 * @brief laplacian of Viscosity kernel (2D)
 * @param[in] r distance between two particles
 * @param[in] h effective radius
 * @return scalar value
 */
float LaplaceViscosity(float r, float h) {
    if(r < 0 || h < r) return 0.0f;
    float coef = 20.0f / (3 * glm::pi<float>() * powf(h, 5.0f));
    float val = coef * (h - r);
    return val;
}
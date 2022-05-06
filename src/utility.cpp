/**
 * @file utility.cpp
 * @brief Implementation of utility functions
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#include "utility.hpp"

/**
 * @brief interpolation with kernel function
 * @param[in] m mass
 * @param[in] phi physical quantity
 * @param[in] rho density
 * @param[in] r position
 * @param[in] i particle index
 * @param[in] indices neighbor particles
 * @param[in] w kernel
 * @param[in] h effective_radius
 * @return interpolated physical quantity
 */
float Interpolate(const std::vector<float> &m, const std::vector<float> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, int i, const std::vector<int> &indices, const kernel &w, float h) {
    float val = 0.0f;
    for(int j : indices) {
        glm::vec2 r_ij = r[i] - r[j];
        val += m[j] * phi[j] / rho[j] * w(glm::length(r_ij), h);
    }
    return val;
}

/**
 * @brief interpolation with kernel function
 * @param[in] m mass
 * @param[in] phi physical quantity
 * @param[in] rho density
 * @param[in] r position
 * @param[in] i particle index
 * @param[in] indices neighbor particles
 * @param[in] w kernel
 * @param[in] h effective_radius
 * @return interpolated gradient of physical quantity
 */
glm::vec2 InterpolateGradient(const std::vector<float> &m, const std::vector<float> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, int i, const std::vector<int> &indices, const gkernel &w, float h) {
    glm::vec2 val = glm::vec2(0.0f, 0.0f);
    for(int j : indices) {
        if(i == j) continue;
        glm::vec2 r_ij = r[i] - r[j];
        val += m[j] * phi[j] / rho[j] * w(r_ij, glm::length(r_ij), h);
    }
    return val;
}

/**
 * @brief interpolation with kernel function
 * @param[in] m mass
 * @param[in] phi physical quantity
 * @param[in] rho density
 * @param[in] r position
 * @param[in] i particle index
 * @param[in] indices neighbor particles
 * @param[in] w kernel
 * @param[in] h effective_radius
 * @return interpolated laplacian of physical quantity
 */
glm::vec2 InterpolateLaplacian(const std::vector<float> &m, const std::vector<glm::vec2> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, int i, const std::vector<int> &indices, const lkernel &w, float h) {
    glm::vec2 val = glm::vec2(0.0f, 0.0f);
    for(int j : indices) {
        glm::vec2 r_ij = r[i] - r[j];
        val += m[j] * (phi[j] - phi[i]) / rho[j] * w(glm::length(r_ij), h);
    }
    return val;
}

/**
 * @brief ground function
 * @param[in] r position
 * @return height
 */
float Flat(const glm::vec2 &r) {
    return -0.8;
}
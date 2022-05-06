/**
 * @file utility.hpp
 * @brief Definition of utility functions
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "type.hpp"

// interpolation with kernel function

float Interpolate(const std::vector<float> &m, const std::vector<float> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, int i, const std::vector<int> &indices, const kernel &w, float h);

glm::vec2 InterpolateGradient(const std::vector<float> &m, const std::vector<float> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, int i, const std::vector<int> &indices, const gkernel &w, float h);

glm::vec2 InterpolateLaplacian(const std::vector<float> &m, const std::vector<glm::vec2> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, int i, const std::vector<int> &indices, const lkernel &w, float h);

// ground function

float Flat(const glm::vec2 &r);
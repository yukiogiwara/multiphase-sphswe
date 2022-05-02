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

float Interpolate(const std::vector<float> &m, const std::vector<float> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int i, const std::vector<unsigned int> &indices, const kernel &w, float h);
glm::vec2 GradInterpolate(const std::vector<float> &m, const std::vector<glm::vec2> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int i, const std::vector<unsigned int> &indices, const gkernel &w, float h);
float LaplaceInterpolate(const std::vector<float> &m, const std::vector<float> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int i, const std::vector<unsigned int> &indices, const lkernel &w, float h);

void InterpolateAll(const std::vector<float> &m, std::vector<float>* phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int n, const std::vector<std::vector<unsigned int>> &indices, const kernel &w, float h);
void GradInterpolateAll(const std::vector<float> &m, std::vector<glm::vec2>* phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int n, const std::vector<std::vector<unsigned int>> &indices, const gkernel &w, float h);
void LaplaceInterpolateAll(const std::vector<float> &m, std::vector<float>* phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int n, const std::vector<std::vector<unsigned int>> &indices, const lkernel &w, float h);
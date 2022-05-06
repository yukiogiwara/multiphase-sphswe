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
void Interpolate(const std::vector<float> &m, std::vector<float>* phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int n, const std::vector<std::vector<unsigned int>> &indices, const kernel &w, float h);

glm::vec2 InterpolateGradient(const std::vector<float> &m, const std::vector<glm::vec2> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int i, const std::vector<unsigned int> &indices, const gkernel &w, float h);
void InterpolateGradient(const std::vector<float> &m, std::vector<glm::vec2>* phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int n, const std::vector<std::vector<unsigned int>> &indices, const gkernel &w, float h);

float InterpolateLaplacian(const std::vector<float> &m, const std::vector<float> &phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int i, const std::vector<unsigned int> &indices, const lkernel &w, float h);
void InterpolateLaplacian(const std::vector<float> &m, std::vector<float>* phi, const std::vector<float> &rho, const std::vector<glm::vec2> &r, unsigned int n, const std::vector<std::vector<unsigned int>> &indices, const lkernel &w, float h);

// ground function

float Flat(const glm::vec2 &r);
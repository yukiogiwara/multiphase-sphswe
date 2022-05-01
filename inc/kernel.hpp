/**
 * @file kernel.hpp
 * @brief Definition of kernel functions
 * @author Yuki Ogiwara
 * @date 2022-05-02
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Poly6

float Poly6(float r, float h);
glm::vec2 GradPoly6(const glm::vec2 &r_ij, float r, float h);
float LaplacePoly6(float r, float h);

// Spiky

float Spiky(float r, float h);
glm::vec2 GradSpiky(const glm::vec2 &r_ij, float r, float h);
float LaplaceSpiky(float r, float h);

// Viscosity

float Viscosity(float r, float h);
glm::vec2 GradViscosity(const glm::vec2 &r_ij, float r, float h);
float LaplaceViscosity(float r, float h);
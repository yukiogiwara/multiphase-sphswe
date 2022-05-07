/**
 * @file main.cpp 
 * @brief Declaration of constants
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <glm/glm.hpp>
#include <string>

// OpenGL
const int kOpenGLVersionMejor = 3;
const int kOpenGLVersionMinor = 3;

// window
const std::string kTitle = "Multiphase SPH Based Shallow Water Simulation";
const glm::vec3 kBackgroundColor = glm::vec3(1.0f, 1.0f, 1.0f);

// particle
const float kPointSize = 8.0f;
const glm::vec3 kBoundaryParticleColor = glm::vec3(0.95f, 0.3f, 0.3f);
const glm::vec3 kFluidParticleColor = glm::vec3(0.3f, 0.3f, 0.95f);
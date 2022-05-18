/**
 * @file constant.hpp 
 * @brief Declaration of constants
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <type.hpp>

// OpenGL
const int kOpenGLVersionMejor = 3;
const int kOpenGLVersionMinor = 3;

// window
const std::string kTitle = "Multiphase SPH Based Shallow Water Simulation";
const glm::vec3 kBackgroundColor = glm::vec3(1.0f, 1.0f, 1.0f);

// math
const float kPi = 3.141592653589;

// physics
const float kGravityAcceleration = 9.80665;

// particle
const float kPointSize = 8.0f;

// phase
const Phase kPhaseBoundary(2.0f, 998.29f, 30.0f, glm::vec3(0.95f, 0.3f, 0.3f));
const Phase kPhaseA(2.0f, 998.29f, 30.0f, glm::vec3(0.3f, 0.3f, 0.95f));
const Phase kPhaseB(2.0f, 998.29f, 30.0f, glm::vec3(0.3f, 0.95f, 0.3f));
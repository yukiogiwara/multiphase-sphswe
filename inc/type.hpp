/**
 * @file type.hpp
 * @brief Definition of types
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#pragma once

#include <glm/glm.hpp>

// kernel function pointer

using kernel = float (*)(float, float);
using gkernel = glm::vec2 (*)(const glm::vec2 &, float, float);
using lkernel = float (*)(float, float);

// ground function pointer

using ground = float (*)(const glm::vec2 &);

// particle attribute

enum ParticleAttribute {
    kBoundary,
    kFluid,
    kNumAttributes
};

// phase

struct Phase {
    float mass;
    float density;
    float kinematic_viscosity;
    glm::vec3 color;

    Phase(float mass, float density, float kinematic_viscosity, const glm::vec3 &color)
    : mass(mass), density(density), kinematic_viscosity(kinematic_viscosity), color(color)
    {}
};
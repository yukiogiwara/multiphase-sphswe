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
/**
 * @file type.hpp
 * @brief Definition of types
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#include <glm/glm.hpp>

// kernel function pointer

using kernel = float (*)(float, float);
using gkernel = glm::vec2 (*)(glm::vec2, float, float);
using lkernel = float (*)(float, float);

// ground function pointer

using ground = float (*)(glm::vec2);
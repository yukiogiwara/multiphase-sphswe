/**
 * @file terrain.hpp
 * @brief Definition of terrain
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#pragma once

#include <glm/glm.hpp>
#include "type.hpp"

/**
 * @brief terrain
 */
class Terrain {
public:
    Terrain(const ground &fn);
    ~Terrain();

    float GetHeight(const glm::vec2 &r);

public:

private:

private:
    ground fn_;
};
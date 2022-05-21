/**
 * @file terrain.hpp
 * @brief Definition of terrain
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#pragma once

#include <glm/glm.hpp>
#include "type.hpp"
#include "mesh.hpp"

/**
 * @brief terrain
 */
class Terrain {
public:
    Terrain(const ground &fn, const glm::vec2 &min_coord, const glm::vec2 &max_coord);
    ~Terrain();

    void Draw();

    float GetHeight(const glm::vec2 &r);

public:

private:
    void ConstructMesh(int num_div, const glm::vec2 &min_coord, const glm::vec2 &max_coord);

private:
    ground fn_;
    Mesh mesh_;
};
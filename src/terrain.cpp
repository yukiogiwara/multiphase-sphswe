/**
 * @file terrain.hpp
 * @brief Implementation of terrain
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#include "terrain.hpp"

/**
 * @brief constructor
 * @param[in] fn ground function
 * @param[in] min_coord minimum coordinate
 * @param[in] max_coord maximum coordinate
 */
Terrain::Terrain(const ground &fn, const glm::vec2 &min_coord, const glm::vec2 &max_coord) 
:fn_(fn) {
    ConstructMesh(32, min_coord, max_coord);
}

/**
 * @brief destructor
 */
Terrain::~Terrain() {

}

/**
 * @brief draw terrain
 */
void Terrain::Draw() {
    mesh.Draw();
}

/**
 * @brief calculate height at r
 * @param[in] r position
 * @return height
 */
float Terrain::GetHeight(const glm::vec2 &r) {
    float height = fn_(r);
    return height;
}

/**
 * @brief construct terrain mesh
 * @param[in] num_div number of division
 * @param[in] min_coord minimum coordinate
 * @param[in] max_corrd maximum coordinate
 */
void Terrain::ConstructMesh(int num_div, const glm::vec2 &min_coord, const glm::vec2 &max_coord) {
    glm::vec2 size = max_coord - min_coord;
    glm::ivec2 div(num_div);
    glm::vec2 d = size / glm::vec2(div);

    for(int z = 0; z < div[1]; z++) {
        for(int x = 0; x < div[0]; x++) {
            glm::vec2 r = min_coord + glm::vec2(x, z) * d;
            glm::vec3 p(r[0], GetHeight(r), r[1]);
            mesh.vertices_.push_back(p);
        }
    }

    for(int z = 0; z < div[1]-1; z++) {
        for(int x = 0; x < div[0]-1; x++) {
            unsigned int idx = z*div[0] + x;
            unsigned int idx_x = idx + 1;
            unsigned int idx_z = idx + div[0];
            mesh.indices_.push_back(idx);
            mesh.indices_.push_back(idx_z);
            mesh.indices_.push_back(idx_x);
        }
    }

    for(int z = div[1]-1; z > 0; z--) {
        for(int x = div[0]-1; x > 0; x--) {
            unsigned int idx = z*div[0] + x;
            unsigned int idx_x = idx - 1;
            unsigned int idx_z = idx - div[0];
            mesh.indices_.push_back(idx);
            mesh.indices_.push_back(idx_z);
            mesh.indices_.push_back(idx_x);
        }
    }

    mesh.SendDataToBuffer();
}
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
 */
Terrain::Terrain(const ground &fn) 
:fn_(fn) {

}

/**
 * @brief destructor
 */
Terrain::~Terrain() {

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
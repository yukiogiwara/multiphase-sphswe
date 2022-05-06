/**
 * @file nearest_neighbor.hpp
 * @brief Definition of nearest neighbor search
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * @brief find nearest neighbor particles
 */
class NearestNeighbor {
public:
    NearestNeighbor(const glm::vec2 &min_cord, const glm::vec2 &max_cord, float effective_radius, int num_particles);
    ~NearestNeighbor();

    void Register(const std::vector<glm::vec2> &ppos);

    void Search(const std::vector<glm::vec2> &ppos, std::vector<std::vector<int>> *neighbors, float radius);
    void Search(const glm::vec2 &pos, const std::vector<glm::vec2> &ppos, std::vector<int> *neighbors, float radius);

    void CheckParameters() const;

public:

private:
    void SearchNeighborsInCell(const glm::vec2 &pos, const std::vector<glm::vec2> &ppos, const glm::ivec2 index, std::vector<int> *neighbors, float radius);

    glm::ivec2 CalculateIndex(const glm::vec2 &pos) const;
    int CalculateHash(const glm::vec2 &pos) const;
    int CalculateHash(const glm::ivec2 &index) const;

private:
    glm::vec2 origin_;

    glm::vec2 cell_width_;
    glm::ivec2 num_cells_;
    int num_all_cells_;

    std::vector<int> sorted_index_;
    std::vector<int> grid_hash_;
    std::vector<int> starts_;
    std::vector<int> ends_;
};
/**
 * @file nearest_neighbor.cpp
 * @brief Implementation of nearest neighbor search
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#include "nearest_neighbor.hpp"

/**
 * @brief constructor
 * @param[in] min_cord minimum coordinate
 * @param[in] max_cord maximum coordinate
 * @param[in] effective_radius effective radius
 * @param[in] num_particles number of particles
 */
NearestNeighbor::NearestNeighbor(const glm::vec2 &min_cord, const glm::vec2 &max_cord, float effective_radius, int num_particles) 
: origin_(min_cord) {
    glm::vec2 world_size = max_cord - min_cord;
    float max_width = glm::max(world_size[0], world_size[1]);

    // calculate cell width
    float d = ceilf(logf(max_width / effective_radius) / logf(2.0f));
    float m = ceilf(powf(2.0f, d));
    float cell_width = max_width / m;
    cell_width_ = glm::vec2(cell_width);

    // calculate number of cells
    for(int i = 0; i < 2; i++) {
        d = ceilf(log(world_size[i] / cell_width) / logf(2.0f));
        num_cells_[i] = (int)ceilf(powf(2.0f, d));
    }
    num_all_cells_ = num_cells_[0] * num_cells_[1];

    // allocate memory
    sorted_index_.resize(num_particles);
    grid_hash_.resize(num_particles);
    starts_.resize(num_all_cells_);
    ends_.resize(num_all_cells_);

    CheckParameters();
}

/**
 * @brief destructor
 */
NearestNeighbor::~NearestNeighbor() {

}

/**
 * @brief register particles on cells
 * @param[in] ppos particles position
 */
void NearestNeighbor::Register(const std::vector<glm::vec2> &ppos) {
    // reset vectors
    std::fill(sorted_index_.begin(), sorted_index_.end(), 0);
    std::fill(grid_hash_.begin(), grid_hash_.end(), 0);
    std::fill(starts_.begin(), starts_.end(), 0xffffffff);
    std::fill(ends_.begin(), ends_.end(), 0xffffffff);

    for(int i = 0; i < ppos.size(); i++) {
        int hash = CalculateHash(ppos[i]);
        sorted_index_[i] = i;
        grid_hash_[i] = hash;
    }

    std::vector<std::pair<int, int>> hash_and_value;
    hash_and_value.resize(ppos.size());
    for(int i = 0; i < ppos.size(); i++) {
        hash_and_value[i].first = grid_hash_[i];
        hash_and_value[i].second = sorted_index_[i];
    }
    std::sort(hash_and_value.begin(), hash_and_value.end());
    for(int i = 0; i < ppos.size(); i++) {
        sorted_index_[i] = hash_and_value[i].second;
        grid_hash_[i] = hash_and_value[i].first;
    }

    for(int i = 0; i < ppos.size(); i++) {
        int hash = grid_hash_[i];

        if(i == 0) {
            starts_[hash] = i;
            ends_[hash] = i;
        } else {
            int prev_hash = grid_hash_[i-1];

            if(i == 0 || hash != prev_hash) {
                starts_[hash] = i;
                if(i > 0) {
                    ends_[prev_hash] = i;
                }
            }
            if(i == ppos.size()-1) {
                ends_[hash] = i+1;
            }
        }
    }
}

/**
 * @brief search all nearest neighbor particles
 * @param[in] ppos particles position 
 * @param[out] neighbors neighbor particles
 * @param[in] radius search radius
 */
void NearestNeighbor::Search(const std::vector<glm::vec2> &ppos, std::vector<std::vector<int>> *neighbors, float radius) {
    for(int i = 0; i < (int)ppos.size(); i++) {
        Search(ppos[i], ppos, &neighbors->at(i), radius);
    }
}

/**
 * @brief search nearest neighbor particles
 * @param[in] pos search position
 * @param[in] ppos particles position
 * @param[out] neighbors neighbor particles
 * @param[in] radius search radius
 */
void NearestNeighbor::Search(const glm::vec2 &pos, const std::vector<glm::vec2> &ppos, std::vector<int> *neighbors, float radius) {
    glm::ivec2 index = CalculateIndex(pos);
    glm::ivec2 range = glm::ivec2(radius / cell_width_) + 1;
    for(int j = -range[1]; j <= range[1]; j++) {
        for(int i = -range[0]; i <= range[0]; i++) {
            glm::ivec2 neighbor_cell_index = index + glm::ivec2(i, j);
            if(neighbor_cell_index[0] < 0 || num_cells_[0] <= neighbor_cell_index[0] || neighbor_cell_index[1] < 0 || num_cells_[1] <= neighbor_cell_index[1]) {
                continue;;
            }
            SearchNeighborsInCell(pos, ppos, neighbor_cell_index, neighbors, radius);
        }
    }
}

/**
 * @brief search neigbor particles in a cell
 * @param[in] pos search position
 * @param[in] ppos particles position
 * @param[in] index cell index
 * @param[out] neighbors neighbor particles
 * @param[in] radius search radius
 */
void NearestNeighbor::SearchNeighborsInCell(const glm::vec2 &pos, const std::vector<glm::vec2> &ppos, const glm::ivec2 index, std::vector<int> *neighbors, float radius) {
    int hash = CalculateHash(index);
    int start_index = starts_[hash];
    int end_index = ends_[hash];
    if(start_index == 0xffffffff) return;

    for(int j = start_index; j < end_index; j++) {
        int idx = sorted_index_[j];
        glm::vec2 r_ij = pos - ppos[idx];
        if(glm::length2(r_ij) <= radius * radius) {
            neighbors->push_back(idx);
        }
    }
}

/**
 * @brief calculate index
 * @param[in] pos position
 * @return index
 */
glm::ivec2 NearestNeighbor::CalculateIndex(const glm::vec2 &pos) const {
    glm::vec2 p = pos - origin_;
    glm::ivec2 index = glm::ivec2(p / cell_width_);
    index = glm::clamp(index, glm::ivec2(0, 0), num_cells_ - 1);
    return index;
}

/**
 * @brief calculate hash
 * @param[in] pos position
 * @return hash value
 */
int NearestNeighbor::CalculateHash(const glm::vec2 &pos) const {
    glm::ivec2 index = CalculateIndex(pos);
    int hash = CalculateHash(index);
    return hash;
}

/**
 * @brief calculate hash
 * @param[in] index index
 * @return hash value
 */
int NearestNeighbor::CalculateHash(const glm::ivec2 &index) const {
    int hash = index[1] * num_cells_[0] + index[0];
    return hash;
}

/**
 * @brief check parameters
 */
void NearestNeighbor::CheckParameters() const {
    std::cout << std::endl;
    std::cout << "NearestNeighbor:" << std::endl;
    std::cout << "origin_: "     << "( " << origin_[0]     << ", " << origin_[1]     << " )" << std::endl;
    std::cout << "cell_width_: " << "( " << cell_width_[0] << ", " << cell_width_[1] << " )" << std::endl;
    std::cout << "num_cells_: "  << "( " << num_cells_[0]  << ", " << num_cells_[1]  << " )" << std::endl;
    std::cout << "num_all_cells_: " << num_all_cells_ << std::endl;
}
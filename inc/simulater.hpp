/**
 * @file main.cpp 
 * @brief Definition of simulater
 * @author Yuki Ogiwara
 * @date 2022-05-05
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "utility.hpp"
#include "kernel.hpp"
#include "particle_system.hpp"
#include "nearest_neighbor.hpp"
#include "terrain.hpp"

/**
 * @brief shallow water simulation
 */
class Simulater {
public:
    Simulater(float scale);
    ~Simulater();

    void Evolve();

public:

private:
    void GenerateBoundary();
    void GenerateFluid(const glm::vec2 &min_pos, const glm::vec2 &max_pos);

    void CheckParameters() const;

private:
    // scale
    glm::vec2 min_cord_;
    glm::vec2 max_cord_;

    // simulation
    float delta_time_;
    float gravity_;

    // kernel
    int kernel_particles_;
    kernel kernel_;
    gkernel gkernel_;
    lkernel lkernel_;

    // fluid
    float mass_;
    float density_;
    float kinematic_viscosity_;

    // particle
    float effective_radius_;
    float particle_radius_;

    // boundary
    int num_boundary_layers_;
    glm::vec2 min_boundary_cord_;
    glm::vec2 max_boundary_cord_;

    std::unique_ptr<ParticleSystem> particles_;
    std::unique_ptr<NearestNeighbor> nn_;
    std::unique_ptr<Terrain> terrain_; 
};
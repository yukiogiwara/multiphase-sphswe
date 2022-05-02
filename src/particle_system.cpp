/**
 * @file particle_system.cpp
 * @brief Implementation of particle system
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#include "particle_system.hpp"

/**
 * @brief constructor
 */
ParticleSystem::ParticleSystem()
:num_particles_(0) {

}

/**
 * @brief destructor
 */
ParticleSystem::~ParticleSystem() {

}

/**
 * @brief get number of particles
 * @return number of particles
 */
int ParticleSystem::GetNumParticles() const {
    return num_particles_;
}

/**
 * @brief add a particle
 * @param[in] pos position
 * @param[in] vel velocity
 * @param[in] dens density
 * @param[in] attr attribute
 */
void ParticleSystem::AddParticle(const glm::vec2 &pos, const glm::vec2 &vel, float dens, int attr) {
    positions_.push_back(pos);
    velocities_.push_back(vel);
    densities_.push_back(dens);
    attributes_.push_back(attr);
    num_particles_++;
}
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
ParticleSystem::ParticleSystem() {
    num_particles_.resize(kNumAttributes, 0);
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
    int sum = std::accumulate(num_particles_.begin(), num_particles_.end(), 0);
    return sum;
}

/**
 * @brief get number of particles
 * @param[in] attr attribute
 * @return number of particles
 */
int ParticleSystem::GetNumParticles(ParticleAttribute attr) const {
    return num_particles_[attr];
}

/**
 * @brief add a particle
 * @param[in] pos position
 * @param[in] vel velocity
 * @param[in] dens density
 * @param[in] attr attribute
 */
void ParticleSystem::AddParticle(const glm::vec2 &pos, const glm::vec2 &vel, const glm::vec2 &acc, float mass, float dens, float height, ParticleAttribute attr) {
    positions_.push_back(pos);
    velocities_.push_back(vel);
    accelerations_.push_back(acc);
    masses_.push_back(mass);
    densities_.push_back(dens);
    heights_.push_back(height);
    attributes_.push_back(attr);
    num_particles_[attr]++;
}

/**
 * @brief check parameters
 */
void ParticleSystem::CheckParameters() const {
    std::cout << std::endl;
    std::cout << "ParticleSystem:" << std::endl;
    std::cout << "num_particles: " << GetNumParticles() << std::endl;
    for(int i = 0; i < kNumAttributes; i++) {
        std::cout << "num_particles" << "[" << i << "]: " << GetNumParticles(static_cast<ParticleAttribute>(i)) << std::endl;
    }
}
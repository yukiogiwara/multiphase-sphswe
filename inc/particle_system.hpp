/**
 * @file particle_system.hpp
 * @brief Definition of particle system
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

/**
 * @brief control particles
 */
class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    int GetNumParticles() const;

    void AddParticle(const glm::vec2 &pos, const glm::vec2 &vel, float dens, int attr);

public:
    std::vector<glm::vec2> positions_;
    std::vector<glm::vec2> velocities_;
    std::vector<float> densities_;
    std::vector<int> attributes_;

private:

private:
    int num_particles_;
};
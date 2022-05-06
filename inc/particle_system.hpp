/**
 * @file particle_system.hpp
 * @brief Definition of particle system
 * @author Yuki Ogiwara
 * @date 2022-05-03
 */

#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include "type.hpp"

/**
 * @brief control particles
 */
class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    int GetNumParticles() const;
    int GetNumParticles(ParticleAttribute attr) const;

    void AddParticle(const glm::vec2 &pos, const glm::vec2 &vel, float dens, ParticleAttribute attr);

    void CheckParameters() const;

public:
    std::vector<glm::vec2> positions_;
    std::vector<glm::vec2> velocities_;
    std::vector<float> densities_;
    std::vector<ParticleAttribute> attributes_;

private:

private:
    std::vector<int> num_particles_;
};
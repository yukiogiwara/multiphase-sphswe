/**
 * @file main.cpp 
 * @brief Implementation of simulater
 * @author Yuki Ogiwara
 * @date 2022-05-05
 */

#include "simulater.hpp"

/**
 * @brief constructor
 */
Simulater::Simulater(float scale) {
    // scale
    min_cord_ = glm::vec2(-scale/2.0f);
    max_cord_ = glm::vec2( scale/2.0f);

    // simulation
    delta_time_ = 0.002;
    gravity_ = 9.81;
    
    // kernel
    kernel_particles_ = 20;
    kernel_ = Poly6;
    gkernel_ = GradSpiky;
    lkernel_ = LaplaceSpiky;

    // terrain
    terrain_ = std::make_unique<Terrain>(Flat);

    // fluid
    mass_ = 2.0;
    density_ = 998.29;
    kinematic_viscosity_ = 30.0;

    // particle
    effective_radius_ = sqrtf(mass_ * kernel_particles_ / (glm::pi<float>() * density_));
    particle_radius_ = 0.5 * effective_radius_ * sqrtf(glm::pi<float>() / kernel_particles_);
    particles_ = std::make_unique<ParticleSystem>();

    // boundary
    num_boundary_layers_ = 3;
    min_boundary_cord_ = min_cord_ - num_boundary_layers_ * 2 * particle_radius_;
    max_boundary_cord_ = max_cord_ + num_boundary_layers_ * 2 * particle_radius_;

    // initialize particles
    GenerateBoundary();
    GenerateFluid(glm::vec2(-scale/4.0f), glm::vec2(scale/4.0f));

    // nearest neighbor
    nn_ = std::make_unique<NearestNeighbor>(min_boundary_cord_, max_boundary_cord_, effective_radius_, particles_->GetNumParticles());

    CheckParameters();
    std::cout << "num_particles: " << particles_->GetNumParticles() << std::endl;
}

/**
 * @brief destructor
 */
Simulater::~Simulater() {

}

/**
 * @brief time evolution
 */
void Simulater::Evolve() {

}

/**
 * @brief generate boundary particles
 */
void Simulater::GenerateBoundary() {
    for(int l = 0; l < num_boundary_layers_; l++) {
        glm::vec2 len = max_cord_ - min_cord_ + 4*l*particle_radius_;
        glm::ivec2 n = glm::ivec2(glm::ceil(len / (2*particle_radius_))) + 2;
        glm::vec2 d = (len + 2*particle_radius_) / glm::vec2(n-1);

        // along x-axis
        glm::vec2 min_pos = min_cord_ - (2*l+1)*particle_radius_;
        glm::vec2 max_pos = max_cord_ + (2*l+1)*particle_radius_;
        for(int xi = 0; xi < n[0]; xi++) {
            particles_->AddParticle(min_pos, glm::vec2(0.0f), 0.0f, 1);
            particles_->AddParticle(max_pos, glm::vec2(0.0f), 0.0f, 1);
            min_pos[0] += d[0];
            max_pos[0] -= d[0];
        }

        // along z-axis
        min_pos[1] += d[1];
        max_pos[1] -= d[1];
        for(int zi = 0; zi < n[1]-2; zi++) {
            particles_->AddParticle(min_pos, glm::vec2(0.0f), 0.0f, 1);
            particles_->AddParticle(max_pos, glm::vec2(0.0f), 0.0f, 1);
            min_pos[1] += d[1];
            max_pos[1] -= d[1];
        }
    }
}

/**
 * @brief generate fluid particles
 * @param[in] min_pos minimum position
 * @param[in] max_pos maximum position
 */
void Simulater::GenerateFluid(const glm::vec2 &min_pos, const glm::vec2 &max_pos) {
    glm::vec2 size = max_pos - min_pos;
    glm::ivec2 n = glm::ivec2(glm::floor(size / (2*particle_radius_)));
    glm::vec2 center = min_pos + size / 2.0f;
    glm::vec2 min_r = center - glm::vec2(n) * particle_radius_ + particle_radius_;
    glm::vec2 max_r = center + glm::vec2(n) * particle_radius_ - particle_radius_;

    for(float x = min_r[0]; x <= max_r[0]; x += 2*particle_radius_) {
        for(float z = min_r[1]; z <= max_r[1]; z += 2*particle_radius_) {
            glm::vec2 pos = glm::vec2(x, z);
            particles_->AddParticle(pos, glm::vec2(0.0f), 0.0f, 0);
        }
    }
}

/**
 * @brief check parameters
 */
void Simulater::CheckParameters() {
    std::cout << std::endl;
    std::cout << "Simulater:" << std::endl;
    std::cout << "min_cord_: " << "( " << min_cord_[0] << ", " << min_cord_[1] << " )" << std::endl;
    std::cout << "max_cord_: " << "( " << max_cord_[0] << ", " << max_cord_[1] << " )" << std::endl;
    std::cout << "delta_time_: "          << delta_time_          << std::endl;
    std::cout << "gravity_: "             << gravity_             << std::endl;
    std::cout << "kernel_particles_: "    << kernel_particles_    << std::endl;
    std::cout << "mass_: "                << mass_                << std::endl;
    std::cout << "density_: "             << density_             << std::endl;
    std::cout << "kinematic_viscosity_: " << kinematic_viscosity_ << std::endl;
    std::cout << "effective_radius_: "    << effective_radius_    << std::endl;
    std::cout << "particle_radius_: "     << particle_radius_     << std::endl;
    std::cout << "num_boundary_layers_: " << num_boundary_layers_ << std::endl;
    std::cout << "min_boundary_cord_: " << "( " << min_boundary_cord_[0] << ", " << min_boundary_cord_[1] << " )" << std::endl;
    std::cout << "max_boundary_cord_: " << "( " << max_boundary_cord_[0] << ", " << max_boundary_cord_[1] << " )" << std::endl;
}
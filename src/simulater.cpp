/**
 * @file simulater.cpp 
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
    lkernel_ = LaplaceViscosity;

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

    // terrain
    terrain_ = std::make_unique<Terrain>(Flat, min_boundary_cord_, max_boundary_cord_);

    // initialize particles
    GenerateBoundary();
    GenerateFluid(glm::vec2(-scale/2.0f), glm::vec2(scale/2.0f));

    // nearest neighbor
    neighbors_.resize(particles_->GetNumParticles());
    nn_ = std::make_unique<NearestNeighbor>(min_boundary_cord_, max_boundary_cord_, effective_radius_, particles_->GetNumParticles());

    // initialize height
    CalculateDensity();
    UpdateHeight();

    // generate buffers
    int n = particles_->GetNumParticles();
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, n*(sizeof(glm::vec2) + sizeof(float) + sizeof(glm::vec3)), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, n*sizeof(glm::vec2), particles_->positions_.data());
    glBufferSubData(GL_ARRAY_BUFFER, n*sizeof(glm::vec2), n*sizeof(float), particles_->heights_.data());
    glBufferSubData(GL_ARRAY_BUFFER, n*(sizeof(glm::vec2) + sizeof(float)), n*sizeof(glm::vec3), particles_->colors_.data());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(n*sizeof(glm::vec2)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(n*(sizeof(glm::vec2) + sizeof(float))));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    CheckParameters();
}

/**
 * @brief destructor
 */
Simulater::~Simulater() {
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

/**
 * @brief time evolution
 */
void Simulater::Evolve() {
    CalculateDensity();
    CalculateAcceleration();
    Integrate();
    UpdateHeight();
    UpdateBuffer();
}

/**
 * @brief draw particles
 * @param[in] attr particle attribute
 */
void Simulater::DrawParticles(ParticleAttribute attr) {
    switch(attr) {
        case kBoundary:
            glBindVertexArray(vao_);
            glDrawArrays(GL_POINTS, 0, particles_->GetNumParticles(kBoundary));
            glBindVertexArray(0);
            break;

        case kFluid:
            glBindVertexArray(vao_);
            glDrawArrays(GL_POINTS, particles_->GetNumParticles(kBoundary), particles_->GetNumParticles(kFluid));
            glBindVertexArray(0);
            break;

        default:
            break;
    }
}

/**
 * @brief draw terrain
 */
void Simulater::DrawTerrain() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    terrain_->Draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
 * @brief ImGui settings
 * @param[in] window window handler
 */
void Simulater::ImGui(GLFWwindow* window) {
    ImGui::SetNextTreeNodeOpen(true);
    if(ImGui::TreeNode("simulation")) {
        ImGui::InputFloat("kinematic viscosity", &kinematic_viscosity_, 0.2f, 1.0f);
        ImGui::TreePop();
    }
}

/**
 * @brief get delta time
 * @return delta time
 */
float Simulater::GetDeltaTime() const {
    return delta_time_;
}

/**
 * @brief calculate density of particles
 */
void Simulater::CalculateDensity() {
    nn_->Register(particles_->positions_);
    nn_->Search(particles_->positions_, &neighbors_, effective_radius_);
    for(int i = 0; i < particles_->GetNumParticles(); i++) {
        particles_->densities_[i] = Interpolate(particles_->masses_, particles_->densities_, particles_->densities_, particles_->positions_, i, neighbors_[i], kernel_, effective_radius_);
    }
}

/**
 * @brief calculate acceleration of particles
 */
void Simulater::CalculateAcceleration() {
    for(int i = 0; i < particles_->GetNumParticles(); i++) {
        if(particles_->attributes_[i] == kBoundary) continue;

        particles_->accelerations_[i] = glm::vec2(0.0f);

        particles_->accelerations_[i] += - gravity_ / density_ * InterpolateGradient(particles_->masses_, particles_->densities_, particles_->densities_, particles_->positions_, i, neighbors_[i], gkernel_, effective_radius_);
        particles_->accelerations_[i] += kinematic_viscosity_ / particles_->densities_[i] * InterpolateLaplacian(particles_->masses_, particles_->velocities_, particles_->densities_, particles_->positions_, i, neighbors_[i], lkernel_, effective_radius_);

        float d = 0.01f;
        glm::vec2 dx = glm::vec2(d, 0.0f);
        glm::vec2 dz = glm::vec2(0.0f, d);
        particles_->accelerations_[i][0] += - gravity_ * (terrain_->GetHeight(particles_->positions_[i]+dx) - terrain_->GetHeight(particles_->positions_[i]-dx)) / (2*d);
        particles_->accelerations_[i][1] += - gravity_ * (terrain_->GetHeight(particles_->positions_[i]+dz) - terrain_->GetHeight(particles_->positions_[i]-dz)) / (2*d);
    }
}

/**
 * @brief integrate particles
 */
void Simulater::Integrate() {
    for(int i = 0; i < particles_->GetNumParticles(); i++) {
        if(particles_->attributes_[i] == kBoundary) continue;

        // integrate velocity
        particles_->velocities_[i] += delta_time_ * particles_->accelerations_[i];

        // maximum velocity constraint
        float v_max = sqrtf(gravity_*particles_->densities_[i]/density_);
        float v_len = glm::length(particles_->velocities_[i]);
        if(v_len > v_max) particles_->velocities_[i] *= v_max / v_len;

        // integrate position
        particles_->positions_[i] += delta_time_ * particles_->velocities_[i];

        // boundary
        particles_->positions_[i] = glm::clamp(particles_->positions_[i], min_cord_, max_cord_);
    }
}

/**
 * @brief update height of particles
 */
void Simulater::UpdateHeight() {
    for(int i = 0; i < particles_->GetNumParticles(); i++) {
        if(particles_->attributes_[i] == kBoundary) continue;;
        particles_->heights_[i] = particles_->densities_[i] / density_ + terrain_->GetHeight(particles_->positions_[i]);
    }
}

/**
 * @brief update vbo
 */
void Simulater::UpdateBuffer() {
    int bn = particles_->GetNumParticles(kBoundary);
    int fn = particles_->GetNumParticles();
    int n = particles_->GetNumParticles();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);    
    glBufferSubData(GL_ARRAY_BUFFER, 0, n*sizeof(glm::vec2), particles_->positions_.data());
    glBufferSubData(GL_ARRAY_BUFFER, n*sizeof(glm::vec2), n*sizeof(float), particles_->heights_.data());
    // glBufferSubData(GL_ARRAY_BUFFER, n*(sizeof(glm::vec2) + sizeof(float)), n*sizeof(glm::vec3), particles_->colors_.data());
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
            particles_->AddParticle(min_pos, glm::vec2(0.0f), glm::vec2(0.0f), kBoundaryParticleColor, mass_, density_, 1.0f + terrain_->GetHeight(min_pos), kBoundary);
            particles_->AddParticle(max_pos, glm::vec2(0.0f), glm::vec2(0.0f), kBoundaryParticleColor, mass_, density_, 1.0f + terrain_->GetHeight(max_pos), kBoundary);
            min_pos[0] += d[0];
            max_pos[0] -= d[0];
        }

        // along z-axis
        for(int zi = 0; zi < n[1]; zi++) {
            particles_->AddParticle(min_pos, glm::vec2(0.0f), glm::vec2(0.0f), kBoundaryParticleColor, mass_, density_, 1.0f + terrain_->GetHeight(min_pos), kBoundary);
            particles_->AddParticle(max_pos, glm::vec2(0.0f), glm::vec2(0.0f), kBoundaryParticleColor, mass_, density_, 1.0f + terrain_->GetHeight(max_pos), kBoundary);
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
            particles_->AddParticle(pos, glm::vec2(0.5f), glm::vec2(0.0f), kFluidParticleColor, mass_, density_, 0.0f, kFluid);
        }
    }
}

/**
 * @brief check parameters
 */
void Simulater::CheckParameters() const {
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
    particles_->CheckParameters();
    nn_->CheckParameters();
}
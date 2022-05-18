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
    min_coord_ = glm::vec2(-scale/2.0f);
    max_coord_ = glm::vec2( scale/2.0f);

    // simulation
    dt_ = 0.002;
    
    // kernel
    kernel_particles_ = 20;
    kernel_ = Poly6;
    gkernel_ = GradSpiky;
    lkernel_ = LaplaceViscosity;

    // particle
    effective_rad_ = sqrtf(2.0 * kernel_particles_ / (glm::pi<float>() * 998.29));
    particle_rad_ = 0.5 * effective_rad_ * sqrtf(kPi / kernel_particles_);
    num_particles_.resize(kNumAttributes);

    // boundary
    num_boundary_layers_ = 3;
    min_boundary_coord_ = min_coord_ - num_boundary_layers_ * 2 * particle_rad_;
    max_boundary_coord_ = max_coord_ + num_boundary_layers_ * 2 * particle_rad_;

    // terrain
    terrain_ = std::make_unique<Terrain>(Flat, min_boundary_coord_, max_boundary_coord_);

    // phase
    phase_.push_back(kPhaseBoundary);
    phase_.push_back(kPhaseA);
    phase_.push_back(kPhaseB);

    // initialize
    GenerateBoundary();
    GenerateFluid(min_coord_, max_coord_);
    CalcMixture();
    CalcCol();

    // nearest neighbor
    int n = std::accumulate(num_particles_.begin(), num_particles_.end(), 0);
    neighbor_.resize(n);
    nn_ = std::make_unique<NearestNeighbor>(min_boundary_coord_, max_boundary_coord_, effective_rad_, n);
    nn_->Register(pos_);
    nn_->Search(pos_, &neighbor_, effective_rad_);

    // height
    CalcInterpDens();
    CalcHeight();

    // buffers
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, n*(sizeof(glm::vec2) + sizeof(float) + sizeof(glm::vec3)), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, n*sizeof(glm::vec2), pos_.data());
    glBufferSubData(GL_ARRAY_BUFFER, n*sizeof(glm::vec2), n*sizeof(float), height_.data());
    glBufferSubData(GL_ARRAY_BUFFER, n*(sizeof(glm::vec2) + sizeof(float)), n*sizeof(glm::vec3), col_.data());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(n*sizeof(glm::vec2)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(n*(sizeof(glm::vec2) + sizeof(float))));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

/**
 * @brief destructor
 */
Simulater::~Simulater() {
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

/**
 * @brief get delta time
 * @return dt
 */
float Simulater::GetDeltaTime() {
    return dt_;
}

/**
 * @brief time evolution
 */
void Simulater::Evolve() {
    CalcMixture();
    nn_->Register(pos_);
    nn_->Search(pos_, &neighbor_, effective_rad_);
    CalcInterpDens();
    CalcAcc();
    Integrate();
    CalcHeight();
    CalcCol();
    UpdateBuffer();
}

/**
 * @brief draw particles
 */
void Simulater::DrawParticles() {
    glBindVertexArray(vao_);
    glDrawArrays(GL_POINTS, 0, num_particles_[kBoundary]);
    glDrawArrays(GL_POINTS, num_particles_[kBoundary], num_particles_[kFluid]);
    glBindVertexArray(0);
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
 * @brief add particle
 * @param[in] pos position
 * @param[in] vel velocity
 * @param[in] acc acceleration
 * @param[in] col color
 * @param[in] mass mass
 * @param[in] visc kinematic viscosity
 * @param[in] dens density
 * @param[in] interp_dens interpolated density
 * @param[in] frac fractuation
 * @param[in] height height
 * @param[in] attr attribute
 */
void Simulater::AddParticle(const glm::vec2 &pos, const glm::vec2 &vel, const glm::vec2 &acc, const glm::vec3 &col, float mass, float visc, float dens, float interp_dens, const std::vector<float> &frac, float height, ParticleAttribute attr) {
    pos_.push_back(pos);
    vel_.push_back(vel);
    acc_.push_back(acc);
    col_.push_back(col);
    mass_.push_back(mass);
    visc_.push_back(visc);        
    dens_.push_back(dens);
    interp_dens_.push_back(interp_dens);
    frac_.push_back(frac);
    height_.push_back(height);
    attr_.push_back(attr);
    num_particles_[attr]++;
}

/**
 * @brief generate boundary particle
 */
void Simulater::GenerateBoundary() {
    for(int l = 0; l < num_boundary_layers_; l++) {
        glm::vec2 len = max_coord_ - min_coord_ + 4 * l * particle_rad_;
        glm::ivec2 n = glm::ivec2(glm::ceil(len / (2 * particle_rad_))) + 2;
        glm::vec2 d = (len + 2 * particle_rad_) / glm::vec2(n-1);

        // along x-axis
        glm::vec2 min_pos = min_coord_ - (2*l+1) * particle_rad_;
        glm::vec2 max_pos = max_coord_ + (2*l+1) * particle_rad_;
        for(int xi = 0; xi < n[0]; xi++) {
            AddParticle(min_pos, glm::vec2(0.0f), glm::vec2(0.0f), kPhaseBoundary.col, kPhaseBoundary.mass, kPhaseBoundary.visc, kPhaseBoundary.dens, kPhaseBoundary.dens, std::vector<float>({1.0f, 0.0f, 0.0f}), 1.0f + terrain_->GetHeight(min_pos), kBoundary);
            AddParticle(max_pos, glm::vec2(0.0f), glm::vec2(0.0f), kPhaseBoundary.col, kPhaseBoundary.mass, kPhaseBoundary.visc, kPhaseBoundary.dens, kPhaseBoundary.dens, std::vector<float>({1.0f, 0.0f, 0.0f}), 1.0f + terrain_->GetHeight(max_pos), kBoundary);
            min_pos[0] += d[0];
            max_pos[0] -= d[0];
        }

        // along z-axis
        for(int zi = 0; zi < n[1]; zi++) {
            AddParticle(min_pos, glm::vec2(0.0f), glm::vec2(0.0f), kPhaseBoundary.col, kPhaseBoundary.mass, kPhaseBoundary.visc, kPhaseBoundary.dens, kPhaseBoundary.dens, std::vector<float>({1.0f, 0.0f, 0.0f}), 1.0f + terrain_->GetHeight(min_pos), kBoundary);
            AddParticle(max_pos, glm::vec2(0.0f), glm::vec2(0.0f), kPhaseBoundary.col, kPhaseBoundary.mass, kPhaseBoundary.visc, kPhaseBoundary.dens, kPhaseBoundary.dens, std::vector<float>({1.0f, 0.0f, 0.0f}), 1.0f + terrain_->GetHeight(max_pos), kBoundary);
            min_pos[1] += d[1];
            max_pos[1] -= d[1];
        }
    }
}

/**
 * @brief generate fluid
 * @param[in] min_pos minimum position
 * @param[in] max_pos maximum position
 */
void Simulater::GenerateFluid(const glm::vec2 &min_pos, const glm::vec2 &max_pos) {
    glm::vec2 size = max_pos - min_pos;
    glm::ivec2 n = glm::ivec2(glm::floor(size / (2*particle_rad_)));
    glm::vec2 center = min_pos + size / 2.0f;
    glm::vec2 min_r = center - glm::vec2(n) * particle_rad_ + particle_rad_;
    glm::vec2 max_r = center + glm::vec2(n) * particle_rad_ - particle_rad_;

    for(float x = min_r[0]; x <= max_r[0]; x += 2*particle_rad_) {
        for(float z = min_r[1]; z <= max_r[1]; z += 2*particle_rad_) {
            glm::vec2 pos = glm::vec2(x, z);
            AddParticle(pos, glm::vec2(0.5f), glm::vec2(0.0f), kPhaseA.col, kPhaseA.mass, kPhaseA.visc, kPhaseA.dens, kPhaseA.dens, std::vector<float>({0.0f, 0.5f, 0.5f}), 1.0f + terrain_->GetHeight(pos), kFluid);
        }
    }
}

/**
 * @brief calculate color
 */
void Simulater::CalcCol() {
    for(int i = 0; i < pos_.size(); i++) {
        col_[i] = glm::vec3(0.0f);
        for(int k = 0; k < phase_.size(); k++)  {
            col_[i] += frac_[i][k] * phase_[k].col;
        }
    }
}

/**
 * @brief calculate mixture values
 */
void Simulater::CalcMixture() {
    for(int i = 0; i < pos_.size(); i++) {
        mass_[i] = 0.0f;
        visc_[i] = 0.0f;
        dens_[i] = 0.0f;
        for(int k = 0; k < phase_.size(); k++)  {
            mass_[i] += frac_[i][k] * phase_[k].mass;
            visc_[i] += frac_[i][k] * phase_[k].visc;
            dens_[i] += frac_[i][k] * phase_[k].dens;
        }
    }
}

/**
 * @brief calculate interpolated density
 */
void Simulater::CalcInterpDens() {
    std::vector<float> tmp(pos_.size(), 0.0f);
    for(int i = 0; i < pos_.size(); i++) {
        for(int j: neighbor_[i]) {
            glm::vec2 r_ij = pos_[i] - pos_[j];
            float r = glm::length(r_ij);
            tmp[i] += mass_[j] * kernel_(r, effective_rad_);
        }
    }
    for(int i = 0; i < pos_.size(); i++) {
        interp_dens_[i] = tmp[i];
    }
}

/**
 * @brief calculate acceleration
 */
void Simulater::CalcAcc() {
    for(int i = 0; i < pos_.size(); i++) {
        if(attr_[i] == kBoundary) continue;

        acc_[i] = glm::vec2(0.0f);

        for(int j: neighbor_[i]) {
            if(j == i) continue;
            glm::vec2 r_ij = pos_[i] - pos_[j];
            float r = glm::length(r_ij);
            acc_[i] += -kGravityAcceleration / dens_[i] * mass_[j] * gkernel_(r_ij, r, effective_rad_);
        }
        for(int j: neighbor_[i]) {
            glm::vec2 r_ij = pos_[i] - pos_[j];
            float r = glm::length(r_ij);
            acc_[i] += visc_[i] / interp_dens_[i] * mass_[j] * (vel_[j] - vel_[i]) / interp_dens_[j] * lkernel_(r, effective_rad_);
        }

        float d = 0.01f;
        glm::vec2 dx = glm::vec2(d, 0.0f);
        glm::vec2 dz = glm::vec2(0.0f, d);
        acc_[i][0] += -kGravityAcceleration * (terrain_->GetHeight(pos_[i]+dx) - terrain_->GetHeight(pos_[i]-dx)) / (2*d);
        acc_[i][1] += -kGravityAcceleration * (terrain_->GetHeight(pos_[i]+dz) - terrain_->GetHeight(pos_[i]-dz)) / (2*d);
    }
}

/**
 * @brief calculate height
 */
void Simulater::CalcHeight() {
    for(int i = 0; i < pos_.size(); i++) {
        if(attr_[i] == kBoundary) continue;
        height_[i] = interp_dens_[i] / dens_[i] + terrain_->GetHeight(pos_[i]);
    }
}

/**
 * @brief integrate
 */
void Simulater::Integrate() {
    for(int i = 0; i < pos_.size(); i++) {
        if(attr_[i] == kBoundary) continue;

        vel_[i] += dt_ * acc_[i];

        float v_max = sqrtf(kGravityAcceleration * interp_dens_[i] / dens_[i]);
        float v_len = glm::length(vel_[i]);
        if(v_len > v_max) vel_[i] *= v_max / v_len;

        pos_[i] += dt_ * vel_[i];

        pos_[i] = glm::clamp(pos_[i], min_coord_, max_coord_);
    }
}

/**
 * @brief update buffers
 */
void Simulater::UpdateBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // pos
    glBufferSubData(GL_ARRAY_BUFFER, 0, pos_.size() * sizeof(glm::vec2), pos_.data());
    // height
    glBufferSubData(GL_ARRAY_BUFFER, pos_.size() * sizeof(glm::vec2), pos_.size() * sizeof(float), height_.data());
    // col
    glBufferSubData(GL_ARRAY_BUFFER, pos_.size() * (sizeof(glm::vec2) + sizeof(float)), pos_.size() * sizeof(glm::vec3), col_.data());
}
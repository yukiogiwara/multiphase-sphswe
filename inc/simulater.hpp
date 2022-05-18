/**
 * @file simulater.hpp
 * @brief Definition of simulater
 * @author Yuki Ogiwara
 * @date 2022-05-05
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <numeric>
#include "constant.hpp"
#include "nearest_neighbor.hpp"
#include "terrain.hpp"
#include "kernel.hpp"
#include "utility.hpp"

/**
 * @brief shallow water simulation
 */
class Simulater {
public:
    Simulater(float scale);
    ~Simulater();

    float GetDeltaTime();

    void Evolve();
    void DrawParticles();
    void DrawTerrain();

public:

private:
    void AddParticle(const glm::vec2 &pos, const glm::vec2 &vel, const glm::vec2 &acc, const glm::vec3 &col, float mass, float visc, float dens, float interp_dens, const std::vector<float> &frac, float height, ParticleAttribute attr);
    void GenerateBoundary();
    void GenerateFluid(const glm::vec2 &min_pos, const glm::vec2 &max_pos);

    void CalcCol();
    void CalcMixture();
    void CalcInterpDens();
    void CalcAcc();
    void CalcHeight();
    void Integrate();

    void UpdateBuffer();

private:
    // scale
    glm::vec2 min_coord_;
    glm::vec2 max_coord_;

    // simulation
    float dt_;

    // kernel
    int kernel_particles_;
    kernel kernel_;
    gkernel gkernel_;
    lkernel lkernel_;

    // particles
    float effective_rad_;
    float particle_rad_;
    std::vector<Phase> phase_;
    std::vector<glm::vec2> pos_;
    std::vector<glm::vec2> vel_;
    std::vector<glm::vec2> acc_;
    std::vector<glm::vec3> col_;
    std::vector<float> mass_;
    std::vector<float> visc_;
    std::vector<float> dens_;
    std::vector<float> interp_dens_;
    std::vector<std::vector<float>> frac_;
    std::vector<float> height_;
    std::vector<ParticleAttribute> attr_;
    std::vector<int> num_particles_;

    // boundary
    int num_boundary_layers_;
    glm::vec2 min_boundary_coord_;
    glm::vec2 max_boundary_coord_;

    // buffers
    GLuint vao_;
    GLuint vbo_;

    // nearest neighbor
    std::vector<std::vector<int>> neighbor_;
    std::unique_ptr<NearestNeighbor> nn_;

    // terrain
    std::unique_ptr<Terrain> terrain_;
};
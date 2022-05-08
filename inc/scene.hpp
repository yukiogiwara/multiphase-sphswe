/**
 * @file scene.hpp
 * @brief Definition of scene
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <memory>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "light.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "simulater.hpp"

/**
 * @brief configuration of scene
 */
class Scene {
public:
    Scene(int window_width, int window_height);
    ~Scene();

    void ImGui(GLFWwindow* window);

public:
    float GetDeltaTime() const;

    void SetAspectRatio(float aspcet_ratio);

    void Draw();
    void Update();

private:
    void DrawParticles(const glm::mat4 &view, const glm::mat4 &projection);
    void DrawTerrain(const glm::mat4 &view, const glm::mat4 &projection);

private:
    std::unique_ptr<DirectionalLight> light_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<Shader> shader_;
    std::unique_ptr<Shader> terrain_shader_;
    std::unique_ptr<Simulater> simulater_;
};
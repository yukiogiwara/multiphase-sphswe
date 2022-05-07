/**
 * @file main.cpp 
 * @brief Definition of scene
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <memory>
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

public:
    float GetDeltaTime() const;

    void SetAspectRatio(float aspcet_ratio);

    void Draw();
    void Update();

private:

private:
    std::unique_ptr<DirectionalLight> light_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<Shader> shader_;
    std::unique_ptr<Simulater> simulater_;
};
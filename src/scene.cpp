/**
 * @file main.cpp 
 * @brief Implementation of scene
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#include "scene.hpp"

Scene::Scene(int window_width, int window_height) {
    // light
    glm::vec3 direction = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
    light_ = std::make_unique<DirectionalLight>(direction, ambient, diffuse, specular);

    // camera
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float fov = 45.0f;
    float aspect_ratio = (float)window_width / (float)window_height;
    float clliping_near = 0.1f;
    float clliping_far = 100.0f;
    camera_ = std::make_unique<Camera>(position, target, world_up, fov, aspect_ratio, clliping_near, clliping_far);

    // shader
    std::string vertex_path = "../shader/vertex.glsl";
    std::string fragment_path = "../shader/fragment.glsl";
    shader_ = std::make_unique<Shader>(vertex_path.c_str(), fragment_path.c_str());

    // simulater
    float scale = 4.0f;
    simulater_ = std::make_unique<Simulater>(scale);
}

Scene::~Scene() {

}

float Scene::GetDeltaTime() const {
    return simulater_->GetDeltaTime();
}

void Scene::SetAspectRatio(float aspect_ratio) {
    camera_->SetAspectRatio(aspect_ratio);
}

void Scene::Draw() {
    // simulater_->Draw();
}

void Scene::Update() {
    simulater_->Evolve();
}
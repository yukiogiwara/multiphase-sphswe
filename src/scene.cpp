/**
 * @file main.cpp 
 * @brief Implementation of scene
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#include "scene.hpp"

/**
 * @brief constructor
 * @param[in] window_width window width
 * @param[in] window_height window height
 */
Scene::Scene(int window_width, int window_height) {
    // light
    glm::vec3 direction = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
    light_ = std::make_unique<DirectionalLight>(direction, ambient, diffuse, specular);

    // camera
    glm::vec3 position = glm::vec3(0.0f, 5.0f, 5.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float fov = 45.0f;
    float aspect_ratio = (float)window_width / (float)window_height;
    float clliping_near = 0.1f;
    float clliping_far = 100.0f;
    camera_ = std::make_unique<Camera>(position, target, world_up, fov, aspect_ratio, clliping_near, clliping_far);

    // shader
    std::string vertex_path = "../shader/particle.vert";
    std::string fragment_path = "../shader/particle.frag";
    shader_ = std::make_unique<Shader>(vertex_path.c_str(), fragment_path.c_str());

    // simulater
    float scale = 4.0f;
    simulater_ = std::make_unique<Simulater>(scale);
}

/**
 * @brief destructor
 */
Scene::~Scene() {

}

/**
 * @brief ImGui settings
 * @param[in] window window handler
 */
void Scene::ImGui(GLFWwindow* window) {
    ImGui::Separator();
    light_->ImGui(window);
    ImGui::Separator();
    camera_->ImGui(window);
    ImGui::Separator();
    simulater_->ImGui(window);
}

/**
 * @brief get delta time
 * @return delta time
 */
float Scene::GetDeltaTime() const {
    return simulater_->GetDeltaTime();
}

/**
 * @brief set aspect ratio 
 * @param[in] aspect_ratio aspect ratio
 */
void Scene::SetAspectRatio(float aspect_ratio) {
    camera_->SetAspectRatio(aspect_ratio);
}

/**
 * @brief draw objects in scene
 */
void Scene::Draw() {
    glm::mat4 view = camera_->GenViewMatrix();
    glm::mat4 projection = camera_->GenProjectionMatrix();

    DrawParticles(view, projection);    
}

/**
 * @brief draw particles
 * @param[in] view view matrix
 * @param[in] projection projection matrix
 */
void Scene::DrawParticles(const glm::mat4 &view, const glm::mat4 &projection) {
    shader_->Use();
    glm::mat4 model(1.0f);
    shader_->SetMat4("model", model);
    shader_->SetMat4("view", view);
    shader_->SetMat4("projection", projection);
    simulater_->Draw(kBoundary);
    simulater_->Draw(kFluid);
}

/**
 * @brief update scene
 */
void Scene::Update() {
    simulater_->Evolve();
}
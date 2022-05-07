/**
 * @file light.hpp 
 * @brief Implementation of light
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#include "light.hpp"

/**
 * @brief constructor
 * @param[in] dir direction
 * @param[in] amb ambient
 * @param[in] dif diffuse
 * @param[in] spe specular
 */
DirectionalLight::DirectionalLight(const glm::vec3 &dir, const glm::vec3 &amb, const glm::vec3 &dif, const glm::vec3 &spe) {
    direction_ = glm::normalize(dir);
    ambient_ = amb;
    diffuse_ = dif;
    specular_ = spe;
}

/**
 * @brief destructor
 */
DirectionalLight::~DirectionalLight() {

}

/**
 * @brief ImGui settings
 * @param[in] window window handler
 */
void DirectionalLight::ImGui(GLFWwindow* window) {
    ImGui::SetNextTreeNodeOpen(true);
    if(ImGui::TreeNode("light")) {
        if(ImGui::InputFloat3("direction", glm::value_ptr(direction_))) {
            direction_ = glm::normalize(direction_);
        }
        ImGui::InputFloat3("ambient", glm::value_ptr(ambient_));
        ImGui::InputFloat3("diffuse", glm::value_ptr(diffuse_));
        ImGui::InputFloat3("specular", glm::value_ptr(specular_));
        ImGui::TreePop();
    }
}

/**
 * @brief get direction
 */
glm::vec3 DirectionalLight::GetDirection() const {
    return direction_;
}

/**
 * @brief get ambient
 */
glm::vec3 DirectionalLight::GetAmbient() const {
    return ambient_;
}

/**
 * @brief get diffuse
 */
glm::vec3 DirectionalLight::GetDiffuse() const {
    return diffuse_;
}

/**
 * @brief get specular
 */
glm::vec3 DirectionalLight::GetSpecular() const {
    return specular_;
}
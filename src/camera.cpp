/**
 * @file camera.cpp
 * @brief Implementation of camera
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#include "camera.hpp"

/**
 * @brief constructor
 * @param[in] position position
 * @param[in] target target
 * @param[in] world_up upward direction
 * @param[in] fov field of view
 * @param[in] clliping_near distance to near clliping plane
 * @param[in] clliping_far distance to far clliping plane
 */
Camera::Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &world_up, float fov, float aspect_ratio, float clliping_near, float clliping_far)
: position_(position), target_(target), world_up_(world_up), fov_(fov), aspect_ratio_(aspect_ratio), clliping_near_(clliping_near), clliping_far_(clliping_far) {
    Update();
}

/**
 * @brief generate view matrix
 * @return view matrix
 */
glm::mat4 Camera::GenViewMatrix() {
    glm::mat4 view = glm::lookAt(position_, target_, world_up_);
    return view;
}

/**
 * @brief generate projection matrix
 * @return projection matrix
 */
glm::mat4 Camera::GenProjectionMatrix() {
    glm::mat4 projection = glm::perspective(fov_, aspect_ratio_, clliping_near_, clliping_far_);
    return projection;
}

/**
 * @brief set camera position
 * @param[in] position position
 */
void Camera::SetPosition(glm::vec3 position) {
    position_ = position;
    Update();
}

/**
 * @brief set aspect ratio
 * @param[in] aspect_ratio aspect ratio
 */
void Camera::SetAspectRatio(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
}

/**
 * @brief update camera vectors
 */
void Camera::Update() {
    front_ = glm::normalize(target_ - position_);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_    = glm::normalize(glm::cross(right_, front_));
}
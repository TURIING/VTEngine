#include "core/common/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


Camera::Camera() {
    this->updateViewMatrix();
}

glm::vec3 Camera::GetUp() const {
    return glm::rotate(this->GetDirection(), m_up);
}

glm::quat Camera::GetDirection() const {
    return glm::quat { glm::vec3(-m_pitch, -m_yaw, 0.0f) };
}

void Camera::SetWindowSize(const Size &size) {
    m_projection = glm::perspectiveRH_ZO(m_fov, size.width / static_cast<float>(size.height), m_near, m_far);
}

void Camera::updateViewMatrix() {
    m_position = m_focus - this->GetForward() * m_distance;

    m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(this->GetDirection());
    m_viewMatrix = glm::inverse(m_viewMatrix);
}

glm::vec3 Camera::GetForward() const {
    return glm::rotate(this->GetDirection(), m_forward);
}

void Camera::SetDistance(float _offset) {
    m_distance -= _offset;
    this->updateViewMatrix();
    LOG_INFO("camera move");
}

glm::vec3 Camera::GetRight() const {
    return glm::rotate(this->GetDirection(), m_right);
}

// 处理鼠标移动事件
void Camera::OnMouseMove(double x, double y, MouseButton button) {
    if(button != MouseButton::Right) {
        m_lastCursorPos = glm::vec2(x, y);
        return;
    }

    glm::vec2 pos { x, y };
    glm::vec2 delta = (pos - m_lastCursorPos) * 0.004f;

    const auto sign = this->GetUp().y < 0 ? -1.0f : 1.0f;
    m_yaw += sign * delta.x * m_rotateSpeed;
    m_pitch += delta.y * m_rotateSpeed;

    this->updateViewMatrix();
    m_lastCursorPos = pos;
}

void Camera::OnMouseWheelScroll(double delta) {
    this->SetDistance(delta * 0.1f);
    this->updateViewMatrix();
}




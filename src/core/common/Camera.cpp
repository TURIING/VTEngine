#include "core/common/Camera.h"

glm::vec3 Camera::getUp() const {
    return glm::rotate(this->getDirection(), m_up);
}

glm::quat Camera::getDirection() const {
    return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
}

void Camera::setWindowSize(Size _size) {
    m_projection = glm::perspective(m_fov, _size.width / _size.height, m_near, m_far);
}

void Camera::updateViewMatrix() {
    m_position = m_focus - this->getForward() * m_distance;

    m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(this->getDirection());
    m_viewMatrix = glm::inverse(m_viewMatrix);
}

glm::vec3 Camera::getForward() const {
    return glm::rotate(this->getDirection(), m_forward);
}

void Camera::setDistance(float _offset) {
    m_distance -= _offset;
    this->updateViewMatrix();
}

glm::vec3 Camera::getRight() const {
    return glm::rotate(this->getDirection(), m_right);
}

// 处理鼠标移动事件
void Camera::onMouseMove(double _x, double _y) {
    glm::vec2 pos { _x, _y };
    glm::vec2 delta = (pos - m_lastCursorPos) * 0.004f;

    const auto sign = this->getUp().y < 0 ? -1.0f : 1.0f;
    m_yaw += sign * delta.x * m_rotateSpeed;
    m_pitch += delta.y * m_rotateSpeed;

    this->updateViewMatrix();
    m_lastCursorPos = pos;
}

void Camera::onMouseWheelScroll(double _delta) {
    this->setDistance(_delta * 0.5f);
    this->updateViewMatrix();
}

// 分发事件
void Camera::dispatch(Event _event, EventParam _param) {
    switch (_event) {
        case Event::MOUSE_MOVE: {
            const auto [button, x, y] = std::get<MouseState>(_param);
            if(button == MOUSE_BUTTON::Right) {
                this->onMouseMove(x, y);
            }
            else {
                m_lastCursorPos = glm::vec2 { x, y };
            }
            break;
        }
        case Event::MOUSE_WHEEL:    this->onMouseWheelScroll(std::get<double>(_param));   break;
        default:                    LOG(FATAL) << "Parameter error.";
    }
}

Camera::Camera() {
    this->updateViewMatrix();
}



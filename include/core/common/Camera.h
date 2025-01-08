#ifndef CAMERA_H
#define CAMERA_H

#include "common/common.h"

/**
 * 摄像机移动的方向
 */
enum class CAMERA_DIRECTION {
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float FOV = 45.0f;

class Camera {
public:
    Camera();
    [[nodiscard]] float getFov() const { return m_fov; }
    [[nodiscard]] glm::vec3 getUp() const;
    [[nodiscard]] glm::vec3 getRight() const;
    [[nodiscard]] glm::vec3 getForward() const;
    [[nodiscard]] glm::quat getDirection() const;
    [[nodiscard]] glm::mat4 getProjection() const { return m_projection; }
    [[nodiscard]] glm::mat4 getViewMatrix() const { return m_viewMatrix; }
    [[nodiscard]] glm::vec3 getPosition() { return m_position; }
    void setWindowSize(Size _size);
    void setDistance(float _offset);
    void dispatch(Event _event, EventParam _param);

private:
    void updateViewMatrix();
    void onMouseMove(double _x, double _y);
    void onMouseWheelScroll(double _delta);

private:
    glm::vec3 m_position = { 0, 0, 3 };
    glm::mat4 m_projection = glm::mat4(1.0f);
    glm::mat4 m_viewMatrix;
    glm::vec3 m_focus = { 0, 0, 0 };
    float m_distance = 5.0f;
    //float m_aspect = 1.3f;
    float m_fov = 45.0f;
    float m_near = 0.1f;
    float m_far = 100.0f;
    float m_pitch = 0;
    float m_yaw = 0;
    glm::vec2 m_lastCursorPos = { 0, 0 };
    const glm::vec3 m_right = { 1.0, 0, 0};
    const glm::vec3 m_up = { 0, 1, 0 };
    const glm::vec3 m_forward = { 0, 0, -1 };
    const float m_rotateSpeed = 2;
};

#endif
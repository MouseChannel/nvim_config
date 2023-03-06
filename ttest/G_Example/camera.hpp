#pragma once
#include "FrameWork/Base/baseHeader.h"

enum class CAMERA_MOVE { MOVE_LEFT, MOVE_RIGHT, MOVE_FRONT, MOVE_BACK };
class Camera {
private:
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  float m_speed;

  float m_pitch;
  float m_yaw;
  float m_sensitivity;

  float m_xpos;
  float m_ypos;
  bool m_firstMove;

  glm::mat4 m_vMatrix;
  glm::mat4 m_pMatrx;

public:
  Camera() {
    m_position = glm::vec3(1.0f);
    m_front = glm::vec3(1.0f);
    m_up = glm::vec3(1.0f);
    m_speed = 0.01f;

    m_pitch = 0.0f;
    m_yaw = -90.0f;
    m_sensitivity = 0.1f;

    m_xpos = 0;
    m_ypos = 0;
    m_firstMove = true;

    m_vMatrix = glm::mat4(1.0f);
  }
  ~Camera() {}
  void lookAt(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up);
  void update();

  glm::mat4 getViewMatrix();

  glm::mat4 getProjectMatrix();

  void setSpeed(float _speed) { m_speed = _speed; }

  void move(CAMERA_MOVE _mode);

  void pitch(float _yOffset);
  void yaw(float _xOffset);
  void setSentitivity(float _s);
  void onMouseMove(double _xpos, double _ypos);

  void setPerpective(float angle, float ratio, float near, float far);
};

inline void Camera::lookAt(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up) {
  m_position = _pos;
  m_front = glm::normalize(_front);
  m_up = _up;

  m_vMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

inline void Camera::update() {
//   std::cout << "camera update" << std::endl;
  m_vMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

inline glm::mat4 Camera::getViewMatrix() { return m_vMatrix; }

inline glm::mat4 Camera::getProjectMatrix() { return m_pMatrx; }

inline void Camera::move(CAMERA_MOVE _mode) {
  // std::cout<<(int)_mode<<std::endl;
  switch (_mode) {
  case CAMERA_MOVE::MOVE_LEFT:
    m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
    break;
  case CAMERA_MOVE::MOVE_RIGHT:
    m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed;
    break;
  case CAMERA_MOVE::MOVE_FRONT:
    m_position += m_speed * m_front;
    break;
  case CAMERA_MOVE::MOVE_BACK:
    m_position -= m_speed * m_front;
    break;
  default:
    break;
  }

  update();
}

inline void Camera::pitch(float _yOffset) {
  m_pitch += _yOffset * m_sensitivity;

  if (m_pitch >= 89.0f) {
    m_pitch = 89.0f;
  }

  if (m_pitch <= -89.0f) {
    m_pitch = -89.0f;
  }

  m_front.y = sin(glm::radians(m_pitch));
  m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

  m_front = glm::normalize(m_front);
  update();
}
inline void Camera::yaw(float _xOffset) {
  m_yaw += _xOffset * m_sensitivity;

  m_front.y = sin(glm::radians(m_pitch));
  m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

  m_front = glm::normalize(m_front);
  update();
}
inline void Camera::setSentitivity(float _s) { m_sensitivity = _s; }

inline void Camera::setPerpective(float angle, float ratio, float near1,
                                  float far1) {
  m_pMatrx = glm::perspective(glm::radians(angle), ratio, near1, far1);
}

inline void Camera::onMouseMove(double _xpos, double _ypos) {
  if (m_firstMove) {
    m_xpos = _xpos;
    m_ypos = _ypos;
    m_firstMove = false;
    return;
  }

  float _xOffset = _xpos - m_xpos;
  float _yOffset = -(_ypos - m_ypos);

  m_xpos = _xpos;
  m_ypos = _ypos;

  pitch(_yOffset);
  yaw(_xOffset);
}

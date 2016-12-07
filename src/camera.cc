#include <fstream>
#include <iostream>
#include "camera.h"

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

glm::mat4 Camera::get_view_matrix() const
{
  return lookAt();
}

glm::mat4 
Camera::lookAt() const{
  glm::mat4 matrix;
  glm::vec3 Z = glm::normalize(eye_ - center_);
  glm::vec3 Y = up_;
  glm::vec3 X = glm::cross(Y, Z);
  Y = glm::normalize(glm::cross(Z, X));
  X = glm::normalize(X);
  matrix[0][0] = X[0];
  matrix[1][0] = X[1];
  matrix[2][0] = X[2];
  matrix[3][0] = -glm::dot(X, eye_);
  matrix[0][1] = Y[0];
  matrix[1][1] = Y[1];
  matrix[2][1] = Y[2];
  matrix[3][1] = -glm::dot(Y, eye_);
  matrix[0][2] = Z[0];
  matrix[1][2] = Z[1];
  matrix[2][2] = Z[2];
  matrix[3][2] = -glm::dot(Z, eye_);
  matrix[0][3] = 0;
  matrix[1][3] = 0;
  matrix[2][3] = 0;
  matrix[3][3] = 1;
  return matrix;
}

void
Camera::switchMode() {
  (fps_mode) ? fps_mode = false : fps_mode = true;
}

void
Camera::setMouseMovement(bool event) {
  mouse_pressed = event;
}

bool
Camera::getMouseMovement() {
  return mouse_pressed;
}

void
Camera::setMousePos(double mouse_x, double mouse_y) {
  saved_mouse_x = mouse_x;
  saved_mouse_y = mouse_y;
}

void 
Camera::rotateMouse(double mouse_x, double mouse_y) {
  double delta_x = mouse_x - saved_mouse_x;
  double delta_y = mouse_y - saved_mouse_y;
  setMousePos(mouse_x, mouse_y);
  if(fps_mode) {
    look_ = glm::rotate(look_, (float)delta_x * rotation_speed, up_);
    tangent_ = glm::normalize(glm::cross(look_, up_));
    look_ = glm::rotate(look_, (float)delta_y * rotation_speed, tangent_);
    up_ = glm::normalize(glm::cross(look_, -tangent_));
    center_ = look_ * camera_distance_ + eye_;
  }
  else {
    eye_ = glm::rotate(eye_, (float)delta_x * rotation_speed, up_);
    look_ = center_ - eye_;
    tangent_ = glm::normalize(glm::cross(look_, up_));
    eye_ = glm::rotate(eye_, (float)delta_y * rotation_speed, tangent_);
    look_ = center_ - eye_;
    up_ = glm::normalize(glm::cross(look_, -tangent_));
  }
  look_ = glm::normalize(look_);
}

void
Camera::roll(bool left) {
  if(left)
    up_ = glm::normalize(glm::rotate(up_, -roll_speed, look_));
  else
    up_ = glm::normalize(glm::rotate(up_, roll_speed, look_));
  tangent_ = glm::normalize(glm::cross(look_, up_));
}
void
Camera::zoomMouse(double mouse_y) {
  glm::vec3 scale = look_ * (zoom_speed * (float)(saved_mouse_y - mouse_y));
  eye_ += scale;
  camera_distance_ = glm::length(center_ - eye_);
  setMousePos(0.0, mouse_y);
}

void
Camera::zoomKey(bool forward) {
  glm::vec3 scale = look_ * zoom_speed;
  if(fps_mode) {
    if(forward) {
      eye_ += scale;
      center_ += scale;
    }
    else {
      eye_ -= scale;
      center_ -= scale;
    }
  }
  else {
    float distance = glm::length(center_ - eye_);
    if(forward) {
      camera_distance_ = distance - zoom_speed;
      eye_ += scale;
    }
    else {
      camera_distance_ = distance + zoom_speed;
      eye_ -= scale;
    }
  }
}

void
Camera::panHor(bool left) {
  if(left) {
    center_ -= tangent_ * pan_speed;
    eye_ -= tangent_ * pan_speed;
  }
  else {
    center_ += tangent_ * pan_speed;
    eye_ += tangent_ * pan_speed;
  }
}

void
Camera::panVert(bool up) {
  if(up) {
    center_ += up_ * pan_speed;
    eye_ += up_ * pan_speed;
  }
  else {
    center_ -= up_ * pan_speed;
    eye_ -= up_ * pan_speed;
  }
}
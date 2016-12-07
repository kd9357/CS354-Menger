#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera {
public:
	glm::mat4 get_view_matrix() const;
  glm::mat4 lookAt() const;
  void switchMode();
  void setMouseMovement(bool event);
  bool getMouseMovement();
  void setMousePos(double mouse_x, double mouse_y);
  void rotateMouse(double mouse_x, double mouse_y);
  void roll(bool left);
  void zoomMouse(double mouse_y);
  void zoomKey(bool forward);
  void panHor(bool left);
  void panVert(bool up);
private:
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
	glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
	// Note: you may need additional member variables
  bool fps_mode = false;
  bool mouse_pressed = true; //Indicates if the mouse was just clicked
  double saved_mouse_x;
  double saved_mouse_y;
  glm::vec3 center_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 tangent_ = glm::normalize(glm::cross(look_, up_));
};

#endif

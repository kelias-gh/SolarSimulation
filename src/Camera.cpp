#include "Camera.h"

void Camera::createProjection(float FOV, float windowWidth, float windowHeight, float nearZ, float farZ) {
	ProjectionMatrix = Math::PerspectiveProjection(FOV, windowWidth, windowHeight, nearZ, farZ);
}

void Camera::Draw() {
	viewMatrix = Math::LookAtView(transform.position, transform.position + transform.forward, transform.up);
}

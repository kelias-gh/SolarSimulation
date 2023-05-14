#include "Camera.h"

void Camera::CreateCamera(float FOV, float windowWidth, float windowHeight, float nearZ, float farZ) {
	ViewAndProjectionMatrixBuffer = new ConstantBuffer;

	ViewAndProjectionMatrixBuffer->data = &ViewAndProjectionMatrix;
	ViewAndProjectionMatrixBuffer->size = sizeof(Math::Matrix4x4);

	ViewAndProjectionMatrixBuffer->Create();

	projectionMatrix = Math::PerspectiveProjection(FOV, windowWidth, windowHeight, nearZ, farZ);
}

Camera::~Camera() {

}

void Camera::calculateVectors() {
	forward.x = cos(rotation.x) * sin(rotation.y);
	forward.y = -sin(rotation.x);
	forward.z = cos(rotation.x) * cos(rotation.y);
	forward = Math::normalize(forward);

	right = Math::CrossProduct(forward, defaultDown);
	right = Math::normalize(right);

	up = Math::CrossProduct(forward, right);
	up = Math::normalize(up);
}

void Camera::LookAt(Math::Vector3 lookAtPos)
{
	if (lookAtPos.x == position.x && lookAtPos.y == position.y && lookAtPos.z == position.z)
		return;

	lookAtPos.x = position.x - lookAtPos.x;
	lookAtPos.y = position.y - lookAtPos.y;
	lookAtPos.z = position.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		yaw = atan(lookAtPos.y / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += PI;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	if (yaw > 360.0f || yaw < -360.0f)
	{
		yaw = 0;
	}

	rotation = Math::Vector3(pitch, yaw, rotation.z);
}

void Camera::Update() {
	calculateVectors();
	viewMatrix = Math::LookAtView(position, position + forward, up);

	ViewAndProjectionMatrix = viewMatrix * projectionMatrix;
	ViewAndProjectionMatrix = Math::Transpose(ViewAndProjectionMatrix);
	
	ViewAndProjectionMatrixBuffer->Update();

	GetDeviceContext()->VSSetConstantBuffers(1, 1, &ViewAndProjectionMatrixBuffer->buffer);
}

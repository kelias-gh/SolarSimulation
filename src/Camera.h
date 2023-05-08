#pragma once
#include "AOMMath.h"
#include "Transform.h"

class Camera {
public:
	Transform transform;

	Math::Matrix4x4 ProjectionMatrix;
	Math::Matrix4x4 viewMatrix;

	void createProjection(float FOV, float windowWidth, float windowHeight, float nearZ, float farZ);
	void Draw();
};
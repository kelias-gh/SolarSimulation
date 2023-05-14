#pragma once
#include "AOMMath.h"
#include "Transform.h"

struct Camera {
	Math::Vector3 position = Math::Vector3(0, 0, 2);
	Math::Vector3 rotation = Math::Vector3(0, 0, 0);
	Math::Vector3 scale = Math::Vector3(1, 1, 1);

	Math::Vector3  right = defaultRight;
	Math::Vector3  up = defaultUp;
	Math::Vector3  forward = { 0,0,1 };

	Math::Vector3  defaultRight = { 1, 0, 0 };
	Math::Vector3  defaultLeft = { -1, 0, 0 };
	Math::Vector3  defaultUp = { 0, 1, 0 };
	Math::Vector3  defaultDown = { 0, -1, 0 };
	Math::Vector3  defaultForward = { 0, 0, 1 };
	Math::Vector3  defaultBackward = { 0, 0, -1 };

	ConstantBuffer* ViewAndProjectionMatrixBuffer;

	Math::Matrix4x4 ViewAndProjectionMatrix;

	Math::Matrix4x4 projectionMatrix;
	Math::Matrix4x4 viewMatrix;

	void CreateCamera(float FOV, float windowWidth, float windowHeight, float nearZ, float farZ);
	~Camera();

	void calculateVectors();
	void LookAt(Math::Vector3 lookAtPos);
	void Update();
};
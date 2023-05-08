#pragma once

#include <string>
#include "AOMMath.h"

struct Transform {
	Math::Vector3 position = Math::Vector3(0,0,2);
	Math::Vector3 rotation = Math::Vector3(0, 0, 0);
	Math::Vector3 scale = Math::Vector3(1, 1, 1);

	Math::Vector3  right = defaultRight;
	//XMVECTOR left =	-right;
	Math::Vector3  up = defaultUp;
	//XMVECTOR down =	-up;
	Math::Vector3  forward = {0,0,1};

	Math::Vector3  defaultRight =		{1, 0, 0};
	Math::Vector3  defaultLeft =		{-1, 0, 0};
	Math::Vector3  defaultUp =		{0, 1, 0};
	Math::Vector3  defaultDown =		{0, -1, 0};
	Math::Vector3  defaultForward =	{0, 0, 1};
	Math::Vector3  defaultBackward =	{0, 0, -1};
	
	Math::Matrix4x4 worldMatrix;

	void calculateVectors();
	void LookAt(Math::Vector3 lookAtPos);
};
#pragma once

struct MatrixBuffer
{
    //wvp (world - view - projection) matrices
	Math::Matrix4x4 wvp;
	Math::Matrix4x4 world;
};

struct Light
{
	Math::Vector3 camPos;
	float padding1;
	Math::Vector3 dir;
	float padding2;
	Math::Vector4 diffuse;
};
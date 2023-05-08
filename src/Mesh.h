#pragma once
#include "AOMMath.h"
#include <vector>

struct Vertex
{
	Math::Vector3 pos;
	Math::Vector2 texCoord;
	Math::Vector3 normals;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	void RecalculateNormals();
};
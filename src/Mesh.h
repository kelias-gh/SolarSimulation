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
	std::vector<unsigned int> indices;

	void RecalculateNormals();
};

void DrawCircle(float radius, int points, Mesh& const mesh);

//https://www.artstation.com/blogs/briz/oKpM/perfectly-remapping-a-cube-to-a-sphere-houdini

void DrawSphere(int radius, int longitudePoints, int latitudePoints, Mesh& const mesh);
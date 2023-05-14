#pragma once
#include "d3d11_renderer.h"
#include "AOMMath.h"
#include "Texture2D.h"
#include <vector>

struct Vertex
{
	Math::Vector3 pos;
	Math::Vector2 texCoord;
	Math::Vector3 normals;
};

struct Mesh {
	VertexBuffer* vertexBuffer;
	Buffer* indexBuffer;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void CreateMesh();
	~Mesh();

	void Update();

	void RecalculateNormals();
};

struct Material
{
	Math::Vector4 ambient;
	Math::Vector4 diffuse;

	Texture2D mainTexture;
	Shader vertexShader;
	Shader pixelShader;

	ConstantBuffer* MaterialBuffer;

	void CreateMaterial();

	void Update();
};

void DrawCircle(float radius, int points, Mesh& const mesh);

//https://www.artstation.com/blogs/briz/oKpM/perfectly-remapping-a-cube-to-a-sphere-houdini

//void DrawSphere(int radius, int longitudePoints, int latitudePoints, Mesh& const mesh);
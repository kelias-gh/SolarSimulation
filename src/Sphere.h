#include "AOMMath.h"
#include "Mesh.h"
#include "d3d11_renderer.h"

struct Sphere {
	void CreateSphere(int radius, int longitude, int latitude);
	void Update();
	Mesh mesh;
};

void DrawSphere(int radius, int longitudePoints, int latitudePoints, Mesh& const mesh);
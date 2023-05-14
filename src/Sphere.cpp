#include "Sphere.h"

using namespace Math;

void Sphere::CreateSphere(int radius, int longitude, int latitude) {
    DrawSphere(radius, longitude, latitude, mesh);
    mesh.CreateMesh();
}

void Sphere::Update() {
    mesh.Update();
}

void DrawSphere(int radius, int longitudePoints, int latitudePoints, Mesh& const mesh) {
    std::vector<Vertex>().swap(mesh.vertices);
    std::vector<unsigned int>().swap(mesh.indices);

    float longitudeAngle = 0;
    float latitudeAngle = 0;

    for (int x = 0; x <= longitudePoints; x++)
    {
        longitudeAngle = x * 2.0f * PI / longitudePoints;
        for (int y = 0; y <= latitudePoints; y++)
        {
            latitudeAngle = y * PI / latitudePoints;

            float xCoord = (float)x / longitudePoints;
            float yCoord = (float)y / latitudePoints;
            mesh.vertices.push_back(Vertex{
                Vector3(
                    radius * sin(latitudeAngle) * cos(longitudeAngle),
                    radius * cos(latitudeAngle),
                    radius * sin(latitudeAngle) * sin(longitudeAngle)
                ),
                Vector2(xCoord, yCoord),
                Vector3(0, 1, 0)
                });
        }
    }

    for (int x = 0; x < longitudePoints; x++)
    {
        for (int y = 0; y < latitudePoints; y++)
        {
            int p0 = y + x * (latitudePoints + 1);
            int p1 = p0 + 1;
            int p2 = p0 + latitudePoints + 1;
            int p3 = p2 + 1;

            mesh.indices.push_back(p0);
            mesh.indices.push_back(p2);
            mesh.indices.push_back(p1);

            mesh.indices.push_back(p1);
            mesh.indices.push_back(p2);
            mesh.indices.push_back(p3);
        }
    }
}
#include "Mesh.h"

using namespace Math;

void DrawCircle(float radius, int points, Mesh& const mesh) {
    float angle = 0;

    mesh.vertices.push_back(Vertex{ Vector3(0, 0, 0), Vector2{0,0}, Vector3{0,0,0} });

    for (int i = 0; i < points; i++)
    {
        mesh.vertices.push_back(Vertex{ Vector3(0,0,0), Vector2{0,0}, Vector3{0,0,0} });
        mesh.vertices[i].pos = Vector3(radius * cos(radians(angle)), radius * sin(radians(angle)), 0);
        mesh.vertices[i].normals = Vector3(0, 1, 0);

        angle += 360.0f / points;
    }

    for (int i = points; i > 0; i--)
    {
        mesh.indices.push_back(points);
        mesh.indices.push_back(i);
        mesh.indices.push_back(i - 1);
    }

    mesh.indices.push_back(points);
    mesh.indices.push_back(0);
    mesh.indices.push_back(points - 1);
}

void Mesh::CreateMesh() {
    vertexBuffer = new VertexBuffer;
    indexBuffer = new Buffer;

    vertexBuffer->data = &vertices[0];
    vertexBuffer->verticesAmount = vertices.size();
    vertexBuffer->size = vertices.size() * sizeof(Vertex);

    indexBuffer->data = &indices[0];
    indexBuffer->indicesAmount = indices.size();
    indexBuffer->size = indices.size() * sizeof(unsigned int);

    vertexBuffer->Create();
    indexBuffer->Create();
}

void Mesh::Update() {
    GetDeviceContext()->DrawIndexed(indexBuffer->indicesAmount, 0, 0);
}

Mesh::~Mesh() {
    delete(vertexBuffer);
    delete(indexBuffer);
}

void Material::CreateMaterial() {
    MaterialBuffer = new ConstantBuffer;

    MaterialBuffer->data = this;
    MaterialBuffer->size = sizeof(Material);
    MaterialBuffer->Create();
}

void Material::Update() {
    MaterialBuffer->Update();

    GetDeviceContext()->PSSetShaderResources(0, 1, &mainTexture.ImageShaderResourceView);
    GetDeviceContext()->PSSetSamplers(0, 1, &mainTexture.ImageSamplerState);
    GetDeviceContext()->PSSetConstantBuffers(1, 1, &MaterialBuffer->buffer);
}

/*void DrawSphere(int radius, int longitudePoints, int latitudePoints, Mesh& const mesh) {
    mesh.vertices.clear();
    mesh.indices.clear();

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
}*/
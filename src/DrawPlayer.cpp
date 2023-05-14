#include "DrawPlayer.h"

using namespace Math;
#ifdef RELEASE
#define VSPath L"../../../src/VertexShader.hlsl"
#define PSPath L"../../../src/PixelShader.hlsl"
#endif // RELEASE

#ifdef DEBUG
#define VSPath L"../src/VertexShader.hlsl"
#define PSPath L"../src/PixelShader.hlsl"
#endif // DEBUG

Camera camera;
Material mainMaterial;

std::vector<Transform*> transforms = { new Transform, new Transform };
std::vector<Sphere*> spheres = { new Sphere, new Sphere };
std::vector<Material> materials = { Material{}, Material{} };

float pitch = 0;
float yaw = 0;

void FreeCam(float dt, Vector3* mouseInput, float speed) {

    float sens = 0.1f;

    float mouseX = mouseInput->x * sens;
    float mouseY = mouseInput->y * sens;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    if (yaw > 360.0f || yaw < -360.0f)
    {
        yaw = 0;
    }

    float cameraSpeed = 0;
    cameraSpeed = speed * dt;

    if (GetKeyState('A') & 0x8000)
    {
        camera.position -= camera.right * cameraSpeed;
    }

    if (GetKeyState('D') & 0x8000)
    {
        camera.position += camera.right * cameraSpeed;
    }

    if (GetKeyState('W') & 0x8000)
    {
        camera.position += camera.forward * cameraSpeed;
    }

    if (GetKeyState('S') & 0x8000)
    {
        camera.position -= camera.forward * cameraSpeed;
    }

    if (GetKeyState(VK_LSHIFT) & 0x8000)
    {
        camera.position -= camera.up * cameraSpeed;
    }

    if (GetKeyState(VK_SPACE) & 0x8000)
    {
        camera.position += camera.up * cameraSpeed;
    }

    ImGui::Text("Camera Position: %f, %f, %f", camera.position.x, camera.position.y, camera.position.z);

    yaw += mouseX;
    pitch += mouseY;

    camera.rotation = Math::Vector3(Math::radians(pitch), Math::radians(yaw), 0);
}

int radii[2] = { 5, 10 };

void Start() {
    mainMaterial.CreateMaterial();
    for (int i = 0; i < transforms.size(); i++)
    {
        spheres.at(i)->CreateSphere(radii[i], 5, 5);
        materials.at(i).CreateMaterial();
        transforms.at(i)->CreateTransform();
    }

    mainMaterial.pixelShader.create(PSPath, "main", "ps_5_0");
    mainMaterial.vertexShader.create(VSPath, "main", "vs_5_0");

    camera.CreateCamera(70.0f, 800, 800, 3.0f, 5000.0f);
}

void Update(float dt, Vector3* mouseInput) {
    FreeCam(dt, mouseInput, 20.0f);

    transforms.at(0)->position = Vector3(10.0f, 0.0f, 0.0f);
    transforms.at(1)->position = Vector3(-5.0f, 0.0f, 0.0f);

    camera.Update();
    mainMaterial.Update();
    for (int i = 0; i < transforms.size(); i++)
    {
        spheres.at(i)->Update();
        materials.at(i).Update();
        transforms.at(i)->Update();
    }
}

void Deconstruct() {

}
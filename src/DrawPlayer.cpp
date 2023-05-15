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

void Start() {
    camera.CreateCamera(70.0f, 800, 800, 3.0f, 5000.0f);
}

void Update(float dt, Vector3* mouseInput) {
    FreeCam(dt, mouseInput, 20.0f);
}
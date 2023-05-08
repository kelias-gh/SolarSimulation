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

struct Material
{
    Math::Vector4 ambient;
    Math::Vector4 diffuse;

    Texture2D mainTexture;
};

struct Planet {
    Mesh mesh;
    Shader vertexShader;
    Shader pixelShader;
    Buffer* indexBuffer;
    VertexBuffer* vertexBuffer;
    ConstantBuffer* constantBuffer;
    ConstantBuffer* MaterialBuffer;
    Transform transform;
    MatrixBuffer shaderMatrices;
    Material mainMaterial;
    Matrix4x4 worldMatrix;
}Venus, Earth;
Camera camera;

float pitch = 0;
float yaw = 0;

float speed = 20.0f;

void FreeCam(float dt, Vector3* mouseInput) {
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
        camera.transform.position -= camera.transform.right * cameraSpeed;
    }

    if (GetKeyState('D') & 0x8000)
    {
        camera.transform.position += camera.transform.right * cameraSpeed;
    }

    if (GetKeyState('W') & 0x8000)
    {
        camera.transform.position += camera.transform.forward * cameraSpeed;
    }

    if (GetKeyState('S') & 0x8000)
    {
        camera.transform.position -= camera.transform.forward * cameraSpeed;
    }

    if (GetKeyState(VK_LSHIFT) & 0x8000)
    {
        camera.transform.position -= camera.transform.up * cameraSpeed;
    }

    if (GetKeyState(VK_SPACE) & 0x8000)
    {
        camera.transform.position += camera.transform.up * cameraSpeed;
    }

    ImGui::Text("Camera Position: %f, %f, %f", camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);

    yaw += mouseX;
    pitch += mouseY;

    camera.transform.rotation = Math::Vector3(Math::radians(pitch), Math::radians(yaw), 0);
}

void InitializePlanet(Planet& const planet) {
    planet.constantBuffer = new ConstantBuffer;
    planet.MaterialBuffer = new ConstantBuffer;
    planet.vertexBuffer = new VertexBuffer;
    planet.indexBuffer = new Buffer;

    planet.constantBuffer->data = &planet.shaderMatrices;
    planet.constantBuffer->size = sizeof(MatrixBuffer);

    planet.MaterialBuffer->data = &planet.mainMaterial;
    planet.MaterialBuffer->size = sizeof(Material);

    planet.vertexBuffer->data = &planet.mesh.vertices[0];
    planet.vertexBuffer->verticesAmount = planet.mesh.vertices.size();
    planet.vertexBuffer->size = planet.mesh.vertices.size() * sizeof(Vertex);

    planet.indexBuffer->data = &planet.mesh.indices[0];
    planet.indexBuffer->indicesAmount = planet.mesh.indices.size();
    planet.indexBuffer->size = planet.mesh.indices.size() * sizeof(uint32_t);

    planet.vertexBuffer->Create();
    planet.indexBuffer->Create();
    planet.constantBuffer->Create();
    planet.MaterialBuffer->Create();

    planet.vertexShader.create(VSPath, "main", "vs_5_0");
    planet.pixelShader.create(PSPath, "main", "ps_5_0");
}

void UpdatePlanet(Planet& const planet) {
    planet.transform.calculateVectors();

    planet.shaderMatrices.world =
        Math::Translation(planet.transform.position.x,
            planet.transform.position.y,
            planet.transform.position.z)

        * Math::RotationRollPitchYaw(planet.transform.rotation.x,
            planet.transform.rotation.y,
            planet.transform.rotation.z)

        * Math::Scaling(planet.transform.scale.x,
            planet.transform.scale.y,
            planet.transform.scale.z);

    planet.shaderMatrices.wvp = planet.shaderMatrices.world
        * Math::LookAtView(camera.transform.position, camera.transform.position + camera.transform.forward, camera.transform.up)
        * camera.ProjectionMatrix;

    planet.shaderMatrices.world = Math::Transpose(planet.shaderMatrices.world);

    planet.shaderMatrices.wvp = Math::Transpose(planet.shaderMatrices.wvp);

    planet.constantBuffer->Update();
    planet.MaterialBuffer->Update();

    GetDeviceContext()->PSSetShaderResources(0, 1, &planet.mainMaterial.mainTexture.ImageShaderResourceView);
    GetDeviceContext()->PSSetSamplers(0, 1, &planet.mainMaterial.mainTexture.ImageSamplerState);
    GetDeviceContext()->VSSetConstantBuffers(0, 1, &planet.constantBuffer->buffer);
    GetDeviceContext()->PSSetConstantBuffers(1, 1, &planet.MaterialBuffer->buffer);
    GetDeviceContext()->DrawIndexed(planet.indexBuffer->indicesAmount, 0, 0);
}

void InitializePlayerCharacter() {
    DrawSphere(20, 72, 72, Venus.mesh);
    DrawSphere(20, 72, 72, Earth.mesh);
    //https://www.solarsystemscope.com/textures/

    Venus.mainMaterial.mainTexture = LoadTexture("../src/venus.jpg");
    Earth.mainMaterial.mainTexture = LoadTexture("../src/earth.jpg");

    InitializePlanet(Earth);
    InitializePlanet(Venus);

    camera.createProjection(70.0f, 800, 800, 3.0f, 5000.0f);
}

void UpdatePlayerCharacter(float dt, Math::Vector3* mouseInput) {
    camera.transform.calculateVectors();
    camera.Draw();

    UpdatePlanet(Venus);
    UpdatePlanet(Earth);

    FreeCam(dt, mouseInput);

    Earth.transform.position = Vector3(50.0f, 0.0f, 0.0f);
}

void DeconstructPlayerCharacter() {
    delete(Venus.constantBuffer);
    delete(Venus.indexBuffer);
    delete(Venus.vertexBuffer);
    delete(Venus.MaterialBuffer);

    delete(Earth.constantBuffer);
    delete(Earth.indexBuffer);
    delete(Earth.vertexBuffer);
    delete(Earth.MaterialBuffer);
}
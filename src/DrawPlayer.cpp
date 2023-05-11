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

#define GRAV_CONSTANT 6.6743e-11

#define NUM_BODIES 9

struct Material
{
    Math::Vector4 ambient;
    Math::Vector4 diffuse;

    Texture2D mainTexture;
    Shader vertexShader;
    Shader pixelShader;
};

struct Body {
    Mesh mesh;
    Buffer* indexBuffer;
    VertexBuffer* vertexBuffer;
    ConstantBuffer* constantBuffer;
    ConstantBuffer* MaterialBuffer;
    Transform transform;
    MatrixBuffer shaderMatrices;
    Material mainMaterial;
    Matrix4x4 worldMatrix;

    float mass;
}Bodies[NUM_BODIES];
Camera camera;

Material mainMaterial;
float pitch = 0;
float yaw = 0;

float speed = 20.0f;

Vector3 CalculateOrbit(Vector3 bodyA, Vector3 bodyB, float mass) {
    Vector3 accel;
    /*The Gravitational force is given by
        F = (G * M * m) / r²
        where
        G - gravitational constant
        M - mass of the sun(in this case)
        r - mean distance between the objects
        as we know the force needed to change the motion of an object of mass 'm' is

        F = ma

        Equating them gives

        Acceleration a = (G * M) / r²*/
    float distance = Vector3(bodyA - bodyB).sqrMagnitude();
    Vector3 direction = normalize(Vector3(bodyA - bodyB));

    accel = (direction * GRAV_CONSTANT * mass) / distance;


    return accel;
}

const char* bodyTextures[NUM_BODIES] = {
    "../src/sun.jpg",
    "../src/mercury.jpg",
    "../src/venus.jpg",
    "../src/earth.jpg",
    "../src/mars.jpg",
    "../src/jupiter.jpg",
    "../src/saturn.jpg",
    "../src/uranus.jpg",
    "../src/neptune.jpg",
};

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

void InitializePlanet(Body& const planet) {
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
}

void UpdatePlanet(Body& const planet) {
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
    mainMaterial.pixelShader.create(PSPath, "main", "ps_5_0");
    mainMaterial.vertexShader.create(VSPath, "main", "vs_5_0");

    for (int i = 0; i < sizeof(Bodies) / sizeof(Body); i++)
    {
        Bodies[i].mainMaterial = mainMaterial;
        DrawSphere(20, 72, 72, Bodies[i].mesh);
        Bodies[i].mainMaterial.mainTexture = LoadTexture(bodyTextures[i]);
        InitializePlanet(Bodies[i]);
    }
    //https://www.solarsystemscope.com/textures/


    camera.createProjection(70.0f, 800, 800, 3.0f, 5000.0f);
}

void UpdatePlayerCharacter(float dt, Math::Vector3* mouseInput) {
    camera.transform.calculateVectors();
    camera.Draw();

    for (int i = 0; i < sizeof(Bodies) / sizeof(Body); i++)
    {
        UpdatePlanet(Bodies[i]);
    }

    FreeCam(dt, mouseInput);

    static float angle = 0.0f;
            angle += 0.1f * dt * 10.0f;
             float x = 90.0f * std::cos(radians(angle));
    float z = 90.0f * std::sin(radians(angle));
    for (int i = 0; i < sizeof(Bodies) / sizeof(Body); i++)
    {
        Bodies[i].transform.position = Vector3(x, 0.0f, z) * i;
    }
    
    Bodies[1].transform.position = Vector3(200.0f, 20.0f, 20.0f);
    Bodies[1].transform.position = CalculateOrbit(Bodies[1].transform.position, Bodies[0], 10);

    //for (int i = 0; i < sizeof(Bodies) / sizeof(Body); i++)
    //{
    //    Bodies[i].transform.position = Vector3(x, 0.0f, z) * i;
    //}
}

void DeconstructPlayerCharacter() {
    for (int i = 0; i < sizeof(Bodies) / sizeof(Body); i++)
    {
        delete(Bodies[i].constantBuffer);
        delete(Bodies[i].indexBuffer);
        delete(Bodies[i].vertexBuffer);
        delete(Bodies[i].MaterialBuffer);
    }
}
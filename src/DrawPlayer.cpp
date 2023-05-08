#include "DrawPlayer.h"
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"
using namespace Math;

struct Texture2D {
    int ImageWidth;
    int ImageHeight;

    ID3D11ShaderResourceView* ImageShaderResourceView;
    ID3D11SamplerState* ImageSamplerState;
};

struct Material
{
    Math::Vector4 ambient;
    Math::Vector4 diffuse;

    Texture2D mainTexture;
};

struct Planet {

};

Texture2D LoadTexture(const char* filePath) {
    Texture2D tex;

    int ImageWidth;
    int ImageHeight;
    int ImageChannels;
    int ImageDesiredChannels = 4;

    unsigned char* ImageData = stbi_load(filePath,
        &ImageWidth,
        &ImageHeight,
        &ImageChannels, ImageDesiredChannels);

    int ImagePitch = ImageWidth * 4;

    D3D11_TEXTURE2D_DESC ImageTextureDesc = {};

    ImageTextureDesc.Width = ImageWidth;
    ImageTextureDesc.Height = ImageHeight;
    ImageTextureDesc.MipLevels = 1;
    ImageTextureDesc.ArraySize = 1;
    ImageTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    ImageTextureDesc.SampleDesc.Count = 1;
    ImageTextureDesc.SampleDesc.Quality = 0;
    ImageTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    ImageTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA ImageSubresourceData = {};

    ImageSubresourceData.pSysMem = ImageData;
    ImageSubresourceData.SysMemPitch = ImagePitch;

    tex.ImageHeight = ImageTextureDesc.Height;
    tex.ImageWidth = ImageTextureDesc.Width;

    ID3D11Texture2D* ImageTexture;

    GetDevice()->CreateTexture2D(&ImageTextureDesc,
        &ImageSubresourceData,
        &ImageTexture
    );

    free(ImageData);

    GetDevice()->CreateShaderResourceView(ImageTexture,
        nullptr,
        &tex.ImageShaderResourceView
    );

    D3D11_SAMPLER_DESC ImageSamplerDesc = {};

    ImageSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    ImageSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    ImageSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    ImageSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    ImageSamplerDesc.MipLODBias = 0.0f;
    ImageSamplerDesc.MaxAnisotropy = 1;
    ImageSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    ImageSamplerDesc.BorderColor[0] = 1.0f;
    ImageSamplerDesc.BorderColor[1] = 1.0f;
    ImageSamplerDesc.BorderColor[2] = 1.0f;
    ImageSamplerDesc.BorderColor[3] = 1.0f;
    ImageSamplerDesc.MinLOD = -FLT_MAX;
    ImageSamplerDesc.MaxLOD = FLT_MAX;

    GetDevice()->CreateSamplerState(&ImageSamplerDesc,
        &tex.ImageSamplerState);

    return tex;
}

Mesh mesh;
Shader vertexShader;
Shader pixelShader;
Buffer* indexBuffer;
VertexBuffer* vertexBuffer;
ConstantBuffer* constantBuffer;
ConstantBuffer* LightBuffer;
ConstantBuffer* MaterialBuffer;

Light directionalLight;
Camera camera;
Transform transform;
Material playerMaterial;
MatrixBuffer shaderMatrices;
Vector3 lastPosition;
//Rigidbody rigidbody;


struct AABB {
    Vector3 vecMax;
    Vector3 vecMin;
}Collider, Collider2;

bool AABB_Intersection(AABB& a, AABB& b) {
    for (int i = 0; i < 3; i++)
    {
        if (a.vecMin.v[i] > b.vecMax.v[i])
            return false;
        if (a.vecMax.v[i] < b.vecMin.v[i])
            return false;
    }

    return true;
}

#ifdef RELEASE
#define VSPath L"../../../src/VertexShader.hlsl"
#define PSPath L"../../../src/PixelShader.hlsl"
#endif // RELEASE

#ifdef DEBUG
#define VSPath L"../src/VertexShader.hlsl"
#define PSPath L"../src/PixelShader.hlsl"
#endif // DEBUG

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
//https://www.artstation.com/blogs/briz/oKpM/perfectly-remapping-a-cube-to-a-sphere-houdini

void DrawSphere(int radius, int longitudePoints, int latitudePoints, Mesh& const mesh) {
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

Material mainMaterial;

void InitializePlayerCharacter() {
    DrawSphere(20, 24, 72, mesh);
    //https://www.solarsystemscope.com/textures/

    mainMaterial.mainTexture = LoadTexture("C:/PGame/src/venus.jpg");

    constantBuffer = new ConstantBuffer;
    LightBuffer = new ConstantBuffer;
    MaterialBuffer = new ConstantBuffer;
    vertexBuffer = new VertexBuffer;
    indexBuffer = new Buffer;

    constantBuffer->data = &shaderMatrices;
    constantBuffer->size = sizeof(MatrixBuffer);

    LightBuffer->data = &directionalLight;
    LightBuffer->size = sizeof(Light);

    MaterialBuffer->data = &playerMaterial;
    MaterialBuffer->size = sizeof(Material);

    vertexBuffer->data = &mesh.vertices[0];
    vertexBuffer->verticesAmount = mesh.vertices.size();
    vertexBuffer->size = mesh.vertices.size() * sizeof(Vertex);

    indexBuffer->data = &mesh.indices[0];
    indexBuffer->indicesAmount = mesh.indices.size();
    indexBuffer->size = mesh.indices.size() * sizeof(uint32_t);

    vertexBuffer->Create();
    indexBuffer->Create();
    constantBuffer->Create();

    LightBuffer->Create();
    MaterialBuffer->Create();

    vertexShader.create(VSPath, "main", "vs_5_0");
    pixelShader.create(PSPath, "main", "ps_5_0");

    camera.createProjection(70.0f, 800, 800, 3.0f, 100.0f);
}

float pitch = 0;
float yaw = 0;

float speed = 10.0f;

void UpdatePlayerCharacter(float dt, Math::Vector3* mouseInput) {
    camera.transform.calculateVectors();
    transform.calculateVectors();
    camera.Draw();
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

    float lightxDir = 1, lightyDir = 0, lightzDir = 0;
    directionalLight.dir = Math::Vector3(-lightxDir, -lightyDir, -lightzDir);
    directionalLight.diffuse = Math::Vector4(1.0f, 0.0f, 0.0f, 0.0f);

    directionalLight.camPos = {
        camera.transform.position.x,
        camera.transform.position.y,
        camera.transform.position.z };
    GetDeviceContext()->PSSetConstantBuffers(0, 1, &LightBuffer->buffer);

    ImGui::Text(std::to_string(yaw).c_str());
    ImGui::Text(std::to_string(pitch).c_str());

    shaderMatrices.world =
        Math::Translation(transform.position.x,
            transform.position.y,
            transform.position.z)

        * Math::RotationRollPitchYaw(transform.rotation.x,
            transform.rotation.y,
            transform.rotation.z)

        * Math::Scaling(transform.scale.x,
            transform.scale.y,
            transform.scale.z);

    shaderMatrices.wvp = shaderMatrices.world 
        * Math::LookAtView(camera.transform.position, camera.transform.position + camera.transform.forward, camera.transform.up)
        * camera.ProjectionMatrix;

    shaderMatrices.world = Math::Transpose(shaderMatrices.world);

    shaderMatrices.wvp = Math::Transpose(shaderMatrices.wvp);

    playerMaterial.ambient = Math::Vector4(.5f, 0.0f, 0.0f, 1.0f);

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


    /*Collider.vecMin = Vector3(-1, 0, -1);
    Collider.vecMax = Vector3(1, 1, 1);

    Collider2.vecMin = Vector3(-1, 0, -1);
    Collider2.vecMax = Vector3(1, 2, 2);

    Collider.vecMin += camera.transform.position;
    Collider.vecMax += camera.transform.position;

    Collider2.vecMin += transform.position;
    Collider2.vecMax += transform.position;

    bool isColliding = AABB_Intersection(Collider, Collider2);
    if (isColliding) {
        camera.transform.position = lastPosition;
        playerMaterial.ambient = Math::Vector4(0.0f, 0.5f, 0.0f, 1.0f);
        //directionalLight.dir = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        playerMaterial.ambient = Math::Vector4(.5f, 0.0f, 0.0f, 1.0f);
        //std::cout << "not" << "\n";
        //directionalLight.dir = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    lastPosition = camera.transform.position;
    */
    yaw += mouseX;
    pitch += mouseY;

    camera.transform.rotation = Math::Vector3(Math::radians(pitch), Math::radians(yaw), 0);

    constantBuffer->Update();
    MaterialBuffer->Update();
    LightBuffer->Update();

    GetDeviceContext()->PSSetShaderResources(0, 1, &mainMaterial.mainTexture.ImageShaderResourceView);
    GetDeviceContext()->PSSetSamplers(0, 1, &mainMaterial.mainTexture.ImageSamplerState);
    GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer->buffer);
    GetDeviceContext()->PSSetConstantBuffers(1, 1, &MaterialBuffer->buffer);
    GetDeviceContext()->DrawIndexed(indexBuffer->indicesAmount, 0, 0);
}

void DeconstructPlayerCharacter() {
    delete(constantBuffer);
    delete(indexBuffer);
    delete(vertexBuffer);
    delete(LightBuffer);
    delete(MaterialBuffer);
}
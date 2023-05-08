#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

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
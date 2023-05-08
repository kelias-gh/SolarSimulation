#include "Texture2D.h"

/*void loadTexture()
{
    int ImageWidth;
    int ImageHeight;
    int ImageChannels;
    int ImageDesiredChannels = 4;

    unsigned char* ImageData = stbi_load("earth.jpg",
        &ImageWidth,
        &ImageHeight,
        &ImageChannels, ImageDesiredChannels);

    int ImagePitch = ImageWidth * sizeof(unsigned char) * 4;

    // Texture

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

    ID3D11Texture2D* ImageTexture;

    GetDevice()->CreateTexture2D(&ImageTextureDesc,
        &ImageSubresourceData,
        &ImageTexture
    );

    stbi_image_free(ImageData);

    // Shader resource view

    ID3D11ShaderResourceView* ImageShaderResourceView;

    GetDevice()->CreateShaderResourceView(ImageTexture,
        nullptr,
        &ImageShaderResourceView
    );

    // Sampler

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

    ID3D11SamplerState* ImageSamplerState;

    GetDevice()->CreateSamplerState(&ImageSamplerDesc,
        &ImageSamplerState);

    GetDeviceContext()->PSSetShaderResources(0, 1, &ImageShaderResourceView);
    GetDeviceContext()->PSSetSamplers(0, 1, &ImageSamplerState);
}*/
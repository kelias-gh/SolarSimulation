#pragma once
#include "d3d11_renderer.h"
#include <d3d11.h>
#include "stb_image.h"

struct Texture2D {
    int ImageWidth;
    int ImageHeight;

    ID3D11ShaderResourceView* ImageShaderResourceView;
    ID3D11SamplerState* ImageSamplerState;
};

Texture2D LoadTexture(const char* filePath);
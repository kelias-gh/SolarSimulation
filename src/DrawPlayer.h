#pragma once

#include "Camera.h"
#include "Sphere.h"
#include "d3d11_renderer.h"
#include "AOMMath.h"
#include "ModelLoader.h"
#include "ConstantBuffers.h"
#include "Rigidbody.h"
#include "Texture2D.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

void Start();

void Update(float dt, Math::Vector3* mouseInput);

void Deconstruct();
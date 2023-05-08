#pragma once
#include "DrawPlayer.h"
#include "Time.h"
#include <algorithm>
#include <ostream>
#include <windowsx.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

extern Math::Vector3* mouseInput;

void CreateScene(HWND hwnd);

void UpdateScene();

void DestroyScene();
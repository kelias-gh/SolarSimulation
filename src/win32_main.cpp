#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <Windows.h>

#include "d3d11_renderer.h"

#include "Scene.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "AOMMath.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((unsigned short) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((unsigned short) 0x02)
#endif

Math::Vector3* mouseInput;
Math::Vector3 mouseInputV;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK
WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR pCmdLine,
    int nCmdShow) {
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = L"Solar System Simulation";
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (RegisterClass(&WindowClass)) {
        HWND WindowHandle = CreateWindowEx(
            0,
            WindowClass.lpszClassName,
            L"Solar System",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            800,
            0,
            0,
            hInstance,
            0
        );
        if (WindowHandle) {
            RAWINPUTDEVICE Rid[1];
            Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
            Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
            Rid[0].dwFlags = RIDEV_INPUTSINK;
            Rid[0].hwndTarget = WindowHandle;
            RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));


            Initialize(WindowHandle, 800, 800);

            CreateScene(WindowHandle);

            mouseInput = &mouseInputV;

    MSG msg = { 0 };
    while (true)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

                DrawScene();
                UpdateScene();
                EndFrame();

                mouseInput->x = 0;
                mouseInput->y = 0;
                mouseInput->z = 0;
            }

            DestroyScene();
        }
    };

    return 0;
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    unsigned size = sizeof(RAWINPUT);
    static RAWINPUT raw[sizeof(RAWINPUT)];
    switch (uMsg)
    {
    case WM_SIZE: {
        RECT Rect;
        GetWindowRect(hwnd, &Rect);
        int width = Rect.left - Rect.right;
        int height = Rect.top - Rect.bottom;
        WindowResizeBuffers(width, height);
        OutputDebugStringA("Resized Window \n");
    }break;
    case WM_DESTROY: {
        PostQuitMessage(0);
        OutputDebugStringA("Destroyed Window \n");
    }break;
    case WM_CLOSE: {
        DestroyWindow(hwnd);
        OutputDebugStringA("Closed Window \n");
    }break;
    case WM_ACTIVATEAPP: {
        OutputDebugStringA("Active App \n");
    }break;
    case WM_INPUT: {
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));
        OutputDebugStringA("Mouse moving\n");
        if (raw->header.dwType == RIM_TYPEMOUSE) {
            mouseInputV.y = raw->data.mouse.lLastY;
            mouseInputV.x = raw->data.mouse.lLastX;

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
                mouseInputV.z = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;
        }
    }break;
    default:
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#include "Scene.h"

Timer deltaTimer;
Timer fpstimer;

void CreateScene(HWND hwnd) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(GetDevice(), GetDeviceContext());
    ImGui::StyleColorsDark();

    fpstimer.Start();

    Start();
}
float dt;
void UpdateScene() {
    float dt = deltaTimer.GetDeltaTime();
    deltaTimer.Restart();

    static int fpsCounter = 0;
    static std::string fpsString = "FPS: 0";
    fpsCounter += 1;
    if (fpstimer.GetDeltaTime() > 1)
    {
        fpsString = "FPS: " + std::to_string(fpsCounter);
        fpsCounter = 0;
        fpstimer.Restart();
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Text(fpsString.c_str());

    Update(dt, mouseInput);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DestroyScene() {
    Deconstruct();
}
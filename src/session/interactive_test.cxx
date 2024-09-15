#include <deque>
#include <memory>
#include <string>

#include <d3d9.h>
#include <tchar.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "sf4e__SessionClient.hxx"
#include "sf4e__SessionServer.hxx"
#include "sf4e__SessionProtocol.hxx"

using ImGui::Begin;
using ImGui::Button;
using ImGui::End;
using ImGui::Separator;
using ImGui::Text;

using sf4e::SessionClient;
using sf4e::SessionProtocol::JoinRequest;
using sf4e::SessionProtocol::SetConditionsRequest;
using sf4e::SessionProtocol::ReportResultsRequest;
using sf4e::SessionProtocol::LobbyData;
using sf4e::SessionProtocol::MatchData;
using sf4e::SessionServer;

// 99% of the skeleton is copy-pasted from Imgui's main example, but with
// custom windows.

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static bool                     g_DeviceLost = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static char* characterNames[] = {
    "Chara 0",
    "Chara 1",
    "Chara 2",
};
#define NUM_STUB_CHARAS 3

struct TestingSessionClient {
    SessionClient c;
    int menuCharaID;
    SetConditionsRequest setConditionsReqBuf;
    ReportResultsRequest reportResultsReqBuf;

    TestingSessionClient(
        std::string sidecarHash,
        uint16_t ggpoPort,
        std::string& name,
        uint8_t deviceType,
        uint8_t deviceIdx,
        uint8_t delay
    ) : c(
        sidecarHash,
        ggpoPort,
        name,
        deviceType,
        deviceIdx,
        delay
    ), menuCharaID(0) {
        setConditionsReqBuf.rngSeed = 0;
        setConditionsReqBuf.stageID = 0;
        setConditionsReqBuf.chara.charaID = 0;
        setConditionsReqBuf.chara.costume = 0;
        setConditionsReqBuf.chara.color = 0;
        setConditionsReqBuf.chara._unused = 0;
        setConditionsReqBuf.chara.personalAction = 0;
        setConditionsReqBuf.chara.winQuote = 0;
        setConditionsReqBuf.chara.ultraCombo = 0;
        setConditionsReqBuf.chara.handicap = 0;
        setConditionsReqBuf.chara.unc_edition = 0;
        reportResultsReqBuf.loserSide = 0;
    }
};

static std::unique_ptr<SessionServer> g_server;
static std::deque<TestingSessionClient> g_clients;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DrawCharaConditionText(const MatchData& matchData, int i) {
    Text(
        "Chara %d: id %d, color %d, costume %d, handicap %d, PA %d, UC %d, edition %d, win quote %d",
        i,
        matchData.chara[i].charaID,
        matchData.chara[i].color,
        matchData.chara[i].costume,
        matchData.chara[i].handicap,
        matchData.chara[i].personalAction,
        matchData.chara[i].ultraCombo,
        matchData.chara[i].unc_edition,
        matchData.chara[i].winQuote
    );
}

void DrawClientSessionInfo(const LobbyData& lobbyData, const MatchData& matchData) {
    Text("RNG seed: %d", matchData.rngSeed);
    Text("Stage ID: %d", matchData.stageID);
    for (int i = 0; i < 2; i++) {
        DrawCharaConditionText(matchData, i);
    }

    for (int i = 0; i < 2 && i < lobbyData.size(); i++) {
        const char* label = i == 0 ? "P1" : "P2";
        Text(
            "%s: %s (%s)",
            label,
            lobbyData[i].name.c_str(),
            matchData.readyMessageNum[i] > -1 ? "Ready!" : "Waiting"
        );
    }
    Separator();
    Text("Queue:");
    if (lobbyData.size() > 2) {
        for (int i = 2; i < lobbyData.size(); i++) {
            Text(lobbyData[i].name.c_str());
        }
    }
    else {
        Text("(No one in queue)");
    }

    Separator();
}

void DrawServerSessionInfo(const std::vector<SessionServer::SessionMember>& clients, const MatchData& matchData) {
    Text("RNG seed: %d", matchData.rngSeed);
    Text("Stage ID: %d", matchData.stageID);
    for (int i = 0; i < 2; i++) {
        DrawCharaConditionText(matchData, i);
    }

    for (int i = 0; i < 2 && i < clients.size(); i++) {
        const char* label = i == 0 ? "P1" : "P2";
        Text(
            "%s: %s (%s)",
            label,
            clients[i].data.name.c_str(),
            matchData.readyMessageNum[i] > -1 ? "Ready!" : "Waiting"
        );
    }
    Separator();
    Text("Queue:");
    if (clients.size() > 2) {
        for (int i = 2; i < clients.size(); i++) {
            Text(clients[i].data.name.c_str());
        }
    }
    else {
        Text("(No one in queue)");
    }

    Separator();
}

int DrawSetConditionsForm(TestingSessionClient& client) {
    SetConditionsRequest& req = client.setConditionsReqBuf;
    static const int stepSize = 1;

    ImGui::InputScalar("RNG seed", ImGuiDataType_U32, &req.rngSeed);
    ImGui::InputInt("Stage ID", &req.stageID);
    ImGui::Combo("Chara", &client.menuCharaID, characterNames, NUM_STUB_CHARAS);
    req.chara.charaID = client.menuCharaID;
    ImGui::InputScalar("Color", ImGuiDataType_U8, &req.chara.color, &stepSize);
    ImGui::InputScalar("Costume", ImGuiDataType_U8, &req.chara.costume, &stepSize);
    ImGui::InputScalar("Handicap", ImGuiDataType_U8, &req.chara.handicap, &stepSize);
    ImGui::InputScalar("Personal action", ImGuiDataType_U8, &req.chara.personalAction, &stepSize);
    ImGui::InputScalar("Ultra Combo", ImGuiDataType_U8, &req.chara.ultraCombo, &stepSize);
    ImGui::InputScalar("Edition", ImGuiDataType_U8, &req.chara.unc_edition, &stepSize);
    ImGui::InputScalar("Win quote", ImGuiDataType_U8, &req.chara.winQuote, &stepSize);

    if (Button("Send set conditions")) {
        return client.c.SetMatchConditions(req);
    }

    return k_EResultNone;
}

int DrawResultsForm(TestingSessionClient& client) {
    ReportResultsRequest& req = client.reportResultsReqBuf;
    ImGui::InputInt("Loser side", &req.loserSide);

    if (Button("Send results")) {
        return client.c.ReportResults(req);
    }

    return k_EResultNone;
}

int DrawServerWindow() {
    static int nNextClientID = 0;
    static int nNextGgpoPort = 23456;

    Begin(
        "Server",
        nullptr,
        ImGuiWindowFlags_None
    );
    Text("Server window");
    if (g_server) {
        DrawServerSessionInfo(g_server->clients, g_server->_matchData);
        if (Button("Create new client")) {
            char szNewClientName[32];
            sprintf_s(szNewClientName, 32, "client %d", nNextClientID);
            HSteamNetConnection pOutConnection1;
            HSteamNetConnection pOutConnection2;

            SteamNetworkingSockets()->CreateSocketPair(&pOutConnection1, &pOutConnection2, false, nullptr, nullptr);
            g_server->AddConnection(pOutConnection1);
            g_clients.emplace_back(
                std::string("123"),
                nNextGgpoPort,
                std::string(szNewClientName),
                0,
                0,
                0
            );
            g_clients.back().c.Connect(pOutConnection2);
            nNextClientID++;
            nNextGgpoPort++;
        }
    }
    else {
        Text("Waiting for server...");
    }

    End();
    return 0;
}

int DrawClientWindow(int idx, TestingSessionClient& client) {
    char label[64];
    sprintf_s(label, "Client %d (%s)", idx, client.c._name.c_str());
    Begin(
        label,
        nullptr,
        ImGuiWindowFlags_None
    );
    Text("Client window for %d: %s", idx, client.c._name.c_str());
    if (Button("Send join")) {
        JoinRequest request;
        request.sidecarHash = client.c._sidecarHash;
        request.username = client.c._name;
        request.port = client.c._ggpoPort;
        nlohmann::json msg = request;
        if (client.c.Send(msg, nullptr) != k_EResultOK) {
        	spdlog::warn("Client could send initial join request");
        }
    }

    DrawClientSessionInfo(client.c._lobbyData, client.c._matchData);
    Separator();
    DrawSetConditionsForm(client);
    Separator();
    DrawResultsForm(client);
    End();
    return 0;
}

// Main code
int main(int, char**)
{
    SteamDatagramErrMsg errMsg;
    if (!GameNetworkingSockets_Init(nullptr, errMsg)) {
        spdlog::error("GameNetworkingSockets_Init failed.  {}", errMsg);
    }
    g_server.reset(new SessionServer(std::string("123")));

    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"sf4e session interactive tests", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"sf4e session interactive tests", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        if (g_server) {
            g_server->PrepareForCallbacks();
        }
        SteamNetworkingSockets()->RunCallbacks();

        auto iter = g_clients.begin();
        while (iter != g_clients.end()) {
            if (iter->c.Step()) {
                iter = g_clients.erase(iter);
            }
            else {
                iter++;
            }
        }

        if (g_server) {
            if (g_server->Step()) {
                delete g_server.release();
            }
        }

        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle lost D3D9 device
        if (g_DeviceLost)
        {
            HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            g_DeviceLost = false;
        }

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        DrawServerWindow();
        for (int i = 0; i < g_clients.size(); i++) {
            DrawClientWindow(i, g_clients[i]);
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            g_DeviceLost = true;
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    g_server.reset();
    g_clients.clear();
    GameNetworkingSockets_Kill();
    spdlog::shutdown();

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

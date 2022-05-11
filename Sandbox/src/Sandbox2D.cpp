#include "Sandbox2D.h"
#include <chrono>

using namespace Orca;

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWDDDDDDDWWWWWWWWWW"
                                "WWWWWWDDDDDDDDDDDWWWWWWW"
                                "WWWWWWDDDDDDDDDDDDDWWWWW"
                                "WWWWDDDDDDDDDDDDDDDDWWWW"
                                "WWDDDDDDDDDDDDDDDDDDDWWW"
                                "WWDDDDDDDDDDDDDDDDDDDWWW"
                                "WWDDDDDDDDDDDDDDDDDDDWWW"
                                "WDDDDDDDDDDDDDDDDDDDDDWW"
                                "WWWDDDDDDDDDDDDDDDDDWWWW"
                                "WWWWDDDDDDDDDDDDDDWWWWWW"
                                "WWWWWWWWDDDDDDDWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D() 
    : Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
    
}

void Sandbox2D::OnAttach() {
    OA_PROFILE_FUNCTION();
    m_Texture = Texture2D::Create("assets/textures/orca.png");
    m_SpriteSheet = Texture2D::Create("assets/game/RPGpack_sheet_2X.png");
    m_TileMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11}, {128, 128}, {1, 1});
    m_TileMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }, { 1, 1 });
    m_CameraController.SetZoomLevel(5.0f);

    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
}



void Sandbox2D::OnDetach() {
    OA_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Orca::Timestep ts) {
    OA_PROFILE_FUNCTION();
    //angle += 1*ts;

    Renderer2D::ResetStats();
    m_CameraController.OnUpdate(ts);

    RenderCommand::Clear();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });


    Renderer2D::BeginScene(m_CameraController.GetCamera());
    for (int i = 0; i < m_MapHeight; i++) {
        for (int j = 0; j < m_MapWidth; j++) {
            char tileType = s_MapTiles[j + i * m_MapWidth];
            Renderer2D::DrawQuad({ 0.5f * j - m_MapWidth/2.0f, 0.5f * i - m_MapHeight/2.0f}, {0.5f, 0.5f}, 0.0f, m_TileMap[tileType], {1.0f, 1.0f, 1.0f, 1.0f});


        }
    }
    Renderer2D::EndScene();


    
    
}


void Sandbox2D::OnImGuiRender() {
    OA_PROFILE_FUNCTION();

    auto stats = Renderer2D::GetStats();

    



    static bool dockspace_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("The Open Ocean", &dockspace_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("OpenOcean");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
    ImGui::End();

    ImGui::End();
}


void Sandbox2D::OnEvent(Orca::Event& e) {
    OA_PROFILE_FUNCTION();
    m_CameraController.OnEvent(e);
}


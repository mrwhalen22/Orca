#include "EditorLayer.h"
#include <chrono>

namespace Orca {
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

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(16.0f / 9.0f)
    {

    }

    void EditorLayer::OnAttach() {
        OA_PROFILE_FUNCTION();
        m_Texture = Texture2D::Create("assets/textures/orca.png");
        m_SpriteSheet = Texture2D::Create("assets/game/RPGpack_sheet_2X.png");
        m_TileMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }, { 1, 1 });
        m_TileMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }, { 1, 1 });
        m_CameraController.SetZoomLevel(5.0f);

        m_MapWidth = s_MapWidth;
        m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

    }



    void EditorLayer::OnDetach() {
        OA_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Orca::Timestep ts) {
        OA_PROFILE_FUNCTION();
        //angle += 1*ts;

       m_FPS = 1.0f/ts.GetSeconds();

        Renderer2D::ResetStats();
        if(m_ViewportFocused)
            m_CameraController.OnUpdate(ts);

        m_Framebuffer->Bind();
        RenderCommand::Clear();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });


        Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (int i = 0; i < m_MapHeight; i++) {
            for (int j = 0; j < m_MapWidth; j++) {
                char tileType = s_MapTiles[j + i * m_MapWidth];
                Renderer2D::DrawQuad({ 0.5f * j - m_MapWidth / 4.0f, 0.5f * i - m_MapHeight / 4.0f }, { 0.5f, 0.5f }, 0.0f, m_TileMap[tileType], m_Color);

            }
        }
        Renderer2D::EndScene();
        m_Framebuffer->Unbind();



    }


    void EditorLayer::OnImGuiRender() {
        OA_PROFILE_FUNCTION();

        auto stats = Renderer2D::GetStats();


        static bool dockspace_open = true;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowBorderSize = 0.0f;
        style.WindowRounding = 0.0f;
        style.WindowPadding = { 0.0f, 0.0f };

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::Begin("The Open Ocean", &dockspace_open, window_flags);

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
                ImGui::Separator();

                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                ImGui::Separator();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        ImGui::Begin("Settings");
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::Text("FPS: %.3f", m_FPS);
        ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
        ImGui::End();

        
        ImGui::Begin("Viewport");
        
        // Blocks events to the ImGuiLayer if the viewport is not focused or hovered.
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);
        

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *((glm::vec2*)&viewportSize)) {
            m_ViewportSize = { viewportSize.x, viewportSize.y };
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        }
        uint32_t TextureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)TextureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
        
        
        ImGui::End();
        ImGui::PopStyleVar();

      

        ImGui::End();
    }


    void EditorLayer::OnEvent(Orca::Event& e) {
        OA_PROFILE_FUNCTION();
        m_CameraController.OnEvent(e);
    }
}




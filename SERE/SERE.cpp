// SERE.cpp : Defines the entry point for the application.
//

#include <fstream>
#include <streambuf>

#include "SERE.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Imgui/implot.h"

#include "RenderFrameworks/RenderFramework_Dx11.h"
#include "RuiNodeEditor/RuiNodeEditor.h"

#include "Nodes/ArgumentNodes.h"
#include "Nodes/ConstantVarNodes.h"
#include "Nodes/GlobalNodes.h"
#include "Nodes/MathNodes.h"
#include "Nodes/RenderJobNodes.h"
#include "Nodes/SplitMergeNodes.h"
#include "Nodes/TransformNodes.h"

#include "ThirdParty/nativefiledialog-extended/src/include/nfd.hpp"




static void ShowDockingDisabledMessage()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
    ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
    ImGui::SameLine(0.0f, 0.0f);
    if (ImGui::SmallButton("click here"))
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ShowExampleAppDockSpace(bool* p_open)
{
    // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - we allow the host window to have padding (when opt_padding == true)
    // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
    // TL;DR; this demo is more complicated than what you would normally use.
    // If we removed all the options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }

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
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        ShowDockingDisabledMessage();
    }

    //if (ImGui::BeginMenuBar())
    //{
    //    if (ImGui::BeginMenu("Options"))
    //    {
    //        // Disabling fullscreen would allow the window to be moved to the front of other windows,
    //        // which we can't undo at the moment without finer window depth/z control.
    //        ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
    //        ImGui::MenuItem("Padding", NULL, &opt_padding);
    //        ImGui::Separator();

    //        if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
    //        if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
    //        if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
    //        if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
    //        if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
    //        ImGui::Separator();

    //        if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
    //            *p_open = false;
    //        ImGui::EndMenu();
    //    }
    //    HelpMarker(
    //        "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
    //        "- Drag from window title bar or their tab to dock/undock." "\n"
    //        "- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
    //        "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)" "\n"
    //        "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)" "\n"
    //        "This demo app has nothing to do with enabling docking!" "\n\n"
    //        "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window." "\n\n"
    //        "Read comments in ShowExampleAppDockSpace() for more details.");

    //    ImGui::EndMenuBar();
    //}

    ImGui::End();
}



// Main code
int main(int, char**)
{


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    std::shared_ptr<RenderFramework> renderFramework = std::make_shared<RenderFramework_Dx11>();
    
    renderFramework->RuiCreatePipeline(Vector2(1920,1080));

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    ImFontConfig config;
    config.OversampleH = 2;
    io.Fonts->AddFontFromFileTTF("imgui/DroidSans.ttf", 16.0f,&config);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool use_docking_space = false;

    loadFonts(renderFramework);
    loadImageAtlases(renderFramework);

    RenderInstance render{1920,1080,renderFramework};
    NodeEditor nodeEdit{render};
    AddArgumentNodes(nodeEdit);
    AddConstantVarNodes(nodeEdit);
    AddMathNodes(nodeEdit);
    AddGlobalNodes(nodeEdit);
    AddRenderNodes(nodeEdit);
    AddSplitMergeNodes(nodeEdit);
    AddTransformNodes(nodeEdit);

    while (renderFramework->ShouldMainLoopRun())
    {

        // Handle window being minimized or screen locked
        if (!renderFramework->ImGuiStartFrame()) {
            continue;
        }

        // Start the Dear ImGui frame
        
        ImGui::NewFrame();

     
        ShowExampleAppDockSpace(&use_docking_space);
        
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {
                   nodeEdit.Clear();
                }
                if (ImGui::MenuItem("Save Graph")) {
                    nodeEdit.Serialize();
                }
                if (ImGui::MenuItem("Load Graph")) {
                    nodeEdit.Deserialize();
                }
                if (ImGui::MenuItem("Export")) {
                    nodeEdit.Export();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        render.StartFrame(ImGui::GetCurrentContext()->Time);
        nodeEdit.Draw();
        render.EndFrame();
        render.DrawImage();
        
       //ImPlot::ShowDemoWindow();
       // Rendering
       ImGui::Render();
       if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
       renderFramework->ImGuiEndFrame();
    }

    renderFramework->ImGuiDeInit();

    // Cleanup
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    
    return 0;
}

// Helper functions


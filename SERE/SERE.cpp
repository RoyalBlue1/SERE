// SERE.cpp : Defines the entry point for the application.
//

#include <fstream>
#include <streambuf>
#include <execution>

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
#include "Nodes/ConditionalNodes.h"

#include "ThirdParty/nativefiledialog-extended/src/include/nfd.hpp"

#include "Settings.h"
#include "PakLoading/cpakfile.h"



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

    ImGui::End();
}

void ReloadAssets(std::string folderPath) {

    clearImageAtlases();

    loadFonts();
    loadImageAtlases();

    std::vector<std::string> paksToLoad{
        "ui(11).rpak",
        "ui_mp(11).rpak",
        "mp_wargames(11).rpak",
        "mp_thaw(11).rpak",
        "mp_relic02(11).rpak",
        "mp_lf_uma(11).rpak",
        "mp_lf_traffic(11).rpak",
        "mp_lf_township(11).rpak",
        "mp_lf_stacks(11).rpak",
        "mp_lf_deck(11).rpak",
        "mp_homestead(11).rpak",
        "mp_colony02(11).rpak",
        "mp_grave(11).rpak",
        "mp_glitch(11).rpak",
        "mp_forwardbase_kodai(11).rpak",
        "mp_eden(11).rpak",
        "mp_drydock(11).rpak",
        "mp_crashsite3(11).rpak",
        "mp_complex3(11).rpak",
        "mp_coliseum_column(11).rpak",
        "mp_coliseum(11).rpak",
        "mp_black_water_canal(11).rpak",
        "mp_angel_city(11).rpak"
    };

    fs::path pakFolder(folderPath);
    std::for_each(std::execution::par, paksToLoad.begin(), paksToLoad.end(), [pakFolder](std::string& pak) {
        LoadRpak(pakFolder/"r2/paks/Win64"/pak);
    });
}

// Main code
int main(int argc, char** argv)
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
    CreateRenderFramework(argv,argc);
    g_renderFramework->RuiCreatePipeline(Vector2(1920,1080));

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

    bool use_docking_space = false;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    RenderInstance render{1920,1080};
    NodeEditor nodeEdit{render};
    AddArgumentNodes(nodeEdit);
    AddConstantVarNodes(nodeEdit);
    AddMathNodes(nodeEdit);
    AddGlobalNodes(nodeEdit);
    AddRenderNodes(nodeEdit);
    AddSplitMergeNodes(nodeEdit);
    AddTransformNodes(nodeEdit);
    AddConditionalNodes(nodeEdit);

    Settings settings;

    while (g_renderFramework->ShouldMainLoopRun())
    {

        // Handle window being minimized or screen locked
        if (!g_renderFramework->ImGuiStartFrame()) {
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
            if(ImGui::MenuItem("Settings")) {
                settings.Open();
            }
            
            ImGui::EndMainMenuBar();
        }
        settings.ShowSettingsWindow();
        if (settings.HasChanged()) {
            ReloadAssets(settings.GetTitanfall2Path());
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
       g_renderFramework->ImGuiEndFrame();
    }

    g_renderFramework->ImGuiDeInit();

    // Cleanup
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    
    return 0;
}



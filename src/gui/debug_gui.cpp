// DebugGUI.cpp
#include "debug_gui.h"
#include "imgui.h"
#include "utils/collision.h"
#include "TSDL.h"
#include "utils/sprite.h"
#include <SDL_render.h>
#include <SDL_surface.h>
#include <cstddef>
#include <type_traits>
#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"

DebugGUI::GUIValues DebugGUI::guiValues;

void DebugGUI::showSelectedSDLTexture(
    SDL_Texture* texture,
    float screenX, 
    float screenY, 
    float screenWidth, 
    float screenHeight, 
    int tileIndex,
    int columns,
    int tileWidth,
    int tileHeight)
{
if (!texture) return;
    SDL_Renderer* renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
    if (!renderer) return;
    
    // Cleanup previous texture
    if (guiValues.currentTileTexture) 
    {
        SDL_DestroyTexture(guiValues.currentTileTexture);
        guiValues.currentTileTexture = nullptr;
    }
    
    // Calculate the source rectangle from the tileset based on tileIndex
    SDL_Rect srcRect = {
        (tileIndex % columns) * tileWidth,
        (tileIndex / columns) * tileHeight,
        tileWidth,
        tileHeight
    };
    
    // Create a sub-texture of the original tile size (not the scaled size)
    SDL_Texture* subTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                               SDL_TEXTUREACCESS_TARGET, tileWidth, tileHeight);
    SDL_SetRenderTarget(renderer, subTexture);
    SDL_SetTextureBlendMode(subTexture, SDL_BLENDMODE_BLEND);
    
    // Clear the texture
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    // Copy just the specific tile from the tileset to our sub-texture
    SDL_Rect destRect = {0, 0, tileWidth, tileHeight};
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    
    // Reset render target
    SDL_SetRenderTarget(renderer, nullptr);
    
    // Store the new texture for ImGui
    guiValues.currentTileTexture = subTexture;
    
    // Store extra info if needed
    guiValues.selectedTileScreenX = screenX;
    guiValues.selectedTileScreenY = screenY;
    guiValues.selectedTileScreenWidth = screenWidth;
    guiValues.selectedTileScreenHeight = screenHeight;
}
void DebugGUI::ApplyGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 5.0f; // Rounded corners
    style.WindowRounding = 10.0f;
    style.FramePadding = ImVec2(5, 5);
    style.ItemSpacing = ImVec2(8, 8);

    ImGui::StyleColorsDark(); // Base theme
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.6f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.7f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.5f, 0.9f, 1.0f);
}
void DebugGUI::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    
    ApplyGuiStyle();

    io.Fonts->AddFontFromFileTTF("/Library/Fonts/SF-Pro.ttf", 16.0f);
    ImGui_ImplSDLRenderer2_CreateFontsTexture();
}


void DebugGUI::Render(SDL_Renderer* renderer)
{
    if (!guiValues.toggleGui) return;

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    /*ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);*/
    /*ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);*/
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Debug Window", nullptr);
    // =====================================================================================================================
    // Header
    // =====================================================================================================================
    ImGui::BeginChild("Header", ImVec2(0, 150), true);
    // =====================================================================================================================
    // FPS Display
    // =====================================================================================================================
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5)); // Adjust spacing
    ImGui::Columns(2);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::NextColumn();

    // =====================================================================================================================
    // Monitor Display
    // =====================================================================================================================
    ImGui::Text("Monitor Resolution: %dx%d", guiValues.monitorWidth, guiValues.monitorHeight);
    ImGui::Columns(1);
    ImGui::PopStyleVar(); // Reset spacing

    // =====================================================================================================================
    // Mouse Position
    // =====================================================================================================================
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5)); // Adjust spacing
    ImGui::Columns(2);
    ImGui::Text("(%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    ImGui::NextColumn();

    // =====================================================================================================================
    // Vsync Information
    // =====================================================================================================================
    ImGui::Text("Vsync: %s", guiValues.vsync ? "Enabled" : "Disabled");
    ImGui::Columns(1);
    ImGui::PopStyleVar(); // Reset spacing

    // =====================================================================================================================
    // Renderer Name
    // =====================================================================================================================
    ImGui::Text("Renderer: %s", guiValues.rendererName.c_str());

    // =====================================================================================================================
    // Map Scale Slider
    // =====================================================================================================================
    ImGui::SliderFloat("Map Scale", guiValues.mapScale, 1.0f, 10.0f);

    // =====================================================================================================================
    // Recompile & Restart Button
    // =====================================================================================================================
    if (ImGui::Button("Recompile & Restart"))
    {
        std::cout << "🔄 Recompiling & Restarting..." << std::endl;
        system("./../compile.sh &"); // Adjust this command as needed
        exit(0);
    }
    ImGui::EndChild();

    // =====================================================================================================================
    // Debug Tabs
    // =====================================================================================================================
    if (ImGui::BeginTabBar("Debug Tabs"))
    {
        // =====================================================================================================================
        // Log Tab
        // =====================================================================================================================
        if(ImGui::BeginTabItem("Logs"))
        {
            renderLogs();
            ImGui::EndTabItem();
        }

        // =====================================================================================================================
        // Map Tab
        // =====================================================================================================================
        if(ImGui::BeginTabItem("Map"))
        {
            renderMapInfo();
            ImGui::EndTabItem();
        }

        // =====================================================================================================================
        // Layers Tab
        // =====================================================================================================================
        if(ImGui::BeginTabItem("Layers"))
        {
            renderLayerInfo();
            ImGui::EndTabItem();
        }

        // =====================================================================================================================
        // Textures Tab
        // =====================================================================================================================
        if(ImGui::BeginTabItem("Textures"))
        {
            renderTexturesInfo();
            ImGui::EndTabItem();
        }

        // =====================================================================================================================
        // Player Tab
        // =====================================================================================================================
        if(ImGui::BeginTabItem("Player"))
        {
            renderPlayerInfo(renderer);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}


void DebugGUI::SetMapName(const std::string& mapName) { guiValues.mapName = mapName; }
void DebugGUI::SetPlayer(Player* player) { guiValues.player = player; }
void DebugGUI::setMapScale(float *scale) { guiValues.mapScale = scale; }

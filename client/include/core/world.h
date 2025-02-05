#pragma once

#include "client/client.h"
#include "rendering/texture.h"
#include "core/map_loader.h"
#include "block.h"
#include "configs.h"
#include "rendering/sprite.h"
#include <SDL2/SDL.h>
#include <memory>

#ifndef WORLD_H
#define WORLD_H

class World {
private:
    /** Renderer/Window **/
    SDL_Window* window;
    SDL_Renderer* renderer;
    // Blocks
    BlockFactory bf;

    /** 
     * Sprite Handling 
     **/
    Uint32 lastFrameTime = 0;
    const Uint32 frameDelay = 180;  // How Long Each Frame of a Sprite Lasts
    Sprite playerIdleSprite;        // Sprite for Player Idle
    Sprite playerRunSprite;         // Sprite for Player Running
    /** 
     * Player Stuff This Should Be Loaded At The Start 
     *      - Later On May Add Skins For Characters
     **/
    SDL_Texture* playerIdleTexture;     // Texture for Player
    SDL_Texture* playerRunTexture;    // Texture for Running
    
    // House Stuff
    bool enterHouse;                // If Player Is Entering House
    SDL_Texture* house_one_texture;
    SDL_Rect house_one_rect;

    /** 
     * TileSet Atlas 
     *      -- Tiles are 16x16 Grid of Tiles 208x208 Pixels
     *      -- The Way This Works is that each tile represents
     *              a index in the mapData vector starting at 1
     *      -- Special Tiles Have a Special Index meaning negative
     *      -- src/core/map_loader.cpp function parseFile assigns
     *              the index to the tile
     **/
    SDL_Texture * tileAtlasTexture;
    std::vector<SDL_Rect> tiles;
    std::vector<std::vector<int>> mapData;  // Stores Index of Tile In TileSet
    
    MapLoader mapLoader;
    std::unique_ptr<Client> client;
    bool keep_window_open = true;
    std::string currentPath;
public:
    World(Sprite playerIdleSprite, Sprite playerRunSprite, SDL_Renderer* renderer, SDL_Window* window);
    World();

    void handleEvent(SDL_Event e);

    void updateServer(float *oldX, float *oldY);

    void setupWorld();
    void initializeTiles();
    void updateAndRender();

    void renderMap();
    void renderHouse(int x, int y, int width, int height);
    void renderPlayer();
    void renderOtherPlayers();

    // Interactions With The Map
    void checkIfPosIsEnterable();

    // Color functions
    void drawGreen();
    void drawBlue();
    void drawRed();
    void drawYellow();
};

#endif // WORLD_H

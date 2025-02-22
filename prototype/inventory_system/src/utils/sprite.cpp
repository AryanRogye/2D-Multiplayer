#include "utils/sprite.h"
#include "comfy_lib.h"
#include "entity/player.h"

Sprites::Sprites(Player *player)
{
    fetchSpritesConfigs(this);
    if (this->sprites.empty())
    {
        SM_WARN("No sprites found");
        // init a empty sprite (maybe the gui will handle this)
        this->sprites = std::vector<Sprites::Sprite>();
    }
}

// Getters
std::vector<Sprites::Sprite> Sprites::getSpritePaths()
{
    return this->sprites;
}

SDL_Texture *Sprites::getTextures(int index)
{
    return this->getSpritePaths()[index].texture;
}
// Setters
void Sprites::addSpritePath(std::string path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        SM_WARN("Failed to load surface: {0}");
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(SDL_GetRenderer(SDL_GetWindowFromID(1)), surface);
    if (!texture)
    {
        SM_WARN("Failed to load texture: {0}");
        return;
    }
    DebugGUI::addDebugLog("Added SDL Texture\n" + path, false, "SPRITE");

    // Construct the object
    Sprite sprite;
    sprite.path = path;
    sprite.texture = texture;
    // add it to the list
    this->sprites.push_back(sprite);
}
void Sprites::removeSpritePath(int index)
{
    this->sprites.erase(this->sprites.begin() + index);
}

void Sprites::clearSpritePaths()
{
    this->sprites.clear();
}
#pragma once

#include <SDL_render.h>
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

class Player
{
private:
    int x;
    int y;
    int speed;
    int health;
    int damage;
    int level;
    int experience;
    int maxHealth;

    SDL_Renderer *renderer;
public:
    Player();
    Player(int x, int y);

    // Getters
    int getX();
    int getY();
    int getHealth();
    int getDamage();
    int getLevel();
    int getExperience();
    int getMaxHealth();
    SDL_Renderer* getRenderer();
    int getSpeed();
    // Setters
    void setX(int x);
    void setY(int y);
    void setHealth(int health);
    void setDamage(int damage);
    void setLevel(int level);
    void setExperience(int experience);
    void setMaxHealth(int maxHealth);
    void setRenderer(SDL_Renderer *renderer);
    void setSpeed(int speed);
    // Methods
    void loadPlayer();
    void drawPlayer();

    void handleInput(SDL_Event &event);
};

#endif

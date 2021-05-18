#pragma once

#include "entity.h"
#include "game.h"
#include "TextureManager.h"
#include <string>

class UI : public Component
{
public:
    UI(int xpos, int ypos, std::string text, std::string font, SDL_Color& color):
        labelText(text), labelFont(font), textColor(color)
    {
        position.x = xpos;
        position.y = ypos;
        SetText(labelText, labelFont);
    }
    ~UI()
    {}

    void SetText(std::string text, std::string font)
    {
        SDL_Surface* surface = TTF_RenderText_Blended(Game::GetFont(font), text.c_str(), textColor);
        labelTexture = SDL_CreateTextureFromSurface(Game::ren, surface);
        SDL_FreeSurface(surface);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
    }

    void draw() override
    {
        SDL_RenderCopy(Game::ren, labelTexture, nullptr, &position);
    }

private:
    SDL_Rect position;
    std::string labelText;
    std::string labelFont;
    SDL_Color textColor;
    SDL_Texture* labelTexture;
};
#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL_render.h>

#include "engine/displayable.hpp"
#include "engine/types.hpp"

class SDL_Color;
class SDL_Renderer;
class SDL_Texture;
class SDL_Window;

class Renderer {
    private:
        static SDL_Renderer* _renderer;

    public:
        bool init(SDL_Window* p_window);

        static Displayable::DisplayableTexture createDisplayableTexture(const std::string& p_assetName);
        static Displayable::DisplayableTexture createDisplayableText(const std::string& p_content, const Types::DisplayableFont& p_font);

        void update();

    private:
};
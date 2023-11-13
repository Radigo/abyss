#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "drawable.hpp"

class SDL_Renderer;
class SDL_Window;
class SDL_Texture;

class Renderer {
    private:
        static SDL_Renderer* _renderer;
        static vector<Drawable*> _drawList;
        static vector<SDL_Texture*> _textures;

    public:
        bool init(SDL_Window* p_window);

        static void addDraw(Drawable* p_drawable);
        static void addText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize);

        void update();
};
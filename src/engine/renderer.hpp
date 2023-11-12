#pragma once

#include <vector>

#include "drawable.hpp"

class SDL_Renderer;
class SDL_Window;

class Renderer {
    private:
        SDL_Renderer* _renderer;
        static vector<Drawable*> _drawList;

    public:
        bool init(SDL_Window* p_window);

        static void addDraw(Drawable* p_drawable);

        void update();
};
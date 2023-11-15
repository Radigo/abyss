#pragma once

#include <vector>
#include <string>

class SDL_Color;
class SDL_Renderer;
class SDL_Texture;
class SDL_Window;

class Renderer {
    private:
        static SDL_Renderer* _renderer;

    public:
        bool init(SDL_Window* p_window);

        void update();

    private:
        static void _addText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize);
};
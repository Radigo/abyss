#pragma once

#include <vector>
#include <string>

class Displayable;
class Drawable;
class SDL_Color;
class SDL_Renderer;
class SDL_Texture;
class SDL_Window;

class Renderer {
    private:
        static SDL_Renderer* _renderer;
        static std::vector<Displayable*> _displayList;
        static std::vector<SDL_Texture*> _textures;

    public:
        bool init(SDL_Window* p_window);

        static void add(Displayable* p_displayable);

        void update();

    private:
        static void _addText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize);
};
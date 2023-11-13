#pragma once

#include <string>

#include "ui/closebutton.hpp"
#include "ui/window.hpp"

class SDL_Texture;
class SDL_Color;
class SDL_Renderer;

using namespace std;

class Scene {
    private:
        Window* _window;
        CloseButton* _closeButton;

    public:
        bool init();
};
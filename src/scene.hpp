#pragma once

#include "ui/closebutton.hpp"
#include "ui/window.hpp"

class Scene {
    private:
        Window* _window;
        CloseButton* _closeButton;

    public:
        bool init();
};
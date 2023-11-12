#pragma once

#include "engine/drawable.hpp"

class Scene {
    private:
        Drawable* _draw1;
        Drawable* _draw2;

    public:
        bool init();
};
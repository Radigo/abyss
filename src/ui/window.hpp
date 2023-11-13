#pragma once

#include "../engine/drawable.hpp"

class Window : public Drawable {
    public:
        Window(const int p_width, const int p_height);
        ~Window();
};
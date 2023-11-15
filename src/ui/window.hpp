#pragma once

#include "engine/displayable.hpp"

class Window : public Displayable {
    public:
        Window(const int p_width, const int p_height);
        ~Window();
};
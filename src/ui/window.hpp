#pragma once

#include "engine/controllable.hpp"
#include "engine/displayable.hpp"

class Window : public Displayable, Controllable {
    public:
        Window(const int p_width, const int p_height);
        ~Window();
};
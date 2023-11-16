#pragma once

#include "engine/gameobject.hpp"

class Displayable;

class Window : public GameObject {
    public:
        Window(const int p_width, const int p_height);
        ~Window();

    private:
        Displayable* _display;
};
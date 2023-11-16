#pragma once

#include "engine/gameobject.hpp"

class Displayable;

class CloseButton : public GameObject {
    static constexpr int CLOSE_BUTTON_SIZE = 20;
    public:
        CloseButton();
        ~CloseButton();

    private:
        Displayable* _display;
};
#pragma once

#include "engine/gameobject.hpp"

#include <functional>

class Controllable;
class Displayable;

class CloseButton : public GameObject {
    public:
        CloseButton(GameObject* p_parent, const int p_size, std::function<void()> p_callback);
        ~CloseButton();

    private:
        Displayable* _display;
        Controllable* _control;
};
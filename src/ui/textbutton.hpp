#pragma once

#include "engine/gameobject.hpp"

#include <functional>
#include <string>

class Controllable;
class Displayable;

class TextButton : public GameObject {
    public:
        TextButton(GameObject* p_parent, const std::string& p_content, std::function<void()> p_callback);
        ~TextButton();

    private:
        static const int MARGIN = 4;
        Displayable* _label;
        Displayable* _display;
        Controllable* _control;
};
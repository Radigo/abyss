#pragma once

#include "engine/gameobject.hpp"
#include "engine/types.hpp"

#include "ui/closebutton.hpp"

class Displayable;

class Window : public GameObject {
    static constexpr int TOP_BAR_SIZE = 20;
    static constexpr int CLOSE_BUTTON_SIZE = 16;

    public:
        Window(std::string p_id, const int p_width, const int p_height, std::function<void()> p_onCloseCallback);
        ~Window();

    private:
        Displayable* _display;
        CloseButton* _closeButton;
        Controllable* _dragArea;

        Types::Point _dragOrigin = Types::Point(0, 0);
};
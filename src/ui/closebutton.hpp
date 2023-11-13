#pragma once

#include "../engine/drawable.hpp"

class CloseButton : public Drawable {
    static constexpr int CLOSE_BUTTON_SIZE = 20;
    public:
        CloseButton();
        ~CloseButton();
};
#pragma once

#include "engine/displayable.hpp"

class CloseButton : public Displayable {
    static constexpr int CLOSE_BUTTON_SIZE = 20;
    public:
        CloseButton();
        ~CloseButton();
};
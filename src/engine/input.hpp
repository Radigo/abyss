#pragma once

#include <SDL2/SDL_events.h>

#include <vector>

class Displayable;

class Input {
    public:
        void onMouseMove(SDL_MouseMotionEvent p_event);
        void onMouseButtonDown(SDL_MouseButtonEvent p_event);
        void onMouseButtonUp(SDL_MouseButtonEvent p_event);
        void onMouseWheel(SDL_MouseWheelEvent p_event);
};
#include "input.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>

#include <vector>

#include "engine/controllable.hpp"

void Input::onMouseMove(SDL_MouseMotionEvent p_event) {
    for (size_t i = 0; i < Controllable::controllableList.size(); i++) {
        // List may have changed in size
        if (i >= Controllable::controllableList.size())
            return;
        Controllable* controllable = Controllable::controllableList.at(i);
        if (controllable)
            controllable->onMouseMove(Types::Point(p_event.x, p_event.y));
    }
}

void Input::onMouseButtonDown(SDL_MouseButtonEvent p_event) {
    for (size_t i = 0; i < Controllable::controllableList.size(); i++) {
        // List may have changed in size
        if (i >= Controllable::controllableList.size())
            return;
        Controllable* controllable = Controllable::controllableList.at(i);
        if (controllable)
            controllable->onMouseButtonDown(Types::Point(p_event.x, p_event.y));
    }
}

void Input::onMouseButtonUp(SDL_MouseButtonEvent p_event) {
    for (size_t i = 0; i < Controllable::controllableList.size(); i++) {
        // List may have changed in size
        if (i >= Controllable::controllableList.size())
            return;
        Controllable* controllable = Controllable::controllableList.at(i);
        if (controllable)
            controllable->onMouseButtonUp(Types::Point(p_event.x, p_event.y));
    }
}

#include "input.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>

#include <vector>

#include "engine/controllable.hpp"

void Input::onMouseMove(SDL_MouseMotionEvent p_event) {
    for (Controllable* controllable : Controllable::controllableList) {
        // ToDo: Check if we are inside a controllabe
        SDL_Log("Mouse moving");
    }
}

void Input::onMouseButtonDown(SDL_MouseButtonEvent p_event) {
    for (Controllable* controllable : Controllable::controllableList) {
        // ToDo: Check if we are inside a controllable
        SDL_Log("Mouse button pressed");
    }
}

void Input::onMouseButtonUp(SDL_MouseButtonEvent p_event) {
    for (Controllable* controllable : Controllable::controllableList) {
        // ToDo: Check if we are inside a controllable
        SDL_Log("Mouse button released");
    }
}

void Input::onMouseWheel(SDL_MouseWheelEvent p_event) {

}
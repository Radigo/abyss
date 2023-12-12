#include "input.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>

#include <vector>

#include "engine/controllable.hpp"

std::vector<Controllable*> Input::_controllableList;

void Input::onMouseMove(SDL_MouseMotionEvent p_event) {
    for (size_t i = 0; i < _controllableList.size(); i++) {
        // List may have changed in size
        if (i >= _controllableList.size())
            return;
        Controllable* controllable = _controllableList.at(i);
        if (controllable)
            controllable->onMouseMove(Types::Point(p_event.x, p_event.y));
    }
}

void Input::onMouseButtonDown(SDL_MouseButtonEvent p_event) {
    for (size_t i = 0; i < _controllableList.size(); i++) {
        // List may have changed in size
        if (i >= _controllableList.size())
            return;
        Controllable* controllable = _controllableList.at(i);
        if (controllable)
            controllable->onMouseButtonDown(Types::Point(p_event.x, p_event.y));
    }
}

void Input::onMouseButtonUp(SDL_MouseButtonEvent p_event) {
    for (size_t i = 0; i < _controllableList.size(); i++) {
        // List may have changed in size
        if (i >= _controllableList.size())
            return;
        Controllable* controllable = _controllableList.at(i);
        if (controllable)
            controllable->onMouseButtonUp(Types::Point(p_event.x, p_event.y));
    }
}

void Input::onKey(SDL_KeyboardEvent p_event) {
    // Don't bother with remapping (yet)
    for (size_t i = 0; i < _controllableList.size(); i++) {
        // List may have changed in size
        if (i >= _controllableList.size())
            return;
        Controllable* controllable = _controllableList.at(i);
        switch (p_event.keysym.sym) {
            case SDLK_UP:
                controllable->onUp(p_event.type == SDL_KEYDOWN);
                break;
            case SDLK_DOWN:
                controllable->onDown(p_event.type == SDL_KEYDOWN);
                break;
            case SDLK_LEFT:
                controllable->onLeft(p_event.type == SDL_KEYDOWN);
                break;
            case SDLK_RIGHT:
                controllable->onRight(p_event.type == SDL_KEYDOWN);
                break;
            case SDLK_w:
                controllable->onButtonA(p_event.type == SDL_KEYDOWN);
                break;
            case SDLK_x:
                controllable->onButtonB(p_event.type == SDL_KEYDOWN);
                break;
            case SDLK_c:
                controllable->onButtonC(p_event.type == SDL_KEYDOWN);
                break;
            case SDLK_RETURN:
                controllable->onButtonStart(p_event.type == SDL_KEYDOWN);
                break;
        }
    }
}
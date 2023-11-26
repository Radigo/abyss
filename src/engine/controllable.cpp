#include "controllable.hpp"

#include <SDL2/SDL_log.h>

#include "input.hpp"

Controllable::Controllable(GameObject* p_parent) :
_hitbox(0, 0, -1, -1),
_isDownInside(false),
_onClickCallback(nullptr),
_onButtonDownCallback(nullptr),
_onDragCallback(nullptr)
{
    size_t numControllables = Input::addControllable(this);
    _id = "Controllable" + std::to_string(numControllables);
    _parent = p_parent;
}

Controllable::~Controllable() {
    SDL_Log("<Controllable::~Controllable> %s", getId().c_str());
    
    _onClickCallback = nullptr;
    _onButtonDownCallback = nullptr;
    _onDragCallback = nullptr;

    Input::removeControllable(this);
}

void Controllable::onMouseButtonDown(Types::Point p_point) {
    if (!isInside(p_point)) {
        return;
    }
    
    if (_onButtonDownCallback)
        _onButtonDownCallback(p_point);
    
    _isDownInside = true;
}

void Controllable::onMouseButtonUp(Types::Point p_point) {
    if (!isInside(p_point)) {
        return;
    }
    
    if (_isDownInside && _onClickCallback)
        _onClickCallback();

    _isDownInside = false;
}

void Controllable::onMouseMove(Types::Point p_point) {
    if (_isDownInside && _onDragCallback)
        _onDragCallback(p_point);
}
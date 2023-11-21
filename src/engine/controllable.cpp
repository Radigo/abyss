#include "controllable.hpp"

#include <SDL2/SDL_log.h>

std::vector<Controllable*> Controllable::controllableList;

Controllable::Controllable(GameObject* p_parent) :
_hitbox(0, 0, -1, -1),
_isDownInside(false),
_onClickCallback(nullptr),
_onButtonDownCallback(nullptr),
_onDragCallback(nullptr)
{
    Controllable::controllableList.emplace_back(this);
    _id = "Controllable" + std::to_string(Controllable::controllableList.size());
    _parent = p_parent;
}

Controllable::~Controllable() {
    SDL_Log("<Controllable::~Controllable> %s", getId().c_str());
    
    _onClickCallback = nullptr;
    _onButtonDownCallback = nullptr;
    _onDragCallback = nullptr;

    auto it = std::find(Controllable::controllableList.begin(), Controllable::controllableList.end(), this);
    if (it == Controllable::controllableList.end())
        static_assert(true, "removing controllable that is NOT in Controllable::controllableList");
    Controllable::controllableList.erase(it);
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
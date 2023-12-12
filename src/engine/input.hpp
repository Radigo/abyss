#pragma once

#include <SDL2/SDL_events.h>

#include <algorithm>
#include <vector>

class Controllable;

class Input {
    private:
        static std::vector<Controllable*> _controllableList;

    public:
        inline static size_t addControllable(Controllable* p_controllable) {
            _controllableList.push_back(p_controllable);
            return _controllableList.size();
        }
        inline static size_t removeControllable(Controllable* p_controllable) {
            auto it = std::find(_controllableList.begin(), _controllableList.end(), p_controllable);
            if (it == _controllableList.end())
                static_assert(true, "removing Updatable that is NOT in Input::_controllableList");
            _controllableList.erase(it);
            return _controllableList.size();
        }

        void onMouseMove(SDL_MouseMotionEvent p_event);
        void onMouseButtonDown(SDL_MouseButtonEvent p_event);
        void onMouseButtonUp(SDL_MouseButtonEvent p_event);
        void onKey(SDL_KeyboardEvent p_event);
};
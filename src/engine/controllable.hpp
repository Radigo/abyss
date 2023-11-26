#pragma once

#include "engine/gameobject.hpp"
#include "engine/types.hpp"

#include <functional>

class Controllable : public GameObject {
    public:
        Controllable(GameObject* p_parent);
        ~Controllable();

        inline void setHitbox(Types::Rectangle p_rect) { _hitbox = p_rect; }
        inline void setOnClick(std::function<void()> p_callback) { _onClickCallback = p_callback; };
        inline void setOnButtonDown(std::function<void(Types::Point)> p_callback) { _onButtonDownCallback = p_callback; };
        inline void setOnDragMove(std::function<void(Types::Point)> p_callback) { _onDragCallback = p_callback; };

        // Get those from input
        void onMouseButtonDown(Types::Point p_point);
        void onMouseButtonUp(Types::Point p_point);
        void onMouseMove(Types::Point p_point);

    private:
        Types::Rectangle _hitbox;
        bool _isDownInside;

        std::function<void()> _onClickCallback;
        std::function<void(Types::Point)> _onButtonDownCallback;
        std::function<void(Types::Point)> _onDragCallback;

        inline bool isInside(Types::Point p_point) {
            if (_hitbox.isValid()) {
                return ((p_point.x >= _parent->getX() + _hitbox.left())
                    && (p_point.x <= _parent->getX() + _hitbox.right())
                    && (p_point.y >= _parent->getY() + _hitbox.top())
                    && (p_point.y <= _parent->getY() + _hitbox.bottom()));
            }

            return true;
        }
};
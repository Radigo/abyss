#pragma once

#include "engine/gameobject.hpp"
#include "engine/types.hpp"

#include <functional>

class Controllable : public GameObject {
    public:
        static constexpr int UP         = 1 << 0;
        static constexpr int DOWN       = 1 << 1;
        static constexpr int LEFT       = 1 << 2;
        static constexpr int RIGHT      = 1 << 3;

        static constexpr int BTN_A      = 1 << 3;
        static constexpr int BTN_B      = 1 << 4;
        static constexpr int BTN_C      = 1 << 5;
        static constexpr int BTN_START  = 1 << 6;
        
        Controllable(GameObject* p_parent);
        ~Controllable();

        inline void setHitbox(Types::Rectangle p_rect) { _hitbox = p_rect; }
        inline void setOnClick(std::function<void()> p_callback) { _onClickCallback = p_callback; };
        inline void setOnMouseButtonDown(std::function<void(Types::Point)> p_callback) { _onMouseButtonDownCallback = p_callback; };
        inline void setOnDragMove(std::function<void(Types::Point)> p_callback) { _onDragCallback = p_callback; };

        // Get those from input
        void onMouseButtonDown(Types::Point p_point);
        void onMouseButtonUp(Types::Point p_point);
        void onMouseMove(Types::Point p_point);
        inline void onUp(bool p_flag) { if (p_flag) _moveState |= UP; else _moveState &= ~UP; }
        inline void onDown(bool p_flag) { if (p_flag) _moveState |= DOWN; else _moveState &= ~DOWN; }
        inline void onLeft(bool p_flag) { if (p_flag) _moveState |= LEFT; else _moveState &= ~LEFT; }
        inline void onRight(bool p_flag) { if (p_flag) _moveState |= RIGHT; else _moveState &= ~RIGHT; }
        inline void onButtonA(bool p_flag) { if (p_flag) _buttonState |= BTN_A; else _buttonState &= ~BTN_A; }
        inline void onButtonB(bool p_flag) { if (p_flag) _buttonState |= BTN_B; else _buttonState &= ~BTN_B; }
        inline void onButtonC(bool p_flag) { if (p_flag) _buttonState |= BTN_C; else _buttonState &= ~BTN_C; }
        inline void onButtonStart(bool p_flag) { if (p_flag) _buttonState |= BTN_START; else _buttonState &= ~BTN_START; }

        inline int getMoveState() { return _moveState; }
        inline int getButtonState() { return _buttonState; }

    private:
        Types::Rectangle _hitbox;
        bool _isDownInside;
        int _moveState;
        int _buttonState;

        std::function<void()> _onClickCallback;
        std::function<void(Types::Point)> _onMouseButtonDownCallback;
        std::function<void(Types::Point)> _onDragCallback;

        bool isInside(Types::Point p_point) {
            if (_hitbox.isValid()) {
                return ((p_point.x >= getX() + _hitbox.left())
                    && (p_point.x <= getX() + _hitbox.right())
                    && (p_point.y >= getY() + _hitbox.top())
                    && (p_point.y <= getY() + _hitbox.bottom()));
            }

            return true;
        }
};
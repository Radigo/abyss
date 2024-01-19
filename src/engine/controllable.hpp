#pragma once

#include "engine/gameobject.hpp"
#include "engine/types.hpp"

#include <functional>

class Controllable : public GameObject {
    public:
        static constexpr int NONE       = 1 << 0;

        static constexpr int UP         = 1 << 1;
        static constexpr int DOWN       = 1 << 2;
        static constexpr int LEFT       = 1 << 3;
        static constexpr int RIGHT      = 1 << 4;

        static constexpr int BTN_A      = 1 << 5;
        static constexpr int BTN_B      = 1 << 6;
        static constexpr int BTN_C      = 1 << 7;
        static constexpr int BTN_START  = 1 << 8;
        
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
        inline void onUp(bool p_flag) { if (p_flag) _inputState |= UP; else _inputState &= ~UP; }
        inline void onDown(bool p_flag) { if (p_flag) _inputState |= DOWN; else _inputState &= ~DOWN; }
        inline void onLeft(bool p_flag) { if (p_flag) _inputState |= LEFT; else _inputState &= ~LEFT; }
        inline void onRight(bool p_flag) { if (p_flag) _inputState |= RIGHT; else _inputState &= ~RIGHT; }
        inline void onButtonA(bool p_flag) { if (p_flag) _inputState |= BTN_A; else _inputState &= ~BTN_A; }
        inline void onButtonB(bool p_flag) { if (p_flag) _inputState |= BTN_B; else _inputState &= ~BTN_B; }
        inline void onButtonC(bool p_flag) { if (p_flag) _inputState |= BTN_C; else _inputState &= ~BTN_C; }
        inline void onButtonStart(bool p_flag) { if (p_flag) _inputState |= BTN_START; else _inputState &= ~BTN_START; }

        // States
        inline void setInputState(int p_inpState) { _inputState = p_inpState; }
        inline int getInputState() { return _inputState; }

    private:
        Types::Rectangle _hitbox;
        bool _isDownInside;
        int _inputState;

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
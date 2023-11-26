#pragma once

#include <string>

class GameObject {
    protected:
        GameObject* _parent;
        std::string _id;
        int _x;
        int _y;

    public:
        GameObject();
        ~GameObject();

        // Setters
        inline void setPosition(const int p_x, const int p_y) {
            _x = p_x;
            _y = p_y;
        }
        inline void setParent(GameObject* p_gameobject) {
            _parent = p_gameobject;
        };

        // Getters
        inline std::string getId() { return _id; }
        inline int getX() {
            if (_parent != nullptr)
                 return _x + _parent->getX();
            return _x;
        };
        inline int getY() {
            if (_parent != nullptr)
                return _y + _parent->getY();
            return _y;
        };
        inline GameObject* getParent() {
            return _parent;
        }
};
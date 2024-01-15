#pragma once

#include <algorithm>
#include <string>

class GameObject {
    private:
        static std::vector<std::pair<GameObject*, GameObject*>> _hierarchy;// The full map of relationships between parents (first) and children (second)

    protected:
        std::string _id;
        int _x;
        int _y;

    public:
        GameObject(GameObject* p_parent);
        virtual ~GameObject();// Ensure destructor is propagated through the chain of inheritance

        // Setters
        void setPosition(const int p_x, const int p_y);
        void setParent(GameObject* p_gameobject);
        GameObject* getParent();
        std::vector<GameObject*> getChildren();

        // Getters
        inline std::string getId() { return _id; }
        inline int getX() {
            if (getParent() != nullptr)
                 return _x + getParent()->getX();
            return _x;
        };
        inline int getY() {
            if (getParent() != nullptr)
                return _y + getParent()->getY();
            return _y;
        };
};
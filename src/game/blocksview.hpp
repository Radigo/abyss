#pragma once

#include "engine/displayable.hpp"
#include "engine/gameobject.hpp"

class Blocks;
class Color;
class Updatable;

class BlocksView : public GameObject {
    public:
        BlocksView(GameObject* p_parent, Blocks* p_game, const int& p_cellSize);
        ~BlocksView();

    private:
        int _cellSize;
        Blocks* _game;
        Displayable* _display;
        Updatable* _updater;

        void _drawPlayfield();
        Displayable::Color _getBlockColor(const std::pair<int, float>& p_colorIndex);
};
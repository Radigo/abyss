#pragma once

#include "engine/displayable.hpp"
#include "engine/gameobject.hpp"

class Blocks;
class Color;
class Updatable;

class BlocksView : public GameObject {
    public:
        BlocksView(GameObject* p_parent, Blocks* p_game);
        ~BlocksView();

    private:
        static const int CELL_SIZE = 32;

        Blocks* _game;
        Displayable* _display;
        Displayable* _debugTf;
        size_t _tfIndex;
        Updatable* _updater;

        void _drawPlayfield();
        Displayable::Color _getBlockColor(const int& p_colorIndex);
};
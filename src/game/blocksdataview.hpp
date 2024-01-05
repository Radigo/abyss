#pragma once

#include "engine/displayable.hpp"
#include "engine/gameobject.hpp"

class Blocks;
class Color;
class Updatable;

class BlocksDataView : public GameObject {
    public:
        BlocksDataView(GameObject* p_parent, Blocks* p_game);
        ~BlocksDataView();

    private:
        Blocks* _game;
        Displayable* _nextTf;
        size_t _nextTfIndex;
        Displayable* _levelTf;
        size_t _levelTfIndex;
        Displayable* _debugTf;
        size_t _debugTfIndex;

        Updatable* _updater;

        void _drawData();
        Displayable::Color _getBlockColor(const std::string& p_pieceTypeStr);
};
#pragma once

#include "engine/displayable.hpp"
#include "engine/gameobject.hpp"

class Bot;
class Blocks;
class Color;
class Updatable;

class BotDataView : public GameObject {
    public:
        BotDataView(GameObject* p_parent, Blocks* p_game, Bot* p_bot, int p_cellSize);
        ~BotDataView();

    private:
        Blocks* _game;
        Bot* _bot;
        Displayable* _target;
        Displayable* _horizon;

        int _cellSize;

        Updatable* _updater;

        void _drawData();
};
#include "botdataview.hpp"

// Debug for bot

#include "game/blocks.hpp"

#include "bot/bot.hpp"
#include "engine/displayable.hpp"
#include "engine/renderer.hpp"
#include "engine/updatable.hpp"

#include "SDL2/SDL_log.h"

BotDataView::BotDataView(GameObject* p_parent, Blocks* p_game, Bot* p_bot, int p_cellSize) :
GameObject(p_parent),
_game(p_game),
_bot(p_bot),
_cellSize(p_cellSize)
{
    _target = new Displayable(this);
    _target->setPosition(0, 0);
    _horizon = new Displayable(this);
    _horizon->setPosition(0, 0);

    _updater = new Updatable(this, [this](const double&){ _drawData(); });
}

BotDataView::~BotDataView() {

}

void BotDataView::_drawData() {
    // Draw target piece
    _target->clear();
    // result is a grid with just the necessary blocks to fill in the playfield coordinates
    auto targetCoordinates = _bot->getDebugTarget();
    
    for (size_t rowIdx = 0; rowIdx < targetCoordinates.size(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < targetCoordinates.at(rowIdx).size(); colIdx++) {
            int blockColorIndex = targetCoordinates.at(rowIdx).at(colIdx);
            if (blockColorIndex >= 0) {
                // Draw a cell of the target piece
                _target->addRectangle(colIdx * _cellSize, rowIdx * _cellSize, _cellSize, _cellSize, Displayable::Color(0, 0, 0, 0), _getBlockColor(blockColorIndex));
            }
        }
    }

    // Draw horizon
    // result is a grid with just the necessary blocks to fill in the playfield coordinates
    auto horizonCoordinates = _bot->getDebugHorizon();

    for (size_t rowIdx = 0; rowIdx < horizonCoordinates.size(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < horizonCoordinates.at(rowIdx).size(); colIdx++) {
            int blockColor = horizonCoordinates.at(rowIdx).at(colIdx);
            if (blockColor >= 0) {
                // Draw a cell of the target piece
                _target->addRectangle(colIdx * _cellSize, rowIdx * _cellSize, _cellSize, _cellSize, Displayable::Color(0, 0, 0, 0), Displayable::Color(255, 255, 255, 32));
            }
        }
    }
}

Displayable::Color BotDataView::_getBlockColor(const int& p_colorIndex) {
    Displayable::Color displayableColor = Displayable::Color(0, 0, 0, 0);

    if (p_colorIndex == 0) {
        // Red
        displayableColor = Displayable::Color(255, 0, 0, 32);
    } else if (p_colorIndex == 1) {
        // Cyan
        displayableColor = Displayable::Color(0, 255, 255, 32);
    } else if (p_colorIndex == 2) {
        // Orange
        displayableColor = Displayable::Color(255, 128, 0, 32);
    } else if (p_colorIndex == 3) {
        // Blue
        displayableColor = Displayable::Color(0, 0, 255, 32);
    } else if (p_colorIndex == 4) {
        // Violet
        displayableColor = Displayable::Color(255, 0, 255, 32);
    } else if (p_colorIndex == 5) {
        // Green
        displayableColor = Displayable::Color(0, 255, 0, 32);
    } else if (p_colorIndex == 6) {
        // Yellow
        displayableColor = Displayable::Color(255, 255, 0, 32);
    }

    return displayableColor;
}
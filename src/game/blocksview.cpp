#include "blocksview.hpp"

// Visual representation of 'blocks' game

#include "game/blocks.hpp"

#include "engine/displayable.hpp"
#include "engine/renderer.hpp"
#include "engine/updatable.hpp"

#include "SDL2/SDL_log.h"

BlocksView::BlocksView(GameObject* p_parent, Blocks* p_game) :
_game(p_game)
{
    _parent = p_parent;

    _display = new Displayable(this);
    _debugTf = new Displayable(this);
    _tfIndex = _debugTf->addTexture(Renderer::createDisplayableText("0000000000", Types::getFont(Types::Fonts::Regular)));
    _debugTf->setPosition(0, 20 * CELL_SIZE);

    _updater = new Updatable(this, [this](const double& p_deltaTime){ _drawPlayfield(); });
}

BlocksView::~BlocksView() {

}

void BlocksView::_drawPlayfield() {
    _display->clear();

    const auto gamePlayfield = _game->getPlayfield();

    for (size_t rowIdx = 0; rowIdx < gamePlayfield.size(); rowIdx++) {
        auto row = gamePlayfield.at(rowIdx);
        for (size_t colIdx = 0; colIdx < row.size(); colIdx++) {
            Displayable::Color cellColor = _getBlockColor(row.at(colIdx));

            // Draw cell at position
            _display->addRectangle(colIdx * CELL_SIZE, rowIdx * CELL_SIZE, CELL_SIZE, CELL_SIZE, Displayable::Color(255, 255, 255, 32), cellColor);
        }
    }

    // Debug display
    std:string message = "level: " + to_string(_game->getLevel());
    message += ", frame: " + to_string(_game->getFrameTick());
    message += ", state: ";
    int gameState = _game->getState();
    if (gameState == 0) {// SPAWN_TETROMINO
        message += "SPAWN";
    } else if (gameState == 1) {// MOVE_TETROMINO
        message += "MOVE";
    } else if (gameState == 2) {// CLEAR_LINE
        message += "CLEAR";
    } else if (gameState == 3) {// GAME_OVER
        message += "GAME OVER";
    }
    _debugTf->updateTexture(_tfIndex, Renderer::createDisplayableText(message, Types::getFont(Types::Fonts::Regular)));
}

Displayable::Color BlocksView::_getBlockColor(const int& p_colorIndex) {
    if (p_colorIndex == 0) {
        // Red
        return Displayable::Color(255, 0, 0, 255);
    } else if (p_colorIndex == 1) {
        // Cyan
        return Displayable::Color(0, 255, 255, 255);
    } else if (p_colorIndex == 2) {
        // Orange
        return Displayable::Color(255, 128, 0, 255);
    } else if (p_colorIndex == 3) {
        // Blue
        return Displayable::Color(0, 0, 255, 255);
    } else if (p_colorIndex == 4) {
        // Violet
        return Displayable::Color(255, 0, 255, 255);
    } else if (p_colorIndex == 5) {
        // Green
        return Displayable::Color(0, 255, 0, 255);
    } else if (p_colorIndex == 6) {
        // Yellow
        return Displayable::Color(255, 255, 0, 255);
    }

    return Displayable::Color(0, 0, 0, 0);
}
#include "blocksview.hpp"

// Visual representation of 'blocks' game

#include "game/blocks.hpp"

#include "engine/displayable.hpp"
#include "engine/renderer.hpp"
#include "engine/updatable.hpp"

#include "SDL2/SDL_log.h"

BlocksView::BlocksView(GameObject* p_parent, Blocks* p_game, const int& p_cellSize) :
GameObject(p_parent),
_cellSize(p_cellSize),
_game(p_game)
{
    _display = new Displayable(this);
    _updater = new Updatable(this, [this](const double&){ _drawPlayfield(); });
}

BlocksView::~BlocksView() {

}

void BlocksView::_drawPlayfield() {
    _display->clear();

    const auto gamePlayfield = _game->getPlayfield();

    for (size_t rowIdx = Blocks::BUFFER_ROWS; rowIdx < gamePlayfield.size(); rowIdx++) {
        auto row = gamePlayfield.at(rowIdx);
        for (size_t colIdx = 0; colIdx < row.size(); colIdx++) {
            Displayable::Color cellColor = _getBlockColor(row.at(colIdx));

            // Draw cell at position
            _display->addRectangle(colIdx * _cellSize, rowIdx * _cellSize, _cellSize, _cellSize, Displayable::Color(255, 255, 255, 32), cellColor);
        }
    }
}

Displayable::Color BlocksView::_getBlockColor(const std::pair<int, float>& p_colorIndex) {
    const int& colorIndex = p_colorIndex.first;
    const float& lockRatio = p_colorIndex.second;
    Displayable::Color displayableColor = Displayable::Color(0, 0, 0, 0);

    if (colorIndex == 0) {
        // Red
        displayableColor = Displayable::Color(255, 0, 0, 255);
    } else if (colorIndex == 1) {
        // Cyan
        displayableColor = Displayable::Color(0, 255, 255, 255);
    } else if (colorIndex == 2) {
        // Orange
        displayableColor = Displayable::Color(255, 128, 0, 255);
    } else if (colorIndex == 3) {
        // Blue
        displayableColor = Displayable::Color(0, 0, 255, 255);
    } else if (colorIndex == 4) {
        // Violet
        displayableColor = Displayable::Color(255, 0, 255, 255);
    } else if (colorIndex == 5) {
        // Green
        displayableColor = Displayable::Color(0, 255, 0, 255);
    } else if (colorIndex == 6) {
        // Yellow
        displayableColor = Displayable::Color(255, 255, 0, 255);
    }

    displayableColor.lighten(lockRatio);

    return displayableColor;
}
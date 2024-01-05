#include "blocksdataview.hpp"

// HUD for blocks

#include "game/blocks.hpp"

#include "engine/displayable.hpp"
#include "engine/renderer.hpp"
#include "engine/updatable.hpp"

#include "SDL2/SDL_log.h"

BlocksDataView::BlocksDataView(GameObject* p_parent, Blocks* p_game) :
_game(p_game)
{
    _parent = p_parent;

    _nextTf = new Displayable(this);
    _nextTfIndex = _nextTf->addTexture(Renderer::createDisplayableText("NEXT:", Types::getFont(Types::Fonts::Regular)));
    _nextTf->setPosition(0, 0);
    _levelTf = new Displayable(this);
    _levelTfIndex = _levelTf->addTexture(Renderer::createDisplayableText("0/100", Types::getFont(Types::Fonts::RegularDouble)));
    _levelTf->setPosition(0, 40);
    _debugTf = new Displayable(this);
    _debugTfIndex = _debugTf->addTexture(Renderer::createDisplayableText("0000000000", Types::getFont(Types::Fonts::Regular)));
    _debugTf->setPosition(0, 60);

    _updater = new Updatable(this, [this](const double& p_deltaTime){ _drawData(); });
}

BlocksDataView::~BlocksDataView() {

}

void BlocksDataView::_drawData() {
    // Draw next piece
    std::string nextStr = _game->getNextStr();
    Displayable::Color nextColor = _getBlockColor(nextStr);
    _nextTf->updateTexture(_nextTfIndex, Renderer::createDisplayableText("NEXT: " + nextStr, Types::getFont(Types::Fonts::Regular), nextColor));

    // Draw level
    int level = _game->getLevel();
    int levelMilestone = ((level / 100) * 100) + 100;
    std:string message = to_string(level) + "/" + to_string(levelMilestone);
    _levelTf->updateTexture(_levelTfIndex, Renderer::createDisplayableText(message, Types::getFont(Types::Fonts::Regular)));

    // Draw debug
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
    message += ", frame: " + to_string(_game->getFrameTick());
    _debugTf->updateTexture(_debugTfIndex, Renderer::createDisplayableText(message, Types::getFont(Types::Fonts::Regular)));
}

Displayable::Color BlocksDataView::_getBlockColor(const std::string& p_pieceTypeStr) {
    Displayable::Color displayableColor = Displayable::Color(0, 0, 0, 0);

    if (p_pieceTypeStr == "I") {
        // Red
        displayableColor = Displayable::Color(255, 0, 0, 255);
    } else if (p_pieceTypeStr == "T") {
        // Cyan
        displayableColor = Displayable::Color(0, 255, 255, 255);
    } else if (p_pieceTypeStr == "L") {
        // Orange
        displayableColor = Displayable::Color(255, 128, 0, 255);
    } else if (p_pieceTypeStr == "J") {
        // Blue
        displayableColor = Displayable::Color(0, 0, 255, 255);
    } else if (p_pieceTypeStr == "S") {
        // Violet
        displayableColor = Displayable::Color(255, 0, 255, 255);
    } else if (p_pieceTypeStr == "Z") {
        // Green
        displayableColor = Displayable::Color(0, 255, 0, 255);
    } else if (p_pieceTypeStr == "O") {
        // Yellow
        displayableColor = Displayable::Color(255, 255, 0, 255);
    }

    return displayableColor;
}
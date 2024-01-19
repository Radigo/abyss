#include "bot.hpp"

#include "engine/controllable.hpp"
#include "engine/updatable.hpp"
#include "game/blocks.hpp"

#include <SDL2/SDL_log.h>

Bot::Bot(Blocks* p_game) :
_game(p_game),
_updater(nullptr),
_inputs({})
{
    // Connects to an existing game of Blocks(tm) and plays it like a boss

    /*
    ToDo:
    read status from game > ok
    read next piece from game
    read all placement possibilities in playfield from game
    pick one (temp random)
    count max frames until lock at position
    determine path to position > build inputs
    send inputs to game
    */

    _updater = new Updatable(nullptr, [this](const double& p_frameDeltaTime){ _updateBot(p_frameDeltaTime); });
}

Bot::~Bot() {

}

void Bot::_updateBot(const double& p_frameDeltaTime) {
    switch (_game->getState()) {
        case Blocks::SPAWN_TETROMINO:
            if (_game->getFrameTick() == 1) {
                Target target = _getTarget(_game->getNext());
                // Get how many frames until lock
                size_t numFrames = _game->getPlayfield().size() * Blocks::GRAVITY_1_G / _game->getGravity();
                SDL_Log("numFrames to position %d, %d: %zu", target.x, target.y, numFrames);
                _inputs = _generateInputs(target, numFrames);
            }
            break;
        case Blocks::MOVE_TETROMINO:
            if (!_inputs.empty()) {
                int input = _inputs.at(0);
                _game->getInput()->setInputState(input);
                _inputs.erase(_inputs.begin());
            }
            break;
        case Blocks::CLEAR_LINE:
            break;
        case Blocks::GAME_OVER:
            break;
    }
}

/** Returns the optimal position of the next piece on the playfield
 * Returned data are the coordinates of the piece x & y, and the rotation value
*/
Bot::Target Bot::_getTarget(const Blocks::TetrominoType& p_piece) {
    // A brute force method:
    // 1 - list all possibilities of valid piece positions on the playfield
    // 2 - Keep the one that has the best horizon

    /* Horizon is the line formed on top of the stack
    The shorter the better
    The lower the better
    */

    // The returned playfield is supposed to have no active piece when retrieved at this moment
    std::vector<std::vector<std::pair<int, float>>> playfield = _game->getPlayfield();
    std::vector<Blocks::Block> nextBlocks = _game->getNextAtRotation(0);
    // Get a current verison of the horizon
    Horizon prevHorizon;
    Target target;

    for (size_t rowIdx = 0; rowIdx < playfield.size(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < playfield.at(rowIdx).size(); colIdx++) {
            if (_game->isPiecePositionValid(nextBlocks, colIdx, rowIdx)) {
                // Make a copy of the playfield
                auto testPlayfield = playfield;
                // Add tested piece to the playfield
                for (size_t i = 0; i < nextBlocks.size(); i++) {
                    //SDL_Log("%zu, %d, %zu, %d, %zu", i, _activePiece->getX(), (i % 4), _activePiece->getY(), (i / 4));
                    int pieceColor = nextBlocks.at(i).colorIndex;
                    if (pieceColor < 0) // Ignore empty cells
                        continue;
                    int blockX = colIdx + (i % 4);
                    int blockY = rowIdx + (i / 4);
                    if ((blockY >= 0) && (blockY < testPlayfield.size())) {
                        if ((blockX >= 0) && (blockX < testPlayfield.at(blockY).size())) {
                            testPlayfield.at(blockY).at(blockX) = std::make_pair(pieceColor, 0.0f);
                        }
                    }
                }
                // Get horizon
                Horizon newHorizon = _getHorizon(testPlayfield);
                // Compare with previous
                if (newHorizon < prevHorizon) {
                    // Save for debug
                    _debugHorizon = newHorizon;

                    // If smaller, save piece position in target
                    target.x = colIdx;
                    target.y = rowIdx;
                }
            }
        }
    }

    return target;
}

Bot::Horizon Bot::_getHorizon(const std::vector<std::vector<std::pair<int, float>>>& p_playfield) {
    Horizon newHorizon;

    // Crawl through each block (left to right) to determine the horizon
    // Find the first free cell starting from the bottom left
    size_t i = 0;
    size_t j = p_playfield.size() - 1;
    while ((p_playfield.at(j).at(0).first > -1) && (j > 0)) {
        j--;
    }

    auto checkNextCells = [p_playfield](const size_t& p_colIdx, const size_t& p_rowIdx){
        //SDL_Log("checkNextCells %zu, %zu", p_colIdx, p_rowIdx);
        // Check right
        if (p_colIdx + 1 > p_playfield.at(p_rowIdx).size() - 1) {
            return HorizonCrawlerDirection::EndPlayfield;
        } else if (p_playfield.at(p_rowIdx).at(p_colIdx + 1).first < 0) {
            return HorizonCrawlerDirection::Right;
        }
        // Check down
        if ((p_rowIdx + 1 <= p_playfield.size() - 1) && (p_playfield.at(p_rowIdx + 1).at(p_colIdx).first < 0)) {
            return HorizonCrawlerDirection::Down;
        }
        // Check up
        if (p_rowIdx == 0) {
            return HorizonCrawlerDirection::EndPlayfield;
        } else if (p_playfield.at(p_rowIdx - 1).at(p_colIdx).first < 0) {
            return HorizonCrawlerDirection::Up;
        }
        // Check left
        if (p_colIdx == 0) {
            return HorizonCrawlerDirection::EndPlayfield;
        } else if (p_playfield.at(p_rowIdx).at(p_colIdx - 1).first < 0) {
            return HorizonCrawlerDirection::Left;
        }

        return HorizonCrawlerDirection::EndPlayfield;
    };

    HorizonCrawlerDirection nextDirection = checkNextCells(i, j);

    while (nextDirection != HorizonCrawlerDirection::EndPlayfield) {
        // Infinite loop check
        if ((newHorizon.data.size() > 1) && (nextDirection == newHorizon.data.at(newHorizon.data.size() - 2))) {
            break;// ToDo: Deal with this (later)
        }
        newHorizon.data.push_back(nextDirection);
        switch (nextDirection) {
            case Up:
                j -= 1;
                break;
            case Down:
                j += 1;
                break;
            case Left:
                i -= 1;
                break;
            case Right:
                i += 1;
                break;
            case EndPlayfield:
                break;
        }
        nextDirection = checkNextCells(i, j);
        //SDL_Log("nextDirection: %d", nextDirection);
    }

    return newHorizon;
}

/** Returns the list of all inputs until the piece locks
 * Returned data are the input value frame by frame
*/
std::vector<int> Bot::_generateInputs(const Target& p_target, const int& p_numFrames) {
    std::vector<int> inputs;

    // Aim for target x, y and rotation
    // consider a virtual active piece and evaluate distance to target for every input

    for (size_t i = 0; i < p_numFrames; i++) {
        if (i % 10 == 0)

            // Fill up with random input
            inputs.push_back(rand() % Controllable::BTN_C);
        else
            inputs.push_back(0);

    }

   return inputs;
}
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

// Generic visualization of the target on the playfield
std::vector<std::vector<int>> Bot::getDebugTarget() {
    // Initialize an empty vector to piece coordinates
    std::vector<int> row(_debugTarget.x + 3, -1);
    std::vector<std::vector<int>> targetCoordinates(_debugTarget.y + 4, row);

    std::vector<Blocks::Block> nextBlocks = _game->getBlocks(_debugTarget.type, _debugTarget.rotation);

    // Add the piece at coordinates
    for (size_t i = 0; i < nextBlocks.size(); i++) {
        if (nextBlocks.at(i).colorIndex >= 0) {
            int blockX = _debugTarget.x + (i % 4);
            int blockY = _debugTarget.y + (i / 4) - Blocks::BUFFER_ROWS;
            targetCoordinates[blockY][blockX] = nextBlocks.at(i).colorIndex;
        }
    }

    return targetCoordinates;
}

// Generic visualization of the horizon on the playfield
std::vector<std::vector<int>> Bot::getDebugHorizon() {
    // Create a grid of playfield size
    auto playfield = _game->getPlayfield();

    std::vector<int> horizonRow(playfield.at(0).size(), -1);
    std::vector<std::vector<int>> horizonData(playfield.size(), horizonRow);

    size_t rowIndex = _debugHorizon.startAltitude;
    size_t colIndex = 0;
    horizonData.at(rowIndex).at(colIndex) = 1;
    // Crawl through horizon data and fill up grid
    for (HorizonCrawlerDirection crawlingData : _debugHorizon.data) {
        switch (crawlingData) {
            case Up:
                rowIndex--;
                break;
            case Down:
                rowIndex++;
                break;
            case Left:
                colIndex--;
                break;
            case Right:
                colIndex++;
                break;
            case EndPlayfield:
                return horizonData;
        }

        horizonData.at(rowIndex).at(colIndex) = 1;
    }

    return {};
}

void Bot::_updateBot(const double&) {
    switch (_game->getState()) {
        case Blocks::SPAWN_TETROMINO:
            if (_game->getFrameTick() == 1) {
                Target target = _getTarget(_game->getNext());
                // Save for debug
                _debugTarget = target;
                // Get how many frames until lock
                size_t numFrames = _game->getPlayfield().size() * Blocks::GRAVITY_1_G / _game->getGravity();
                _inputs = _generateInputs(_game->getPlayfield(), target, numFrames);
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

    std::vector<std::vector<std::pair<int, float>>> playfield = _game->getPlayfield();

    // Get a current verison of the horizon
    Horizon prevHorizon;
    Target target;
    target.type = p_piece;

    for (size_t rotation = 0; rotation < _game->getNumPossibleRotations(p_piece); rotation++) {
        std::vector<Blocks::Block> nextBlocksAtRotation = _game->getBlocks(p_piece, rotation);
        for (size_t rowIdx = 0; rowIdx < playfield.size(); rowIdx++) {
            for (size_t colIdx = 0; colIdx < playfield.at(rowIdx).size(); colIdx++) {
                // We shift the piece to be able to test it in all configurations
                int pieceX = colIdx - 2;
                int pieceY = rowIdx - 2;// Generic playfield has no buffer row, we need to test the piece 1 row higher
                if (_game->isPiecePositionValid(nextBlocksAtRotation, pieceX, pieceY)) {
                    // Make a copy of the playfield
                    auto testPlayfield = playfield;
                    // Add tested piece to the playfield
                    for (size_t i = 0; i < nextBlocksAtRotation.size(); i++) {
                        //SDL_Log("%zu, %d, %zu, %d, %zu", i, _activePiece->getX(), (i % 4), _activePiece->getY(), (i / 4));
                        int pieceColor = nextBlocksAtRotation.at(i).colorIndex;
                        if (pieceColor < 0) // Ignore empty cells
                            continue;
                        int blockX = pieceX + (i % 4);
                        int blockY = pieceY + (i / 4);
                        if ((blockY >= 0) && (blockY < static_cast<int>(testPlayfield.size()))) {
                            if ((blockX >= 0) && (blockX < static_cast<int>(testPlayfield.at(blockY).size()))) {
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
                        target.x = pieceX;
                        target.y = pieceY;
                        target.rotation = rotation;
                    }
                }
            }
        }
    }

    return target;
}

Bot::Horizon Bot::_getHorizon(const std::vector<std::vector<std::pair<int, float>>>& p_playfield) {
    Horizon newHorizon;

    // Crawl through each block (left to right) to determine the horizon
    size_t i = 0;
    size_t j = 0;
    while ((p_playfield.at(j).at(0).first < 0) && (j < p_playfield.size() - 1)) {
        j++;
    }

    newHorizon.startAltitude = j;

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
        if ((newHorizon.data.size() > 1) && (nextDirection != newHorizon.data.at(newHorizon.data.size() - 1)) && (nextDirection == newHorizon.data.at(newHorizon.data.size() - 2))) {
            newHorizon.data.push_back(EndPlayfield);
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
                newHorizon.data.push_back(EndPlayfield);
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
std::vector<int> Bot::_generateInputs(const std::vector<std::vector<std::pair<int, float>>>& /*p_playfield*/, const Target& p_target, const int& p_numFrames) {
    std::vector<int> inputs;

    // Aim for target x, y and rotation
    int currentX = _game->getSpawnX();
    int currentRotation = 0;

    for (int i = 0; i < p_numFrames; i++) {
        bool canLock = (currentX == p_target.x) && (currentRotation == p_target.rotation);
        if (i % 10 == 0) {
            // Rotate until reaching target
            if (currentRotation < p_target.rotation) {
                currentRotation++;
                inputs.push_back(static_cast<int>(Controllable::BTN_B));
            } else if (currentRotation > p_target.rotation) {
                currentRotation--;
                inputs.push_back(static_cast<int>(Controllable::BTN_A));
            }

            // Move until reaching target
            if (currentX < p_target.x) {
                currentX++;
                inputs.push_back(static_cast<int>(Controllable::RIGHT));
            } else if (currentX > p_target.x) {
                currentX--;
                inputs.push_back(static_cast<int>(Controllable::LEFT));
            }
        } else {
            // Fill up with lock input
            if (canLock)
                inputs.push_back(static_cast<int>(Controllable::DOWN));
            else
                inputs.push_back(0);
        }
    }

   return inputs;
}
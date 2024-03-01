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
            int blockY = _debugTarget.y + (i / 4);
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

    for (HorizonCrawler crawlingData : _debugHorizon.data) {
        horizonData.at(crawlingData.getY()).at(crawlingData.getX()) = 1;
    }

    return horizonData;
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
    // Refined brute force method:
    // 1 - Get the horizon
    // 2 - list all possibilities of valid piece positions on the horizon
    // 3 - Keep the one that has the best horizon

    /* Horizon is the line formed on top of the stack
    The shorter the better
    The lower the better
    */

    std::vector<std::vector<std::pair<int, float>>> playfield = _game->getPlayfield();

    // Get a current verison of the horizon
    _debugHorizon = _getHorizon(playfield, true);
    // Make a copy of it
    std::vector<HorizonCrawler> horizonDataCopy;
    horizonDataCopy.assign(_debugHorizon.data.begin(), _debugHorizon.data.end());
    Target target;
    target.type = p_piece;

    SDL_Log("<Bot::_getTarget>");

    for (size_t rotation = 0; rotation < _game->getNumPossibleRotations(p_piece); rotation++) {
        std::vector<Blocks::Block> nextBlocksAtRotation = _game->getBlocks(p_piece, rotation);

        for (HorizonCrawler horizonData : horizonDataCopy) {
            // Get the 4 possible positions of the piece with this rotation for this coordinates in the horizon
            for (size_t i = 0; i < nextBlocksAtRotation.size(); i++) {
                if (nextBlocksAtRotation.at(i).colorIndex >= 0) {
                    int pieceX = horizonData.getX() - (i % 4);
                    int pieceY = horizonData.getY() - (i / 4);

                    if (_game->isPiecePositionValid(nextBlocksAtRotation, pieceX, pieceY)) {
                        // Make a copy of the playfield
                        // ToDo: try to get rid of this copy...
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
                        Horizon newHorizon = _getHorizon(testPlayfield, false);

                        SDL_Log("newHorizon with %s @ %d:%d %zu altitude: %f, length: %zu", _game->getPieceTypeStr(p_piece).c_str(), pieceX, pieceY, rotation, newHorizon.getAltitude(), newHorizon.data.size());

                        // Compare with previous
                        if (newHorizon < _debugHorizon) {
                            SDL_Log("Saving better horizon than previous altitude: %f, length: %zu", _debugHorizon.getAltitude(), _debugHorizon.data.size());
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
    }

    SDL_Log("target: %d;%d %d", target.x, target.y, target.rotation);

    return target;
}

Bot::Horizon Bot::_getHorizon(const std::vector<std::vector<std::pair<int, float>>>& p_playfield, bool p_init) {
    Horizon newHorizon;
    newHorizon.isInit = p_init;

    // Crawl through each block (left to right) to determine the horizon
    int startAltitude = p_playfield.size() - 1;
    size_t i = 0;
    size_t j = 0;
    while (j < startAltitude) {
        if (p_playfield.at(j + 1).at(0).first >= 0) {
            break;
        }
        j++;
    }

    auto isCellInPlayfield = [p_width = p_playfield.at(0).size(), p_height = p_playfield.size()](HorizonCrawler& p_crawler){
        return (p_crawler.getX() >= 0) && (p_crawler.getX() < p_width) && (p_crawler.getY() >= 0) && (p_crawler.getY() < p_height);
    };

    auto hasReachedEndPlayfield = [p_width = p_playfield.at(0).size()](HorizonCrawler& p_crawler){
        return (p_crawler.getX() < 0) || (p_crawler.getX() >= p_width) || (p_crawler.getY() < 0);
    };

    auto checkNextCells = [p_playfield, isCellInPlayfield, hasReachedEndPlayfield](HorizonCrawler p_crawler){
        //SDL_Log("checkNextCells %zu, %zu", p_colIdx, p_rowIdx);

        // Check right
        HorizonCrawler right = p_crawler.getRight();
        if (hasReachedEndPlayfield(right)) {
            return HorizonCrawlerDirection::EndPlayfield;
        } else if (isCellInPlayfield(right) && (p_playfield.at(right.getY()).at(right.getX()).first < 0)) {
            return right.getDirection();
        }
        // Check forward
        HorizonCrawler forward = p_crawler.getForward();
        if (hasReachedEndPlayfield(forward)) {
            return HorizonCrawlerDirection::EndPlayfield;
        } else if (isCellInPlayfield(forward) && (p_playfield.at(forward.getY()).at(forward.getX()).first < 0)) {
            return forward.getDirection();
        }
        // Check left
        HorizonCrawler left = p_crawler.getLeft();
        if (hasReachedEndPlayfield(left)) {
            return HorizonCrawlerDirection::EndPlayfield;
        } else if (isCellInPlayfield(left) && (p_playfield.at(left.getY()).at(left.getX()).first < 0)) {
            return left.getDirection();
        }
        // Check back
        HorizonCrawler back = p_crawler.getBack();
        if (hasReachedEndPlayfield(back)) {
            return HorizonCrawlerDirection::EndPlayfield;
        } else if (isCellInPlayfield(back) && (p_playfield.at(back.getY()).at(back.getX()).first < 0)) {
            return back.getDirection();
        }
        // Not supposed to happen
        return HorizonCrawlerDirection::EndPlayfield;
    };

    HorizonCrawler crawler = HorizonCrawler(i, j, HorizonCrawlerDirection::Right);
    HorizonCrawlerDirection nextDirection = checkNextCells(crawler);
    //SDL_Log("Init nextDirection: %d", nextDirection);

    while (nextDirection != HorizonCrawlerDirection::EndPlayfield) {
        // Infinite loop check
        if (!newHorizon.data.empty()) {
            // Simply check if this crawler already exists (it means we already went through this cell)
            for (HorizonCrawler crawler : newHorizon.data) {
                if ((crawler.getX() == i) &&
                    (crawler.getY() == j) &&
                    (crawler.getDirection() == nextDirection)) {
                        newHorizon.data.push_back(EndPlayfield);
                        break;
                    }
            }
        }

        newHorizon.data.push_back(HorizonCrawler(i, j, nextDirection));
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
        nextDirection = checkNextCells(HorizonCrawler(i, j, nextDirection));
        //SDL_Log("nextDirection: %d", nextDirection);
    }

    newHorizon.data.push_back(HorizonCrawler(i, j, nextDirection));
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
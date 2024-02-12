#include "blocks.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include "engine/controllable.hpp"
#include "engine/updatable.hpp"

#include <SDL2/SDL_log.h>

void Blocks::Tetromino::applyGravity(const int& p_gravity) {
    _gravityTick += p_gravity;

    // We move down by how much Gs in gravity
    while (_gravityTick >= Blocks::GRAVITY_1_G) {
        _gravityTick -= Blocks::GRAVITY_1_G;
        _y+=1;
    }
}

Blocks::Blocks(const size_t& p_version, const int& p_numColumns, const int& p_numRows) :
GameObject(nullptr),
_version(p_version),
_state(GameState::SPAWN_TETROMINO),
_level(0),
_blocksDb({}),
_playfield({}),
_nextTetromino(TetrominoType::I),
_activePiece(nullptr),
_clearedLines(0),
_frameTick(0),
_irsRotation(0),
_dasCounter(0),
_lockCounter(0),
_history({}),
_updater(nullptr),
_input(nullptr)
{
    assert(_version > 0 && _version < 3);// Version should be 1 (TGM) or 2 (TAP)

    _blocksDb = _buildBlocksDb();

    for (int i = 0; i < BUFFER_ROWS + p_numRows; i++) {
        std::vector<Block> row = {};
        for (int j = 0; j < p_numColumns; j++) {
            row.emplace_back(Block(-1));
        }
        _playfield.emplace_back(row);
    }

    _updater = new Updatable(this, [this](const double& p_frameDeltaTime){ _updateGame(p_frameDeltaTime); });
    _input = new Controllable(this);

    _nextTetromino = _pickNextTetromino(true);
}

Blocks::~Blocks() {
    if (_activePiece)
        delete _activePiece;
}

// Returns a generic view of the playfield
std::vector<std::vector<std::pair<int, float>>> Blocks::getPlayfield() {
    std::vector<std::vector<std::pair<int, float>>> playfieldView;

    // Start after BUFFER_ROWS
    for (size_t rowIdx = BUFFER_ROWS; rowIdx < _playfield.size(); rowIdx++) {
        std::vector<std::pair<int, float>> rowView;
        for (auto column : _playfield.at(rowIdx)) {
            rowView.push_back(std::make_pair(column.colorIndex, 1.0f));
        }
        playfieldView.push_back(rowView);
    }

    // Add active piece to the playfield if it's there
    const float lockRatio = static_cast<float>(_lockCounter) / static_cast<float>(_getLockDelay(_level));
    if (_activePiece) {
        std::vector<Block> activePieceBlocks = getBlocks(_activePiece->getType(), _activePiece->getRotation());
        for (size_t i = 0; i < activePieceBlocks.size(); i++) {
            //SDL_Log("%zu, %d, %zu, %d, %zu", i, _activePiece->getX(), (i % 4), _activePiece->getY(), (i / 4));
            int pieceColor = activePieceBlocks.at(i).colorIndex;
            if (pieceColor < 0) // Ignore empty cells
                continue;
            int blockX = _activePiece->getX() + (i % 4);
            int blockY = _activePiece->getY() + (i / 4) - BUFFER_ROWS;
            if ((blockY >= 0) && (blockY < static_cast<int>(_playfield.size()))) {
                if ((blockX >= 0) && (blockX < static_cast<int>(_playfield.at(blockY).size()))) {
                    playfieldView.at(blockY).at(blockX) = std::make_pair(pieceColor, lockRatio);
                }
            }
        }
    }

    return playfieldView;
}

// Returns a generic representation of the next piece
std::string Blocks::getNextStr() {
    switch (_nextTetromino) {
        case I:
            return "I";
        case O:
            return "O";
        case S:
            return "S";
        case Z:
            return "Z";
        case J:
            return "J";
        case L:
            return "L";
        case T:
            return "T";
    }

    return "";
}

bool Blocks::isPiecePositionValid(std::vector<Block> p_pieceBlocks, const int& p_x, const int& p_y) {
    for (size_t i = 0; i < p_pieceBlocks.size(); i++) {
        int pieceColor = p_pieceBlocks.at(i).colorIndex;
        //SDL_Log("%zu, %d;%d, %zu;%zu = %d", i, _activePiece->getX(), _activePiece->getY(), p_x + (i % 4), p_y + (i / 4), pieceColor);
        if (pieceColor < 0) // Ignore empty cells
            continue;
        int blockX = p_x + (i % 4);
        int blockY = p_y + (i / 4);
        if ((blockY < 0)
            || (blockY >= static_cast<int>(_playfield.size()))
            || (blockX < 0)
            || (blockX >= static_cast<int>(_playfield.at(blockY).size()))) {
                //SDL_Log("> OOB!");
                return false;
        }
        if (_playfield.at(blockY).at(blockX).colorIndex >= 0) {
            //SDL_Log("> block collision!");
            return false;
        }
    }

    return true;
}

void Blocks::_updateGame(const double&) {
    _frameTick++;

    int inpState, gravity, piecePositionX, piecePositionY, pieceRotation, pieceTotalGravityAvailable;

    switch (_state) {
        case SPAWN_TETROMINO:
            // We pick a piece and place it on the playfield
            // Wait for ARE
            if (_frameTick < _getAre(_level, _clearedLines.empty())) {
                inpState = _input->getInputState();
                // DAS buffer
                if (inpState & Controllable::LEFT) {
                    _dasCounter--;
                } else if (inpState & Controllable::RIGHT) {
                    _dasCounter--;
                } else {
                    _dasCounter = _getDas(_level);
                }
                // IRS
                if (inpState & Controllable::BTN_A) {
                    _irsRotation = -1;
                    _btnIsReleased[0] = false;
                } else if (inpState & Controllable::BTN_B) {
                    _irsRotation = 1;
                    _btnIsReleased[1] = false;
                } else if (inpState & Controllable::BTN_C) {
                    _irsRotation = -1;
                    _btnIsReleased[2] = false;
                } else {
                    _irsRotation = 0;
                    _btnIsReleased[0] = true;
                    _btnIsReleased[1] = true;
                    _btnIsReleased[2] = true;
                }
            } else {
                // Spawn piece at correct location on the playfield (create the Tetromino* object)
                _activePiece = new Tetromino(_nextTetromino, getSpawnX(), 0, _irsRotation);
                // GAME OVER if active piece spawns at invalid position
                if (!isPiecePositionValid(getBlocks(_activePiece->getType(), _activePiece->getRotation()), _activePiece->getX(), _activePiece->getY())) {
                    _lockPiece();
                    _activePiece = nullptr;
                    _state = GAME_OVER;
                    return;
                }
                _state = GameState::MOVE_TETROMINO;
                _frameTick = 0;
                _lockCounter = _getLockDelay(_level);
                // Select the next piece
                _nextTetromino = _pickNextTetromino(false);
            }
            break;
        case MOVE_TETROMINO:
            // Block control flow: Rotation > Horizontal move > vertical drop
// ToDo: implement synchros
            inpState = _input->getInputState();
            piecePositionX = _activePiece->getX();
            piecePositionY = _activePiece->getY();
            pieceRotation = _activePiece->getRotation();

            // Rotation
            if (inpState & Controllable::BTN_A) {
                if (_btnIsReleased[0]) {
                    _btnIsReleased[0] = false;
                    int kickedOffset = _kickPiece(piecePositionX, piecePositionY, pieceRotation - 1);
                    if (kickedOffset != Blocks::INVALID_KICK_OFFSET) {
                        piecePositionX += kickedOffset;
                        pieceRotation -= 1;
                    }
                }
            } else if (inpState | Controllable::BTN_A) {
                _btnIsReleased[0] = true;
            }
            if (inpState & Controllable::BTN_B) {
                if (_btnIsReleased[1]) {
                    _btnIsReleased[1] = false;
                    int kickedOffset = _kickPiece(piecePositionX, piecePositionY, pieceRotation + 1);
                    if (kickedOffset != Blocks::INVALID_KICK_OFFSET) {
                        piecePositionX += kickedOffset;
                        pieceRotation += 1;
                    }
                }
            } else if (inpState | Controllable::BTN_B) {
                _btnIsReleased[1] = true;
            }
            if (inpState & Controllable::BTN_C) {
                if (_btnIsReleased[2]) {
                    _btnIsReleased[2] = false;
                    int kickedOffset = _kickPiece(piecePositionX, piecePositionY, pieceRotation - 1);
                    if (kickedOffset != Blocks::INVALID_KICK_OFFSET) {
                        piecePositionX += kickedOffset;
                        pieceRotation -= 1;
                    }
                }
            } else if (inpState | Controllable::BTN_C) {
                _btnIsReleased[2] = true;
            }
            // Horizontal move
            if (inpState & Controllable::LEFT) {
                if ((_dasCounter == _getDas(_level)) || (_dasCounter <= 0)) {
                    if (isPiecePositionValid(getBlocks(_activePiece->getType(), pieceRotation), piecePositionX - 1, piecePositionY))
                        piecePositionX -= 1;
                }
                _dasCounter--;
            } else if (inpState & Controllable::RIGHT) {
                if ((_dasCounter == _getDas(_level)) || (_dasCounter <= 0)) {
                    if (isPiecePositionValid(getBlocks(_activePiece->getType(), pieceRotation), piecePositionX + 1, piecePositionY))
                        piecePositionX += 1;
                }
                _dasCounter--;
            } else {
                _dasCounter = _getDas(_level);
            }
            // Vertical drop
            // For some reason GRAVITY_1_G (or Blocks::GRAVITY_1_G) doesn't compile here
            gravity = _getGravity(_level);
            if (inpState & Controllable::DOWN) {
                gravity = std::max(gravity, 256);
                _lockCounter = 0;// Instant lock in case of collision
            } else if ((_version > 1) && (inpState & Controllable::UP)) {
                gravity = 256 * _playfield.size();
            }

            // The piece will chute by all this amount of gravity
            pieceTotalGravityAvailable = gravity;

            // We test where the piece will be if it's affected by gravity
            for (size_t i = 0; i <= _getRowsByGravity(gravity); i++) {
                if (!isPiecePositionValid(getBlocks(_activePiece->getType(), pieceRotation), piecePositionX, piecePositionY + i)) {
                    pieceTotalGravityAvailable = (i - 1) * 256;
                    break;
                }
            }

            // Move the piece by the available distance
            _activePiece->setX(piecePositionX);
            _activePiece->setRotation(pieceRotation);
            _activePiece->applyGravity(pieceTotalGravityAvailable);

            // The piece was interrupted in its chute
            if (pieceTotalGravityAvailable < gravity) {
                if (_lockCounter > 0) {
                    _lockCounter--;
                } else {
                    _lockPiece();
                    if ((_level == 0) || (_level % 100) != 0) {
                        _level++;
                    }
                    _activePiece = nullptr;
                    _clearedLines = _clearLines();
                    _level += _clearedLines.size();
                    if (!_clearedLines.empty()) {
                        _state = GameState::CLEAR_LINE;
                    } else {
                        _state = GameState::SPAWN_TETROMINO;
                    }
                    _frameTick = 0;
                }
            } else {
                _lockCounter = _getLockDelay(_level);
            }
            break;
        case CLEAR_LINE:
            // We wait for line clear delay
            if (_frameTick >= _getLineClear(_level)) {
                _reorganizePlayfieldAfterLineClear();
                _state = GameState::SPAWN_TETROMINO;
                _frameTick = 0;
            }
            break;
        case GAME_OVER:
            // Game freezes and send an end signal to the void
            _frameTick = 0;
            break;
    }
}

Blocks::TetrominoType Blocks::_pickNextTetromino(bool p_firstDraw) {
    // Follow https://tetris.wiki/TGM_randomizer

    // Init
    // Initiate history with 4 Z
    if (p_firstDraw) {
        _history.clear();
        _history.push_back(TetrominoType::Z);
        _history.push_back(TetrominoType::Z);
        _history.push_back(TetrominoType::Z);
        _history.push_back(TetrominoType::Z);
    }
    // Don't pick S, Z or O as first piece
    int sampleSize = (p_firstDraw) ? 4 : 7;

    // Keep a history of the last 4 pieces
    // Pick randomly among the 7 pieces
    // If next is in history, try again (4 times)
    TetrominoType next = _history.back();
    size_t retry = 0;
    size_t numRetries = 4;// TGM1 rule

    while (std::find(_history.begin(), _history.end(), next) != _history.end() && retry <= numRetries) {
        next = static_cast<TetrominoType>(rand() % sampleSize);
        retry++;
    }

    // Update history
    _history.erase(_history.begin());
    _history.push_back(next);

    return next;
}

int Blocks::_kickPiece(const int& p_x, const int& p_y, const int& p_rotation) {
    std::vector<int> testPositions = {0, 1, -1};
    // Try positions
    for (int testPosition : testPositions) {
        if (isPiecePositionValid(getBlocks(_activePiece->getType(), p_rotation), p_x + testPosition, p_y)) {
            return testPosition;
        }
    }
    return Blocks::INVALID_KICK_OFFSET;
}

void Blocks::_lockPiece() {
    std::vector<Block> activePieceBlocks = getBlocks(_activePiece->getType(), _activePiece->getRotation());

    for (size_t i = 0; i < activePieceBlocks.size(); i++) {
        int pieceColor = activePieceBlocks.at(i).colorIndex;
        //SDL_Log("%zu, %d, %zu, %d, %zu = %d", i, _activePiece->getX(), (i % 4), _activePiece->getY(), (i / 4), pieceColor);
        if (pieceColor < 0) // Ignore empty cells
            continue;
        int blockX = _activePiece->getX() + (i % 4);
        int blockY = _activePiece->getY() + (i / 4);
        _playfield.at(blockY).at(blockX) = pieceColor;
    }
}

std::vector<size_t> Blocks::_clearLines() {
    std::vector<size_t> clearedLines;
    for (size_t rowIdx = 0; rowIdx < _playfield.size(); rowIdx++) {
        bool rowIsFull = true;
        for (size_t colIdx = 0; colIdx < _playfield.at(rowIdx).size(); colIdx++) {
            if (_playfield.at(rowIdx).at(colIdx).colorIndex < 0)
                rowIsFull = false;
        }

        if (rowIsFull) {
            // We save how many lines we cleared for later
            clearedLines.push_back(rowIdx);
            // We empty the line
            for (size_t colIdx = 0; colIdx < _playfield.at(rowIdx).size(); colIdx++) {
                _playfield.at(rowIdx).at(colIdx).colorIndex = -1;
            }
        }
    }

    return clearedLines;
}

void Blocks::_reorganizePlayfieldAfterLineClear() {
    while (!_clearedLines.empty()) {
        size_t clearedLineIdx = _clearedLines.at(0);

        // From bottom to top
        for (size_t rowIdx = clearedLineIdx; rowIdx > 0; rowIdx--) {
            // We move the line over us here
            for (size_t colIdx = 0; colIdx < _playfield.at(rowIdx).size(); colIdx++) {
                _playfield.at(rowIdx).at(colIdx).colorIndex = _playfield.at(rowIdx - 1).at(colIdx).colorIndex;
                _playfield.at(rowIdx - 1).at(colIdx).colorIndex = -1;
            }
        }

        _clearedLines.erase(_clearedLines.begin());
    }
}
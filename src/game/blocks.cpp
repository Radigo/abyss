#include "blocks.hpp"

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

void Blocks::Tetromino::move(const int& p_direction) {
    _x += p_direction;
}

void Blocks::Tetromino::rotate(const int& p_direction) {
    _rotation += p_direction;
}

Blocks::Blocks(const int& p_numColumns, const int& p_numRows) :
_state(GameState::SPAWN_TETROMINO),
_playfield({}),
_level(0),
_frameTick(0),
_dasCounter(0),
_lockCounter(0)
{
    for (int i = 0; i < BUFFER_ROWS + p_numRows; i++) {
        std::vector<Block> row = {};
        for (int j = 0; j < p_numColumns; j++) {
            row.emplace_back(Block(-1));
        }
        _playfield.emplace_back(row);
    }

    _updater = new Updatable(this, [this](const double& p_frameDeltaTime){ _updateGame(p_frameDeltaTime); });
    _input = new Controllable(this);

    _pickNextTetromino();
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
        std::vector<Block> activePieceBlocks = _getBlocks(_activePiece->getType(), _activePiece->getRotation());
        for (size_t i = 0; i < activePieceBlocks.size(); i++) {
            //SDL_Log("%zu, %d, %zu, %d, %zu", i, _activePiece->getX(), (i % 4), _activePiece->getY(), (i / 4));
            int pieceColor = activePieceBlocks.at(i).colorIndex;
            if (pieceColor < 0) // Ignore empty cells
                continue;
            int blockX = _activePiece->getX() + (i % 4);
            int blockY = _activePiece->getY() + (i / 4) - BUFFER_ROWS;
            if ((blockY >= 0) && (blockY < _playfield.size())) {
                if ((blockX >= 0) && (blockX < _playfield.at(blockY).size())) {
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

void Blocks::_updateGame(const double&) {
    _frameTick++;

    int moveState, btnState, gravity, pieceTotalGravityAvailable;

    switch (_state) {
        case SPAWN_TETROMINO:
            // We pick a piece and place it on the playfield
            // Wait for ARE
            if (_frameTick < _getAre(_level)) {
                // DAS buffer
                if (moveState & Controllable::LEFT) {
                    _dasCounter--;
                } else if (moveState & Controllable::RIGHT) {
                    _dasCounter--;
                } else {
                    _dasCounter = _getDas(_level);
                }
                // IRS
                if (btnState & Controllable::BTN_A) {
                    _irsRotation = -1;
                    _btnIsReleased[0] = false;
                } else if (btnState & Controllable::BTN_B) {
                    _irsRotation = 1;
                    _btnIsReleased[1] = false;
                } else if (btnState & Controllable::BTN_C) {
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
                _activePiece = new Tetromino(_nextTetromino, _getSpawnX(), 0, _irsRotation);
                // GAME OVER if active piece spawns at invalid position
                if (!_isActivePiecePositionValid(_activePiece->getX(), _activePiece->getY(), _activePiece->getRotation())) {
                    _lockPiece();
                    _activePiece = nullptr;
                    _state = GAME_OVER;
                    return;
                }
                _state = GameState::MOVE_TETROMINO;
                _frameTick = 0;
                _lockCounter = _getLockDelay(_level);
                // Select the next piece
                _pickNextTetromino();
            }
            break;
        case MOVE_TETROMINO:
            // The gravity applies, we control the piece (includes lock delay)
            moveState = _input->getMoveState();
            btnState = _input->getButtonState();

            if (moveState & Controllable::LEFT) {
                if ((_dasCounter == _getDas(_level)) || (_dasCounter <= 0)) {
                    if (_isActivePiecePositionValid(_activePiece->getX() - 1, _activePiece->getY(), _activePiece->getRotation()))
                        _activePiece->move(-1);
                }
                _dasCounter--;
            } else if (moveState & Controllable::RIGHT) {
                if ((_dasCounter == _getDas(_level)) || (_dasCounter <= 0)) {
                    if (_isActivePiecePositionValid(_activePiece->getX() + 1, _activePiece->getY(), _activePiece->getRotation()))
                        _activePiece->move(1);
                }
                _dasCounter--;
            } else {
                _dasCounter = _getDas(_level);
            }

            if (btnState & Controllable::BTN_A) {
                if (_btnIsReleased[0] && _isActivePiecePositionValid(_activePiece->getX(), _activePiece->getY(), _activePiece->getRotation() - 1)) {
                    _btnIsReleased[0] = false;
                    _activePiece->rotate(-1);
                }
            } else if (btnState | Controllable::BTN_A) {
                _btnIsReleased[0] = true;
            }
            if (btnState & Controllable::BTN_B) {
                if (_btnIsReleased[1] && _isActivePiecePositionValid(_activePiece->getX(), _activePiece->getY(), _activePiece->getRotation() + 1)) {
                    _btnIsReleased[1] = false;
                    _activePiece->rotate(1);
                }
            } else if (btnState | Controllable::BTN_B) {
                _btnIsReleased[1] = true;
            }
            if (btnState & Controllable::BTN_C) {
                if (_btnIsReleased[2] && _isActivePiecePositionValid(_activePiece->getX(), _activePiece->getY(), _activePiece->getRotation() - 1)) {
                    _btnIsReleased[2] = false;
                    _activePiece->rotate(-1);
                }
            } else if (btnState | Controllable::BTN_C) {
                _btnIsReleased[2] = true;
            }

            // Wall kicks

            // floor
            // For some reason GRAVITY_1_G (or Blocks::GRAVITY_1_G) doesn't compile here
            gravity = _getGravity(_level);
            if (moveState & Controllable::DOWN) {
                gravity = std::max(gravity, 256);
                _lockCounter = 0;// Instant lock in case of collision
            }

            // The piece will chute by all this amount of gravity
            pieceTotalGravityAvailable = gravity;

            // We test where the piece will be if it's affected by gravity
            for (int i = 0; i <= _getRowsByGravity(gravity); i++) {
                if (!_isActivePiecePositionValid(_activePiece->getX(), _activePiece->getY() + i, _activePiece->getRotation())) {
                    pieceTotalGravityAvailable = (i - 1) * 256;
                    break;
                }
            }

            // Move the piece by the available distance
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

void Blocks::_pickNextTetromino() {
    //ToDo: pick next using TGM algorithm
    _nextTetromino = static_cast<TetrominoType>(rand() % 7);
}

int Blocks::_getSpawnX() {
    return static_cast<int>(static_cast<float>(_playfield.at(0).size()) * 0.5f) - 2;
}

bool Blocks::_isActivePiecePositionValid(const int& p_x, const int& p_y, const int& p_rotation) {
    std::vector<Block> activePieceBlocks = _getBlocks(_activePiece->getType(), p_rotation);

    for (size_t i = 0; i < activePieceBlocks.size(); i++) {
        int pieceColor = activePieceBlocks.at(i).colorIndex;
        SDL_Log("%zu, %d;%d, %zu;%zu = %d", i, _activePiece->getX(), _activePiece->getY(), p_x + (i % 4), p_y + (i / 4), pieceColor);
        if (pieceColor < 0) // Ignore empty cells
            continue;
        int blockX = p_x + (i % 4);
        int blockY = p_y + (i / 4);
        if ((blockY < 0)
            || (blockY >= _playfield.size())
            || (blockX < 0)
            || (blockX >= _playfield.at(blockY).size())) {
                SDL_Log("> OOB!");
                return false;
        }
        if (_playfield.at(blockY).at(blockX).colorIndex >= 0) {
            SDL_Log("> block collision!");
            return false;
        }
    }

    return true;
}

void Blocks::_lockPiece() {
    std::vector<Block> activePieceBlocks = _getBlocks(_activePiece->getType(), _activePiece->getRotation());

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
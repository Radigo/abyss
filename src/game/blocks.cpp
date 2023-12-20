#include "blocks.hpp"

#include <cmath>
#include <vector>

#include "engine/updatable.hpp"

void Blocks::Tetromino::applyGravity(const float& p_gravity) {
    _gravityStatus += p_gravity;
    if (_gravityStatus < Blocks::GRAVITY_1_G)
        return;
    
    // We move down by 1G
    _gravityStatus -= Blocks::GRAVITY_1_G;
    _y+=1;
    // Resolve collisions
    // Handle lock
}

void Blocks::Tetromino::move(const int& p_direction) {
    _x += p_direction;
    // Resolve collisions
}

void Blocks::Tetromino::rotate(const int& p_direction) {
    _rotation += p_direction;
    // Resolve collisions
}

Blocks::Blocks(const int& p_numColumns, const int& p_numRows) :
_state(GameState::SPAWN_TETROMINO),
_playfield({}),
_level(0)
{
    // ToDo: add a buffer row on top, make sure it's hidden when we want to draw the playfield
    for (int i = 0; i < p_numRows; i++) {
        std::vector<Block> row = {};
        for (int j = 0; j < p_numColumns; j++) {
            row.emplace_back(Block(-1));
        }
        _playfield.emplace_back(row);
    }

    _updater = new Updatable(this, [this](const double& p_frameDeltaTime){ _updateGame(p_frameDeltaTime); });

    _pickNextTetromino();
}

Blocks::~Blocks() {
    if (_activePiece)
        delete _activePiece;
}

void Blocks::_updateGame(const double&) {
    _frameTick++;

    switch (_state) {
        case SPAWN_TETROMINO:
            // We pick a piece and place it on the playfield (includes ARE)

            // Wait for ARE (prepare IRS)
            if (_frameTick >= _getAre(_level)) {
                // Spawn piece at correct location on the playfield (create the Tetromino* object)
                // ToDo: implement IRS
                _activePiece = new Tetromino(_nextTetromino, 0, 0, 0);
                _state = GameState::MOVE_TETROMINO;
                // Select the next piece
                _pickNextTetromino();
            }
            break;
        case MOVE_TETROMINO:
            // The gravity applies, we control the piece (includes lock delay)
            _activePiece->applyGravity(_getGravity(_level));
            break;
        case LOCK_TETROMINO:
            // The piece locks and we resolve lines (includes clear delay)
            delete _activePiece;
            break;
        case GAME_OVER:
            // Game freezes and send an end signal to the void
            break;
    }
}

void Blocks::_pickNextTetromino() {
    _nextTetromino = _getTetromino(static_cast<TetrominoType>(rand() % 7), 0);
}
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

// Returns a generic view of the playfield
std::vector<std::vector<int>> Blocks::getPlayfield() {
    std::vector<std::vector<int>> playfieldView;

    for (auto row : _playfield) {
        std::vector<int> rowView;
        for (auto column : row) {
            rowView.push_back(column.colorIndex);
        }
        playfieldView.push_back(rowView);
    }

    return playfieldView;
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
                _frameTick = 0;
                // Select the next piece
                _pickNextTetromino();
            }
            break;
        case MOVE_TETROMINO:
            // The gravity applies, we control the piece (includes lock delay)
            _activePiece->applyGravity(_getGravity(_level));
            if (_frameTick >= 60) {
                delete _activePiece;
                _state = GameState::CLEAR_LINE;
                _frameTick = 0;
            }
            break;
        case CLEAR_LINE:
            // The piece locks and we resolve lines (includes clear delay)
            if (_frameTick >= _getLineClear(_level)) {
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
    _nextTetromino = _getTetromino(static_cast<TetrominoType>(rand() % 7), 0);
}
#include "blocks.hpp"

#include <cmath>
#include <vector>

#include "engine/updatable.hpp"

Blocks::Blocks(const int& p_numColumns, const int& p_numRows) :
_playfield({})
{
    for (int i = 0; i < p_numRows; i++) {
        std::vector<Block> row = {};
        for (int j = 0; j < p_numColumns; j++) {
            row.emplace_back(Block(-1));
        }
        _playfield.emplace_back(row);
    }

    _updater = new Updatable(this, [this](const double& p_frameDeltaTime){ _updateGame(p_frameDeltaTime); });
}

Blocks::~Blocks() {
    
}

Immplement tetromino map from here: https://tetris.wiki/Arika_Rotation_System

std::vector<std::vector<Blocks::Block>> Blocks::getTetromino(const TetrominoType& p_type) {
    std::vector<int> colors;
    switch (p_type) {
        case Blocks::TetrominoType::I:
            colors = {
                -1, -1, -1, -1,
                 0,  0,  0,  0,
                -1, -1, -1, -1,
                -1, -1, -1, -1,
            };
        case O:
            colors = {
                -1, -1, -1, -1,
                -1,  1,  1, -1,
                -1,  1,  1, -1,
                -1, -1, -1, -1,
            };
            break;
        case S:
            break;
        case Z:
            break;
        case J:
            break;
        case L:
            break;
        case T:
            break;
    }

    const int TETROMINO_SIZE = 4;
    std::vector<std::vector<Blocks::Block>> tetromino;

    for (size_t i = 0; i < TETROMINO_SIZE; i++) {
        tetromino.emplace_back(std::vector<Blocks::Block>(TETROMINO_SIZE));
    }

    for (size_t i = 0; i < colors.size(); i++) {
        size_t column = i % TETROMINO_SIZE;
        size_t row = std::floor(i / TETROMINO_SIZE);
        tetromino.at(column).at(row) = Block(colors.at(i));
    }
}

void Blocks::_updateGame(const double&) {
    
}
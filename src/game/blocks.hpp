#pragma once

/*
Game of Tetris
A grid of X columns by Y rows
A set of tetrominos that spawn on the top
A gravity system that pull pieces down
Controls to move with fancy rules
Lines clear when rows are full
*/

#include <vector>

#include "engine/gameobject.hpp"

class Updatable;

class Blocks : public GameObject {
    public:
        Blocks(const int& p_numColumns, const int& p_numRows);
        ~Blocks();

    private:
        struct Block {
            Block(const int& p_color = -1) :
            colorIndex(p_color) {}
            int colorIndex = -1;
        };

        enum TetrominoType {
            I, O, S, Z, J, L, T
        };

        struct Tetromino {
            public:
            Tetromino(const std::vector<std::vector<Block>>& p_color, const int& p_x, const int& p_y) :
            _pieceBlocks(p_color),
            _x(p_x),
            _y(p_y) {}
            ~Tetromino();

            void applyGravity(const float& p_gravity);
            void move(const int& p_direction);
            void rotate(const int& p_direction);

            private:
                std::vector<std::vector<Block>> _pieceBlocks;
                // Increase each time gravity is applied.
                // The piece falls down by the amount of gravity as soon as the gravity exceeds 1
                float _gravityStatus;
                int _x;
                int _y;
        };

        std::vector<std::vector<Block>> _playfield;
        Tetromino* _activePiece;
        // Gravity is how many rows are applied at each update. Between 0 and 1 it takes n frames to go 1 row down.
        float _gravity;

        Updatable* _updater;

        std::vector<std::vector<Block>> getTetromino(const TetrominoType& p_type);
        void _updateGame(const double& p_frameDeltaTime);
};
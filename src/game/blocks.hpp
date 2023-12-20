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
        static const int GRAVITY_1_G = 256;

        enum GameState {
            SPAWN_TETROMINO,    // We pick a piece and place it on the playfield (includes ARE)
            MOVE_TETROMINO,     // The gravity applies, we control the piece (includes lock delay)
            LOCK_TETROMINO,     // The piece locks and we resolve lines (includes clear delay)
            GAME_OVER           // Game freezes and send an end signal to the void
        };

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
            Tetromino(const std::vector<Block>& p_color, const int& p_x, const int& p_y, const int& p_rotation) :
            _pieceBlocks(p_color),
            _x(p_x),
            _y(p_y),
            _rotation(p_rotation) {}
            ~Tetromino() {}

            void applyGravity(const float& p_gravity);
            void move(const int& p_direction);
            void rotate(const int& p_direction);

            private:
                std::vector<Block> _pieceBlocks;
                // Increase each time gravity is applied.
                // The piece falls down by the amount of gravity as soon as the gravity exceeds 1
                int _gravityStatus;
                int _x;
                int _y;
                int _rotation;
        };

        GameState _state;
        std::vector<std::vector<Block>> _playfield;
        int _level;
        int _frameTick;// Our main internal time counter for each state
        std::vector<Block> _nextTetromino;

        Tetromino* _activePiece;
        Updatable* _updater;

        void _updateGame(const double& p_frameDeltaTime);
        void _pickNextTetromino();

        inline int _getAre(const int&) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            return 30;
        }
        
        inline int _getDas(const int&) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            return 16;
        }
        
        inline int _getLockDelay(const int&) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            return 30;
        }
        
        inline int _getLineClear(const int&) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            return 41;
        }
        
        inline int _getGravity(const int& p_level) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            if (p_level >= 500) return GRAVITY_1_G * 20;// 20G
            else if (p_level >= 450) return GRAVITY_1_G * 3;// 3G
            else if (p_level >= 420) return GRAVITY_1_G * 4;// 4G
            else if (p_level >= 400) return GRAVITY_1_G * 5;// 5G
            else if (p_level >= 360) return GRAVITY_1_G * 4;// 4G
            else if (p_level >= 330) return GRAVITY_1_G * 3;// 3G
            else if (p_level >= 300) return GRAVITY_1_G * 2;// 2G
            else if (p_level >= 251) return GRAVITY_1_G;// 1G
            else if (p_level >= 247) return 224;
            else if (p_level >= 243) return 192;
            else if (p_level >= 239) return 160;
            else if (p_level >= 236) return 128;
            else if (p_level >= 233) return 96;
            else if (p_level >= 230) return 64;
            else if (p_level >= 220) return 32;
            else if (p_level >= 200) return 4;
            else if (p_level >= 170) return 144;
            else if (p_level >= 160) return 128;
            else if (p_level >= 140) return 112;
            else if (p_level >= 120) return 96;
            else if (p_level >= 100) return 80;
            else if (p_level >= 90) return 64;
            else if (p_level >= 80) return 48;
            else if (p_level >= 70) return 32;
            else if (p_level >= 60) return 16;
            else if (p_level >= 50) return 12;
            else if (p_level >= 40) return 10;
            else if (p_level >= 35) return 8;
            else if (p_level >= 30) return 6;
            else return 4;
        }

        inline std::vector<Block> _getTetromino(const TetrominoType& p_type, const int& p_rotation) {
            // Tetromino map from: https://tetris.wiki/Arika_Rotation_System
            std::vector<std::vector<int>> colors;
            switch (p_type) {
                case I:
                    colors = {{
                        -1, -1, -1, -1,
                        0,  0,  0,  0,
                        -1, -1, -1, -1,
                        -1, -1, -1, -1,
                    },{
                        -1, -1,  0, -1,
                        -1, -1,  0, -1,
                        -1, -1,  0, -1,
                        -1, -1,  0, -1,
                    }};
                    break;
                case T:
                    colors = {{
                        -1, -1, -1, -1,
                        1,  1,  1, -1,
                        -1,  1, -1, -1,
                        -1, -1, -1, -1,
                    },{
                        -1,  1, -1, -1,
                        1,  1, -1, -1,
                        -1,  1, -1, -1,
                        -1, -1, -1, -1,
                    },{
                        -1, -1, -1, -1,
                        -1,  1, -1, -1,
                        1,  1,  1, -1,
                        -1, -1, -1, -1,
                    },{
                        -1,  1, -1, -1,
                        -1,  1,  1, -1,
                        -1,  1, -1, -1,
                        -1, -1, -1, -1,
                    }};
                    break;
                case L:
                    colors = {{
                        -1, -1, -1, -1,
                        2,  2,  2, -1,
                        2, -1, -1, -1,
                        -1, -1, -1, -1,
                    },{
                        2,  2, -1, -1,
                        -1,  2, -1, -1,
                        -1,  2, -1, -1,
                        -1, -1, -1, -1,
                    },{
                        -1, -1, -1, -1,
                        -1, -1,  2, -1,
                        2,  2,  2, -1,
                        -1, -1, -1, -1,
                    },{
                        -1,  2, -1, -1,
                        -1,  2, -1, -1,
                        -1,  2,  2, -1,
                        -1, -1, -1, -1,
                    }};
                    break;
                case J:
                    colors = {{
                        -1, -1, -1, -1,
                        3,  3,  3, -1,
                        -1, -1,  3, -1,
                        -1, -1, -1, -1,
                    },{
                        -1,  3, -1, -1,
                        -1,  3, -1, -1,
                        3,  3, -1, -1,
                        -1, -1, -1, -1,
                    },{
                        -1, -1, -1, -1,
                        3, -1, -1, -1,
                        3,  3,  3, -1,
                        -1, -1, -1, -1,
                    },{
                        -1,  3,  3, -1,
                        -1,  3, -1, -1,
                        -1,  3, -1, -1,
                        -1, -1, -1, -1,
                    }};
                    break;
                case S:
                    colors = {{
                        -1, -1, -1, -1,
                        -1,  4,  4, -1,
                        4,  4, -1, -1,
                        -1, -1, -1, -1,
                    },{
                        4, -1, -1, -1,
                        4,  4, -1, -1,
                        -1,  4, -1, -1,
                        -1, -1, -1, -1,
                    }};
                    break;
                case Z:
                    colors = {{
                        -1, -1, -1, -1,
                        5,  5, -1, -1,
                        -1,  5,  5, -1,
                        -1, -1, -1, -1,
                    },{
                        -1, -1,  5, -1,
                        -1,  5,  5, -1,
                        -1,  5, -1, -1,
                        -1, -1, -1, -1,
                    }};
                    break;
                case O:
                    colors = {{
                        -1, -1, -1, -1,
                        -1,  6,  6, -1,
                        -1,  6,  6, -1,
                        -1, -1, -1, -1,
                    }};
                    break;
            }

            std::vector<Blocks::Block> tetromino;

            for (size_t i = 0; i < colors.size(); i++) {
                tetromino.push_back(Block(colors.at(i).at(p_rotation % colors.at(i).size())));
            }

            return tetromino;
        }
};
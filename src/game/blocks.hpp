#pragma once

/*
Game of Tetris
A grid of X columns by Y rows
A set of tetrominos that spawn on the top
A gravity system that pull pieces down
Controls to move with fancy rules
Lines clear when rows are full
*/

#include <cmath>
#include <vector>
#include <unordered_map>

#include "engine/gameobject.hpp"

class Updatable;
class Controllable;

class Blocks : public GameObject {
    public:
        static const int BUFFER_ROWS = 1;   // How many rows are hiden over the playfield
        static const int GRAVITY_1_G = 256; // 1G in gravity units

        enum GameState {
            SPAWN_TETROMINO,    // We pick a piece and place it on the playfield (includes ARE)
            MOVE_TETROMINO,     // The gravity applies, we control the piece (includes lock delay)
            CLEAR_LINE,         // We clear the line(s) and rearrange the playfield (includes clear delay)
            GAME_OVER           // Game freezes and send an end signal to the void
        };

        enum TetrominoType {
            I, T, L, J, S, Z, O
        };

        struct Block {
            Block(const int& p_color = -1) :
            colorIndex(p_color) {}
            ~Block() {}
            int colorIndex = -1;
        };

    private:
        static const int INVALID_KICK_OFFSET = 2;

        struct Tetromino {
            public:
            Tetromino(const TetrominoType& p_type, const int& p_x, const int& p_y, const int& p_rotation) :
            _type(p_type),
            _rotation(p_rotation),
            _x(p_x),
            _y(p_y),
            _gravityTick(0) {}
            ~Tetromino() {}

            inline TetrominoType getType() { return _type; }
            inline void setRotation(const int& p_rotation) { _rotation = p_rotation; }// Consider % this to limit value
            inline int getRotation() { return _rotation; }
            inline int getX() { return _x; }
            inline void setX(const int& p_x) { _x = p_x; }
            inline int getY() { return _y; }
            void applyGravity(const int& p_gravity);

            private:
                TetrominoType _type;
                int _rotation;
                int _x;
                int _y;
                int _gravityTick;
        };

        size_t _version;// 1 (TGM) or 2 (TAP)
        GameState _state;
        size_t _level;

        std::unordered_map<TetrominoType, std::vector<std::vector<int>>> _blocksDb;
        std::vector<std::vector<Block>> _playfield;
        TetrominoType _nextTetromino;
        Tetromino* _activePiece;
        std::vector<size_t> _clearedLines;

        size_t _frameTick;// Our main internal time counter for each state
        int _irsRotation;// Initial rotation value
        size_t _dasCounter;// Delayed Auto Shift (holding direction)
        size_t _lockCounter;// Lock delay
        bool _btnIsReleased[3];
        std::vector<TetrominoType> _history;

        Updatable* _updater;
        Controllable* _input;

    public:
        Blocks(const size_t& p_version = 1, const int& p_numColumns = 10, const int& p_numRows = 20);
        ~Blocks();

        std::vector<std::vector<std::pair<int, float>>> getPlayfield();
        bool isPiecePositionValid(std::vector<Block> p_pieceBlocks, const int& p_x, const int& p_y);

        inline void setLevel(const int p_level) { _level = p_level; }
        inline int getLevel() { return _level; }
        inline int getState() { return _state; }
        inline int getGravity() { return _getGravity(_level); }
        inline size_t getFrameTick() { return _frameTick; }
        inline Controllable* getInput() { return _input; }
        inline TetrominoType getNext() { return _nextTetromino; }
        inline int getSpawnX() { return static_cast<int>(static_cast<float>(_playfield.at(0).size()) * 0.5f) - 2; }
        inline std::vector<Block> getBlocks(const TetrominoType& p_type, const int& p_rotation) {
            std::vector<Blocks::Block> blocks;
            std::vector<int> colorsAtRotation = _blocksDb.at(p_type).at(p_rotation % _blocksDb.at(p_type).size());
            for (size_t i = 0; i < colorsAtRotation.size(); i++) {
                blocks.push_back(Block(colorsAtRotation.at(i)));
            }

            return blocks;
        }
        inline size_t getNumPossibleRotations(const TetrominoType& p_type) {
            return _blocksDb.at(p_type).size();
        }
        inline std::string getPieceTypeStr(TetrominoType p_type) {
            switch (p_type) {
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

    private:
        void _updateGame(const double& p_frameDeltaTime);
        TetrominoType _pickNextTetromino(bool p_firstDraw);
        int _kickPiece(const int& p_x, const int& p_y, const int& p_rotation);
        void _lockPiece();
        std::vector<size_t> _clearLines();
        void _reorganizePlayfieldAfterLineClear();

        inline std::unordered_map<TetrominoType, std::vector<std::vector<int>>> _buildBlocksDb() {
            // Tetromino map from: https://tetris.wiki/Arika_Rotation_System
            std::unordered_map<TetrominoType, std::vector<std::vector<int>>> db;
            std::vector<std::vector<int>> colors;

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
            db.emplace(I, colors);

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
            db.emplace(T, colors);
                    
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
            db.emplace(L, colors);
            
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
            db.emplace(J, colors);

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
            db.emplace(S, colors);
            
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
            db.emplace(Z, colors);
            
            colors = {{
                -1, -1, -1, -1,
                -1,  6,  6, -1,
                -1,  6,  6, -1,
                -1, -1, -1, -1,
            }};
            db.emplace(O, colors);

            return db;
        }

        inline size_t _getAre(const int& p_level, bool p_noLineCleared) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            if (_version == 2) {
                // TAP
                if (p_noLineCleared) {
                    if (p_level >= 900) return 14;
                    else if (p_level >= 800) return 14;
                    else if (p_level >= 700) return 18;
                    else if (p_level >= 600) return 27;
                    else if (p_level >= 500) return 27;
                    else return 27;
                } else {
                    if (p_level >= 900) return 8;
                    else if (p_level >= 800) return 8;
                    else if (p_level >= 700) return 14;
                    else if (p_level >= 600) return 18;
                    else if (p_level >= 500) return 27;
                    else return 27;
                }
            }
            // TGM
            return 30;
        }
        
        inline size_t _getDas(const int& p_level) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            if (_version == 2) {
                // TAP
                if (p_level >= 900) return 8;
                else if (p_level >= 800) return 10;
                else if (p_level >= 700) return 10;
                else if (p_level >= 600) return 10;
                else if (p_level >= 500) return 10;
                else return 16;
            }
            // TGM
            return 16;
        }
        
        inline size_t _getLockDelay(const int& p_level) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            if (_version == 2) {
                // TAP
                if (p_level >= 900) return 17;
                    else if (p_level >= 800) return 30;
                    else if (p_level >= 700) return 30;
                    else if (p_level >= 600) return 30;
                    else if (p_level >= 500) return 30;
                    else return 30;
            }
            // TGM
            return 30;
        }
        
        inline size_t _getLineClear(const int& p_level) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            if (_version == 2) {
                // TAP
                if (p_level >= 900) return 6;
                else if (p_level >= 800) return 6;
                else if (p_level >= 700) return 12;
                else if (p_level >= 600) return 16;
                else if (p_level >= 500) return 25;
                else return 40;
            }
            // TGM
            return 41;
        }
        
        inline size_t _getGravity(const int& p_level) {
            // As in: https://tetris.wiki/Tetris_The_Grand_Master
            // Same for TGM and TAP
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

        /** Returns the number of rows travelled by a given amount of gravity */
        inline size_t _getRowsByGravity(const int& p_gravity) {
            return static_cast<int>(std::ceil(static_cast<float>(p_gravity) / static_cast<float>(256)));
        }
};
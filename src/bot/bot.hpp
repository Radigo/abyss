#pragma once

#include "game/blocks.hpp"

#include <vector>

class Updatable;

class Bot {
    public:
        Bot(Blocks* p_game);
        ~Bot();

        std::vector<std::vector<int>> getDebugTarget();
        std::vector<std::vector<int>> getDebugHorizon();

    private:
        enum HorizonCrawlerDirection {
            Right,
            Down,
            Left,
            Up,
            EndPlayfield
        };

        struct Target {
            Blocks::TetrominoType type = Blocks::TetrominoType::I;
            int x = 0;
            int y = 0;
            int rotation = 0;
        };

        struct HorizonCrawler {
            HorizonCrawler(const int& p_x = -1, const int& p_y = -1, const HorizonCrawlerDirection& p_direction = EndPlayfield) :
            _x(p_x),
            _y(p_y),
            _direction(p_direction) {};
            int _x;
            int _y;
            HorizonCrawlerDirection _direction;

            inline const int getX() { return _x; }
            inline const int getY() { return _y; }
            inline const HorizonCrawlerDirection getDirection() { return _direction; }

            inline const int moveX(const HorizonCrawlerDirection& p_direction) {
                switch (p_direction) {
                    case Left:
                        return _x - 1;
                    case Right:
                        return _x + 1;
                    case Up:
                    case Down:
                    case EndPlayfield:
                        return _x;
                }
            }

            inline const int moveY(const HorizonCrawlerDirection& p_direction) {
                switch (p_direction) {
                    case Up:
                        return _y - 1;
                    case Down:
                        return _y + 1;
                    case Left:
                    case Right:
                    case EndPlayfield:
                        return _y;
                }
            }

            inline const HorizonCrawler getForward() {
                HorizonCrawlerDirection direction = _direction;
                return HorizonCrawler(moveX(direction), moveY(direction), direction);
            };

            inline const HorizonCrawler getRight() {
                HorizonCrawlerDirection direction = static_cast<HorizonCrawlerDirection>((_direction + 1) % 4);
                return HorizonCrawler(moveX(direction), moveY(direction), direction);
            };

            inline const HorizonCrawler getLeft() {
                HorizonCrawlerDirection direction = static_cast<HorizonCrawlerDirection>((_direction + 3) % 4);
                return HorizonCrawler(moveX(direction), moveY(direction), direction);
            };

            inline const HorizonCrawler getBack() {
                HorizonCrawlerDirection direction = static_cast<HorizonCrawlerDirection>((_direction + 2) % 4);
                return HorizonCrawler(moveX(direction), moveY(direction), direction);
            };
        };

        struct Horizon {
            /** Are we a base for comparison? */
            bool isInit;
            /** The successive directions we need to crawl through this horizon */
            std::vector<HorizonCrawler> data;

            /** The average altitude of this horizon */
            inline float getAltitude() {
                ToDo: rework altitude calculation > get the top most of each column
                std::vector<int> columnAltitudes;

                for (HorizonCrawler crawler : data) {
                    Fill up columnAltitudes with index as X position
                    columnAltitudes.push_back(new altitude if X is over columnAltitudes.size())
                    or replace columnAltitudes.at(crawler X if it's higher than previous value)
                }

                size_t cumulatedAltitudes = 0;
                for (size_t altitude : columnAltitudes)
                    cumulatedAltitudes += altitude;

                return static_cast<float>(cumulatedAltitudes) / static_cast<float>(data.size());
            }

            bool operator <(Horizon& p_other) {
                if (p_other.isInit) {
                    // We are necessarily better than an undefined horizon
                    return true;
                }
                if (isInit) {
                    // We are necessarily worst than a defined horizon
                    return false;
                }

                // Look for the lowest altitude
                if (getAltitude() > p_other.getAltitude()) {
                    // Reverse altitude logic becase it's based on Y (where higher Y value means lower coordinates on the playfield)    
                    return true;
                }

                return false;
            };
        };

        Blocks* _game;
        Updatable* _updater;

        std::vector<int> _inputs;
        Horizon _debugHorizon;
        Target _debugTarget;

        void _updateBot(const double& p_frameDeltaTime);

        Target _getTarget(const Blocks::TetrominoType& p_piece);
        Horizon _getHorizon(const std::vector<std::vector<std::pair<int, float>>>& p_playfield, bool p_init);
        std::vector<int> _generateInputs(const std::vector<std::vector<std::pair<int, float>>>& p_playfield, const Target& p_target, const int& p_numFrames);
};
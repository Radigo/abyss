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
            Up,
            Down,
            Left,
            Right,
            EndPlayfield
        };

        struct Target {
            Blocks::TetrominoType type = Blocks::TetrominoType::I;
            int x = 0;
            int y = 0;
            int rotation = 0;
        };

        struct Horizon {
            /** The successive directions we need to crawl through this horizon */
            std::vector<HorizonCrawlerDirection> data;

            size_t startAltitude;

            /** The average altitude of this horizon */
            inline float getAltitude() {
                size_t cumulatedAltitudes = startAltitude;
                size_t prevAltitude = startAltitude;

                for (HorizonCrawlerDirection direction : data) {
                    switch (direction) {
                        case HorizonCrawlerDirection::Up:
                            cumulatedAltitudes += prevAltitude + 1;
                            break;
                        case HorizonCrawlerDirection::Down:
                            cumulatedAltitudes += prevAltitude - 1;
                            break;
                        case HorizonCrawlerDirection::Left:
                        case HorizonCrawlerDirection::Right:
                            cumulatedAltitudes += prevAltitude;
                            break;
                        case HorizonCrawlerDirection::EndPlayfield:
                            break;
                    }
                }

                return static_cast<float>(cumulatedAltitudes) / static_cast<float>(data.size() - 1);// -1 because of "End"
            }

            bool operator <(Horizon& p_other) {
                if (p_other.data.empty()) {
                    // We are necessarily better than an undefined horizon
                    return true;
                }
                if (data.size() < p_other.data.size()) {
                    return getAltitude() < p_other.getAltitude();
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
        Horizon _getHorizon(const std::vector<std::vector<std::pair<int, float>>>& p_playfield);
        std::vector<int> _generateInputs(const std::vector<std::vector<std::pair<int, float>>>& p_playfield, const Target& p_target, const int& p_numFrames);
};
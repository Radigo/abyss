#pragma once

#include <vector>

#include <SDL2/SDL_render.h>

#include "engine/gameobject.hpp"
#include "engine/types.hpp"

using namespace std;

class Displayable : public GameObject {
    public:
        struct Color {
            int r;
            int g;
            int b;
            int a;
            Color(const int p_r, const int p_g, const int p_b, const int p_a = 255) :
            r(p_r),
            g(p_g),
            b(p_b),
            a(p_a) {
            }
        };
        struct DrawablePoint {
            Types::Point point;
            Color color;
            DrawablePoint(const Types::Point p_point, const Color p_color) :
            point(p_point),
            color(p_color) {
            }
        };
        struct DrawableLine {
            Types::Line line;
            Color color;
            DrawableLine(Types::Line p_line, const Color p_color) :
            line(p_line),
            color(p_color) {
            }
        };
        struct DrawableRectangle {
            Types::Rectangle rectangle;
            Color outColor;
            Color inColor;
            DrawableRectangle(Types::Rectangle p_rectangle, const Color p_outColor, const Color p_inColor) :
            rectangle(p_rectangle),
            outColor(p_outColor),
            inColor(p_inColor) {
            }
        };
        struct DisplayableTexture {
            SDL_Texture* texture;
            int initWidth;
            int initHeight;
            DisplayableTexture(SDL_Texture* p_texture = nullptr, const int p_initWidth = 0, const int p_initHeight = 0) :
            texture(p_texture),
            initWidth(p_initWidth),
            initHeight(p_initHeight) {
            }
        };

    protected:
        vector<DrawablePoint> _drawablePoints;
        vector<DrawableLine> _drawableLines;
        vector<DrawableRectangle> _drawableRectangles;
        vector<DisplayableTexture> _textures;

    public:
        Displayable(GameObject* p_parent);
        ~Displayable();

        // Modifiers
        void clear();
        size_t addPoint(const int p_x, const int p_y, const Color p_color = Color(255, 255, 255, 255));
        size_t addLine(const int p_x1, const int p_y1, const int p_x2, const int p_y2, const Color p_color = Color(255, 255, 255, 255));
        size_t addRectangle(const int p_x, const int p_y, const int p_w, const int p_h, const Color p_outColor = Color(255, 255, 255, 255), const Color p_inColor = Color(0, 0, 0, 0));
        size_t addTexture(SDL_Texture* p_texture, const int p_initWidth, const int p_initHeight);
        size_t addTexture(DisplayableTexture p_displayableTexture);
        void updateTexture(size_t p_index, SDL_Texture* p_texture);
        void updateTexture(size_t p_index, DisplayableTexture p_displayableTexture);

        // Getters
        inline vector<DrawablePoint> getDrawablePoints() { return _drawablePoints; }
        inline vector<DrawableLine> getDrawableLines() { return _drawableLines; }
        inline vector<DrawableRectangle> getDrawableRectangles() { return _drawableRectangles; }
        inline vector<DisplayableTexture> getTextures() { return _textures; }
};
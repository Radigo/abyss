#pragma once

#include <vector>

#include "engine/gameobject.hpp"

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
        struct Point {
            int x;
            int y;
            Color color;
            Point(const int p_x, const int p_y, const Color p_color) :
            x(p_x),
            y(p_y),
            color(p_color) {
            }
        };
        struct Line {
            int x1;
            int y1;
            int x2;
            int y2;
            Color color;
            Line(const int p_x1, const int p_y1, const int p_x2, const int p_y2, const Color p_color) :
            x1(p_x1),
            y1(p_y1),
            x2(p_x2),
            y2(p_y2),
            color(p_color) {
            }
        };
        struct Rectangle {
            int x;
            int y;
            int w;
            int h;
            Color outColor;
            Color inColor;
            Rectangle(const int p_x, const int p_y, const int p_w, const int p_h, const Color p_outColor, const Color p_inColor) :
            x(p_x),
            y(p_y),
            w(p_w),
            h(p_h),
            outColor(p_outColor),
            inColor(p_inColor) {
            }
        };

        static std::vector<Displayable*> displayList;

    protected:
        vector<Point> _drawablePoints;
        vector<Line> _drawableLines;
        vector<Rectangle> _drawableRectangles;

    public:
        Displayable();
        ~Displayable();

        // Draw
        void clear();
        void addPoint(const int p_x, const int p_y, const Color p_color = Color(255, 255, 255, 255));
        void addLine(const int p_x1, const int p_y1, const int p_x2, const int p_y2, const Color p_color = Color(255, 255, 255, 255));
        void addRectangle(const int p_x, const int p_y, const int p_w, const int p_h, const Color p_outColor = Color(255, 255, 255, 255), const Color p_inColor = Color(0, 0, 0, 0));

        // Draw
        inline vector<Point> getDrawablePoints() { return _drawablePoints; }
        inline vector<Line> getDrawableLines() { return _drawableLines; }
        inline vector<Rectangle> getDrawableRectangles() { return _drawableRectangles; }
};
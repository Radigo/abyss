#pragma once

class Types {
    public:
        struct Point {
            int x;
            int y;
            Point(const int p_x, const int p_y) :
            x(p_x),
            y(p_y) {
            }

            Point operator+(Point p_a) const {
                return Point(x + p_a.x, y + p_a.y);
            };
            
            Point operator-(Point p_a) const {
                return Point(x - p_a.x, y - p_a.y);
            };
        };
        struct Line {
            int x1;
            int y1;
            int x2;
            int y2;
            Line(const int p_x1, const int p_y1, const int p_x2, const int p_y2) :
            x1(p_x1),
            y1(p_y1),
            x2(p_x2),
            y2(p_y2) {
            }
        };
        struct Rectangle {
            int x;
            int y;
            int w;
            int h;
            Rectangle(const int p_x, const int p_y, const int p_w, const int p_h) :
            x(p_x),
            y(p_y),
            w(p_w),
            h(p_h) {
            }

            inline int left() { return x; }
            inline int right() { return x + w; }
            inline int top() { return y; }
            inline int bottom() { return y + h; }
            inline bool isValid() { return (w > 0) && (h > 0); }
        };
        struct DisplayableFont {
            std::string fontName = "font/Smaco_Regular8.ttf";
            int fontSize = 8;
        };
        enum Fonts {
            Compressed,
            CompressedDouble,
            Condensed,
            CondensedDouble,
            RegularTiny,
            RegularSmall,
            Regular,
            RegularDouble
        };

        public:
            static inline DisplayableFont getFont(Fonts p_font) {
                DisplayableFont font = DisplayableFont();

                switch (p_font) {
                    case Compressed:
                        font.fontName = "font/Smaco_Compressed8.ttf";
                        font.fontSize = 8;
                        break;
                    case CompressedDouble:
                        font.fontName = "font/Smaco_Compressed8.ttf";
                        font.fontSize = 16;
                        break;
                    case Condensed:
                        font.fontName = "font/Smaco_Condensed8.ttf";
                        font.fontSize = 8;
                        break;
                    case CondensedDouble:
                        font.fontName = "font/Smaco_Condensed8.ttf";
                        font.fontSize = 16;
                        break;
                    case RegularTiny:
                        font.fontName = "font/Smaco_Regular6.ttf";
                        font.fontSize = 6;
                        break;
                    case RegularSmall:
                        font.fontName = "font/Smaco_Regular7.ttf";
                        font.fontSize = 7;
                        break;
                    case Regular:
                        font.fontName = "font/Smaco_Regular8.ttf";
                        font.fontSize = 8;
                        break;
                    case RegularDouble:
                        font.fontName = "font/Smaco_Regular8.ttf";
                        font.fontSize = 16;
                        break;
                }

               return font;
            };
};
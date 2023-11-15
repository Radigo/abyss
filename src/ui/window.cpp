#include "window.hpp"

#include "engine/controllable.hpp"

Window::Window(const int p_width, const int p_height) {
    addRectangle(0, 0, p_width, p_height, Color(255, 255, 255, 255), Color(0, 0, 0, 0));
    addRectangle(1, 1, p_width - 2, 22, Color(0, 0, 0, 0), Color(64, 64, 64, 255));
    addRectangle(1, 23, p_width - 2, p_height - 24, Color(0, 0, 0, 0), Color(16, 16, 16, 196));
}

Window::~Window() {

}
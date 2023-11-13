#include "window.hpp"

Window::Window(const int p_width, const int p_height) {
    addRectangle(0, 0, p_width, p_height, Color(255, 255, 255, 255), Color(0, 0, 0, 196));
}

Window::~Window() {

}
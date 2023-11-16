#include "window.hpp"

#include "engine/displayable.hpp"

Window::Window(const int p_width, const int p_height) {
    _display = new Displayable();
    _display->addRectangle(0, 0, p_width, p_height, Displayable::Color(255, 255, 255, 255), Displayable::Color(0, 0, 0, 0));
    _display->addRectangle(1, 1, p_width - 2, 22, Displayable::Color(0, 0, 0, 0), Displayable::Color(64, 64, 64, 255));
    _display->addRectangle(1, 23, p_width - 2, p_height - 24, Displayable::Color(0, 0, 0, 0), Displayable::Color(16, 16, 16, 196));
}

Window::~Window() {

}
#ifndef POSITION_HPP
#define POSITION_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

struct Position : entityx::Component<Position> {
public:
    glm::vec2 position;
    glm::vec2 size;
    Position(glm::vec2 new_position = glm::vec2(0.0f, 0.0f), int width = 0, int height = 0)
        : position(new_position), size({width, height}) {}

    SDL_Rect rect() {
        return SDL_Rect {(int)position.x, (int)position.y, (int)size.x, (int)size.y};
    }
};

#endif

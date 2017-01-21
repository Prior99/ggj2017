#ifndef POSITION_HPP
#define POSITION_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

class Position : entityx::Component<Position> {
public:
    glm::vec2 position;
    Position(glm::vec2 new_position = glm::vec2(0.0f, 0.0f)) : position(new_position) {}
};

#endif

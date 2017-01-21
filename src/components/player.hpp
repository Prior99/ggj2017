#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

struct Player : entityx::Component<Player> {
public:
    bool game_over;
    bool is_jumping;
    bool is_diving;

    glm::vec2 vel;

    virtual ~Player() {
        game_over = false;
    }
};

#endif

#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"

struct Player : entityx::Component<Player> {
    virtual ~Player() {}
};

#endif

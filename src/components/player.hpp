#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"

struct Player : entityx::Component<Player> {
public:
    bool is_jumping;
    bool is_diving;

    virtual ~Player() {}
};

#endif

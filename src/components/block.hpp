#ifndef COMPONENT_BLOCK_HPP
#define COMPONENT_BLOCK_HPP

#include "entityx/entityx.h"
#include <iostream>

static int counter = 0;

struct Block : entityx::Component<Block> {
    public:
        int num;
        Block() : num(counter++) {
            
        }
};

#endif

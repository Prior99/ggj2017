#ifndef COMPONENT_COLLECTABLE_HPP
#define COMPONENT_COLLECTABLE_HPP

#include "entityx/entityx.h"
#include <iostream>

struct Collectable : entityx::Component<Collectable> {
    Collectable(float height): height(height) {}

    float get_height() {
        return height;
    }
  private:
    float height;
};

#endif

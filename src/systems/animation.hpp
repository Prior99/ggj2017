#ifndef ANIMATION_SYSTEM_CPP
#define ANIMATION_SYSTEM_CPP

#include <iostream>
#include "entityx/entityx.h"
#include "components/drawable.hpp"

class AnimationSystem : public entityx::System<AnimationSystem> {
    public:
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Drawable> drawable;
            for(entityx::Entity entity: es.entities_with_components(drawable)) {
                (void) entity;
                if(drawable->hasAnimation()) {
                    drawable->getAnimation().update(dt);
                }
            }
        }
};

#endif

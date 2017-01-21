#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/position.hpp"
#include "events.hpp"
#include "game.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

class CollisionSystem : public entityx::System<CollisionSystem> {
  public:
    CollisionSystem() {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Position> first_position, second_position;
        for (entityx::Entity first_entity : es.entities_with_components(first_position)) {
            for (entityx::Entity second_entity : es.entities_with_components(second_position)) {
                SDL_Rect f_rect = first_position->rect();
                SDL_Rect s_rect = second_position->rect();
                // std::cout << f_rect.x << " " << f_rect.y << " " << f_rect.w << " " << f_rect.h << std::endl;
                // std::cout << s_rect.x << " " << s_rect.y << " " << s_rect.w << " " << s_rect.h << std::endl;
                if (first_entity != second_entity &&
                    SDL_HasIntersection(&f_rect, &s_rect)) {
                        std::cout << "asd" << std::endl;
                        events.emit<CollisionEvent>(first_entity, second_entity);
                }
            }
        }
    }
};

#endif

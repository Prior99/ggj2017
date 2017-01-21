#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "components/collectable.hpp"
#include "events.hpp"
#include "game.hpp"
#include "main_state.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <SDL2/SDL.h>

class CollisionSystem : public entityx::System<CollisionSystem> {
  private:
    MainState *state;
  public:
    CollisionSystem(MainState *state): state(state) {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Position> player_pos, garbage_pos;
        entityx::ComponentHandle<Collectable> collectable;
        entityx::ComponentHandle<Player> player;

        for (entityx::Entity player : es.entities_with_components(player_pos, player)) {
            for (entityx::Entity garbage : es.entities_with_components(garbage_pos, collectable)) {
                (void) player;
                float distance = glm::length(player_pos->position - garbage_pos->position);
                if (distance < 80) {
                    garbage.destroy();
                    state->cash();
                }
            }
        }
    }
};

#endif

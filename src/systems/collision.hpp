#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "components/collectable.hpp"
#include "components/wall.hpp"
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
        entityx::ComponentHandle<Position> player_pos, garbage_pos, wall_pos;
        entityx::ComponentHandle<Collectable> collectable;
        entityx::ComponentHandle<Player> player;
        entityx::ComponentHandle<Wall> wall;

        for (entityx::Entity player_e : es.entities_with_components(player_pos, player)) {
            (void) player_e;
            for (entityx::Entity garbage : es.entities_with_components(garbage_pos, collectable)) {
                float distance = glm::length(player_pos->position - garbage_pos->position);
                if (distance < 100) {
                    garbage.destroy();
                    state->cash();
                }
            }
            for (entityx::Entity wall : es.entities_with_components(wall_pos, wall)) {
                (void)wall;
                player->game_over = true;
            }
        }
    }
};

#endif

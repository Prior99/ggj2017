#ifndef GARBAGECOLLECTORINCLUDEGUASDASD
#define GARBAGECOLLECTORINCLUDEGUASDASD

#include "components/position.hpp"
#include "events.hpp"
#include "game.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

class GarbageCollectionSystem : public entityx::System<GarbageCollectionSystem> {
private:
    Game* const game;
public:
    GarbageCollectionSystem(Game* game): game(game) {
    }

    void update(entityx::EntityManager& em, entityx::EventManager& events, double dt) {
        auto player = game->get_player();
        auto player_position = player.component<Position>();
        float player_x = player_position->position.x;

        entityx::ComponentHandle<Position> position;
        for (entityx::Entity first_entity : em.entities_with_components(position)) {
            if(position->position.x < player_x - DESPAWN_OFFSET) {
                first_entity.destroy();
            }
        }
    }
};

#endif

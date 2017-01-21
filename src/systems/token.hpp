#ifndef SYSTEM_TOKEN_HPP
#define SYSTEM_TOKEN_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glm/vec2.hpp>

#include "game.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"

#include "components/position.hpp"
#include "components/collectable.hpp"

#include "../spawners.hpp"

class TokenSystem : public entityx::System<TokenSystem> {
private:
    Game *game;
    entityx::TimeDelta local_dt;
public:
    TokenSystem(Game *game): game(game), local_dt(0) {
		std::srand(time(nullptr));
	}

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        entityx::ComponentHandle<Collectable> collectable;
        entityx::ComponentHandle<Position> position;
        for(entityx::Entity e: entities.entities_with_components(collectable, position)) {
            (void) e;
            glm::vec2 pos = position->position;
            if(pos.y < collectable->get_height()) {
                position->position = glm::vec2(pos.x, pos.y + DROP_SPEED * dt);
            }
        }

        local_dt += dt;
        if (local_dt > 0.3) {
            local_dt = 0;

            float player_x = game->get_player().component<Position>()->position.x;
            float y = std::rand()/(float)RAND_MAX * HEIGHT/COLLECTABLE_BAND + PROTECTED_TOP;
    		spawn_collectable(entities, player_x + WIDTH - 50, PROTECTED_TOP, y);
        }
    }
};

#endif

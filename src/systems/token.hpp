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
    entityx::Entity heli;
    float heli_x;
public:
    TokenSystem(Game *game): game(game), local_dt(0) {
		std::srand(time(nullptr));
	}

    void configure(entityx::EntityManager& entities, entityx::EventManager& events) override {
        heli_x = WIDTH - 240 - 20;
        heli = entities.create();
        heli.assign<Position>(glm::vec2(heli_x, PROTECTED_TOP - 110));
        AnimationCollection heliAnimation = AnimationCollection("heli");
        heliAnimation.addAnimation("animate", 0, 3, 0.3, glm::vec2(240, 100));
        heliAnimation.setAnimation("animate", AnimationPlaybackType::LOOP);
        heli.assign<Drawable>("heli", 240, 100, heliAnimation);
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

        float player_x = game->get_player().component<Position>()->position.x;
        heli.component<Position>()->position.x = player_x + heli_x - PLAYER_OFFSET;

        local_dt += dt;
        double currentSpawnPeriod = TOKEN_SPAWN_PERIOD - TOKEN_SPAWN_VARIATION + std::rand()/(float)RAND_MAX * 2 * TOKEN_SPAWN_VARIATION;
        if (local_dt > currentSpawnPeriod) {
            local_dt = 0;

            float y = std::rand()/(float)RAND_MAX * HEIGHT/COLLECTABLE_BAND + PROTECTED_TOP;
    		spawn_collectable(entities, player_x + WIDTH - 150 - PLAYER_OFFSET, PROTECTED_TOP, y, (int)(std::rand()/(float)RAND_MAX * 8 + 1));
        }
    }
};

#endif

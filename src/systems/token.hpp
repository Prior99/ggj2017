#ifndef SYSTEM_TOKEN_HPP
#define SYSTEM_TOKEN_HPP

#include <iostream>
#include <glm/vec2.hpp>

#include "game.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"

#include "components/position.hpp"
#include "components/collectable.hpp"
#include "components/decay.hpp"

#include "../spawners.hpp"

class TokenSystem : public entityx::System<TokenSystem> {
private:
    Game *game;
    entityx::TimeDelta local_dt;
    entityx::Entity heli;
    float total_time;
    float last_spawn_y;
public:
    TokenSystem(Game *game): game(game), local_dt(0), total_time(0.0) {
        last_spawn_y = std::rand()/(float)RAND_MAX * HEIGHT/COLLECTABLE_BAND + PROTECTED_TOP;
	}

    void configure(entityx::EntityManager& entities, entityx::EventManager& events) override {
        heli = spawn_heli(entities);
    }

    void normal_update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
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

        float x_offset = HELI_MOVE_MAX * glm::sin(total_time / 1.234);
        float y_offset = HELI_MOVE_MAX * glm::cos(total_time / 0.735);
        float new_x = player_x - PLAYER_OFFSET + HELI_SCREEN_POS_X + x_offset;
        float new_y = HELI_SCREEN_POS_Y + y_offset;
        heli.component<Position>()->position = glm::vec2(new_x, new_y);

        local_dt += dt;
        double currentSpawnPeriod = TOKEN_SPAWN_PERIOD - TOKEN_SPAWN_VARIATION + std::rand()/(float)RAND_MAX * 2 * TOKEN_SPAWN_VARIATION;
        if (local_dt > currentSpawnPeriod) {
            local_dt = 0;

            float suggested_y = last_spawn_y - MAX_SPAWN_DISTANCE + std::rand()/(float)RAND_MAX * 2 * MAX_SPAWN_DISTANCE;
            float y = glm::clamp(suggested_y, PROTECTED_TOP, PROTECTED_TOP + HEIGHT/ COLLECTABLE_BAND);
            spawn_collectable(entities, player_x + WIDTH - 100 - PLAYER_OFFSET, PROTECTED_TOP, y, rand() % 8 + 1);
        }
    }

    void dead_update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        float player_x = game->get_player().component<Position>()->position.x;
        for(int i=0; i<10; i++) {
            float x = rand() % WIDTH + player_x - PLAYER_OFFSET;
            float y = rand() % HEIGHT;
            spawn_collectable(entities, x, y, y, rand() % 8 + 1, true);
        }

        entityx::ComponentHandle<Decay> decay;
        for(entityx::Entity e: entities.entities_with_components(decay)) {
            if((decay->dt += dt) > 3.0) {
                e.destroy();
            }
        }
    }

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        total_time += dt;
        if(!game->get_player().component<Player>()->game_over)
            normal_update(entities, events, dt);
        else
            dead_update(entities, events, dt);
    }
};

#endif

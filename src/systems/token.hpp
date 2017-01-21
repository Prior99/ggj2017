#ifndef SYSTEM_TOKEN_HPP
#define SYSTEM_TOKEN_HPP

#include <iostream>
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
    entityx::Entity mermaid;

    float total_time;
    float last_spawn_y;
public:
    TokenSystem(Game *game): game(game), local_dt(0), total_time(0.0) {
        last_spawn_y = std::rand()/(float)RAND_MAX * HEIGHT/COLLECTABLE_BAND + PROTECTED_TOP;
	}

    void configure(entityx::EntityManager& entities, entityx::EventManager& events) override {
        heli = spawn_heli(entities);
        mermaid = spawn_mermaid(entities);
    }

    void update_heli(float player_x) {
        float x_offset = HELI_MOVE_MAX * glm::sin(total_time / 1.234);
        float y_offset = HELI_MOVE_MAX * glm::cos(total_time / 0.735);
        float x = player_x - PLAYER_OFFSET + HELI_SCREEN_POS_X + x_offset;
        float y = HELI_SCREEN_POS_Y + y_offset;
        heli.component<Position>()->position = glm::vec2(x, y);
    }

    void update_mermaid(float player_x) {
        float x_offset = HELI_MOVE_MAX * glm::sin(total_time / 0.9434);
        float y_offset = HELI_MOVE_MAX * glm::cos(total_time / 1.7435);
        float x = player_x + WAVE_GENERATOR_X - MERMAID_WIDTH / 2.0 + x_offset;
        float y = HEIGHT - MERMAID_HEIGHT - 60 + y_offset;
        mermaid.component<Position>()->position = glm::vec2(x, y);
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
        update_mermaid(player_x);
        update_heli(player_x);

        local_dt += dt;
        double currentSpawnPeriod = TOKEN_SPAWN_PERIOD + (2.0 * std::rand()/(float)RAND_MAX - 1.0) *  TOKEN_SPAWN_VARIATION;
        if (local_dt > currentSpawnPeriod) {
            local_dt = 0;

            float suggested_y = last_spawn_y + (2.0 * rand()/(float)RAND_MAX - 1.0) * MAX_SPAWN_DISTANCE;
            last_spawn_y = glm::clamp(suggested_y, PROTECTED_TOP, PROTECTED_TOP + HEIGHT/ COLLECTABLE_BAND);
            int gbg_id = rand() % 8 + 1;
            auto heli_pos = heli.component<Position>()->position;
            float heli_x = heli_pos.x + HELI_WIDTH/2.0;
            float x_pos = heli_x + WIDTH - 100 - PLAYER_OFFSET;
            float y_pos = heli_pos.y + HELI_HEIGHT;
            spawn_collectable(entities, x_pos, y_pos, last_spawn_y, gbg_id);
        }
    }

    void dead_update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        float player_x = game->get_player().component<Position>()->position.x;
        update_heli(player_x);
        for(int i=0; i<10; i++) {
            float x = rand() % WIDTH + player_x - PLAYER_OFFSET;
            float y = rand() % HEIGHT;
            spawn_collectable(entities, x, y, y, rand() % 8 + 1);
        }
    }

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        total_time += dt;
        if(!game->get_player().component<Player>()->game_over)
        {
            normal_update(entities, events, dt);
        }
        else
        {
            dead_update(entities, events, dt);
        }
    }
};

#endif

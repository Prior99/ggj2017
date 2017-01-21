#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>

#include "game.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/block.hpp"

#include "../spawners.hpp"

#define BLOCK_SIZE 40
#define MAX_CHANGE 15

class MapSystem : public entityx::System<MapSystem> {
private:
    Game *game;
    entityx::TimeDelta local_dt;
    float left_border_position;
    float last_height = HEIGHT / 2.0f;
public:
    MapSystem(Game *game): game(game), local_dt(0), left_border_position(0.0) {}

    float WAVE_GENERATOR_X = WIDTH * 0.6;

    void wave_it(entityx::EntityManager& entities) {
        auto player = game->get_player();
        auto player_position = player.component<Position>();
        auto mid_position = player_position->get_x() + WAVE_GENERATOR_X;
        entityx::ComponentHandle<Block> block;
        entityx::ComponentHandle<Position> position;
        float band = HEIGHT * MAX_BLOCK_HEIGHT;
        float lower_offset = (HEIGHT - band) / 2.0f;
        float amplitude = this->game->take_amplitude();
        float inverted_height = lower_offset + amplitude * band;
        float height = HEIGHT - inverted_height;
        // std::cout << (last_height - MAX_CHANGE) << " < " << height << " < " << (last_height + MAX_CHANGE) << std::endl;
        height = glm::clamp(height, last_height - MAX_CHANGE, last_height + MAX_CHANGE);
        this->last_height = height;
        for (auto entity: entities.entities_with_components(block, position)) {
            (void) entity;
            auto x = position->get_x();
            if (x > mid_position) {
                float amplitude = height - HEIGHT / 2;
                float distance_to_mid = x - mid_position;
                float max_distance = WIDTH / 2;
                float pi = glm::pi<float>();
                float y = glm::sin((distance_to_mid / max_distance) * pi - pi) * amplitude + height;
                position->set_y(y);
            }
        }
    }

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        auto player = game->get_player();
        auto position = player.component<Position>();
        float position_of_player = position->get_x() + 10.0;
        position->set_position(glm::vec2(position_of_player, position->get_y()));

        if ((int)position_of_player % BLOCK_WIDTH == 0) {
            float position_of_block = position_of_player + BLOCK_SPAWN_OFFSET;
            spawn_block(entities, position_of_block);
        }
        this->wave_it(entities);

    }
};

#endif

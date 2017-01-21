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
    float WAVE_GENERATOR_X = WIDTH * 0.6;
public:
    MapSystem(Game *game): game(game), local_dt(0), left_border_position(0.0) {}
    virtual void configure(entityx::EntityManager& entities, entityx::EventManager& events) {
        for(int i=-1; i<WIDTH/BLOCK_WIDTH + 10; i++)
            spawn_block(entities, i*BLOCK_WIDTH);
    }

    void wave_it(entityx::EntityManager& entities) {
        auto player = game->get_player();
        auto player_position = player.component<Position>();
        auto mid_position = player_position->position.x + WAVE_GENERATOR_X;
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
            auto x = position->position.x;
            if (x > mid_position) {
                float amplitude = height - HEIGHT / 2;
                float distance_to_mid = x - mid_position;
                float max_distance = WIDTH / 2;
                float pi = glm::pi<float>();
                float y = glm::sin((distance_to_mid / max_distance) * pi - pi) * amplitude + height;
                position->position.y = y;
            }
        }
    }

    void move_it(entityx::EntityManager& entities) {
        auto player = game->get_player();
        auto player_position = player.component<Position>();
        auto player_x = player_position->position.x;

        float closest_left = - 1.0/0.0;
        entityx::Entity left_block;
        float closest_right = 1.0/0.0;
        entityx::Entity right_block;

        entityx::ComponentHandle<Block> block;
        entityx::ComponentHandle<Position> position;

        // Find the left and right blocks closest to the player.
        for (auto entity: entities.entities_with_components(block, position))
        {
            auto x = position->position.x;
            if (x > player_x && x < closest_right)
                right_block = entity;
            if (x < player_x && x > closest_left)
                left_block = entity;
        }

        std::cout << left_block << std::endl;
        std::cout << right_block << std::endl;
        float left_height = left_block.component<Position>()->position.y;
        float right_height = right_block.component<Position>()->position.y;

        player_position->position.y = (left_height + right_height) / 2.0f;
    }

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        auto player = game->get_player();
        auto position = player.component<Position>();
        float new_player_x = position->position.x + 10.0;
        position->position = glm::vec2(new_player_x, position->position.y);

        if ((int)new_player_x % BLOCK_WIDTH == 0) {
            float position_of_block = new_player_x + BLOCK_SPAWN_OFFSET;
            spawn_block(entities, position_of_block);
        }
        this->wave_it(entities);
        this->move_it(entities);
    }
};

#endif

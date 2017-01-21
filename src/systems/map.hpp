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

class MapSystem : public entityx::System<MapSystem> {
private:
    Game *game;
    entityx::TimeDelta local_dt;
    float left_border_position;
public:
    MapSystem(Game *game): game(game), local_dt(0), left_border_position(0.0) {}

    float WAVE_GENERATOR_X = WIDTH * 0.6;

    void wave_it(entityx::EntityManager& entities) {
        auto player = game->get_player();
        auto player_position = player.component<Position>();
        auto mid_position = player_position->get_x() + WAVE_GENERATOR_X;
        entityx::ComponentHandle<Block> block;
        entityx::ComponentHandle<Position> position;
        auto height = this->game->take_amplitude() - 0.5f;
        height *= MAX_BLOCK_HEIGHT;
        for (auto entity: entities.entities_with_components(block, position)) {
            std::cout << "height: " << height;
            (void) entity;
            float x = glm::abs(position->get_x() - mid_position);
            auto influence = (glm::cos(x * 2.0f * glm::pi<float>() / WIDTH) + 1.0f)/2.0f;
            influence *= 0.5;
            if (position->get_x() < mid_position - 200) {
                influence *= 0.1;
            }
            if (glm::abs(position->get_y() - HEIGHT /2) < HEIGHT * MAX_BLOCK_HEIGHT * 0.5f) {
                float y;
                std::cout << " influence:" << influence;
                if (position->get_x() < mid_position - 50) {
                    y = position->get_y() - influence * height * HEIGHT / 2.0f;
                } else {
                    y = HEIGHT/2.0f - influence * height * HEIGHT / 2.0f;
                }
                std::cout << " y:" << y;
                position->set_position(glm::vec2(position->get_x(), y));
                std::cout << std::endl;
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

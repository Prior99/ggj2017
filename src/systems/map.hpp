#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include <iostream>
#include <glm/vec2.hpp>

#include "game.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/block.hpp"

#include "../spawners.hpp"

const entityx::TimeDelta THRESHOLD = 0.1;
const int SPAWN_X = 800;

class MapSystem : public entityx::System<MapSystem> {
private:
    Game *game;
    entityx::TimeDelta local_dt;
    float left_border_position;
public:
    MapSystem(Game *game): game(game), local_dt(0), left_border_position(0.0) {}

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        auto player = game->get_player();
        auto position = player.component<Position>();
        float position_of_player = position->get_x() + 10.0;
        position->set_position(glm::vec2(position_of_player, position->get_y()));

        std::cout << position_of_player << std::endl;

        float position_of_block = position_of_player + BLOCK_SPAWN_OFFSET;
        spawn_block(entities, position_of_block, this->game->take_amplitude());
    }
};

#endif

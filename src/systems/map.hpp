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

const entityx::TimeDelta THRESHOLD = 1;
const int SPAWN_X = 120;

class MapSystem : public entityx::System<MapSystem> {
  public:
	MapSystem(): local_dt(0) {

	}

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
		local_dt += dt;
		if(local_dt > THRESHOLD) {
			local_dt = 0;
			entityx::ComponentHandle<Block> block;
			entityx::ComponentHandle<Position> position;
			entityx::ComponentHandle<Drawable> drawable;
			for(entityx::Entity block: entities.entities_with_components(block, position, drawable)) {
				(void) block;
				glm::vec2 oldPos = position->position();
				position->set_position(glm::vec2(oldPos.x - drawable->getWidth(), oldPos.y));
			}

            // TODO
            float position_of_player = 0.0;
            float position_of_block = position_of_player + BLOCK_SPAWN_OFFSET;
            spawn_block(entities, position_of_block, 0.5f);
		}
	}

  private:
    entityx::TimeDelta local_dt;
};

#endif

#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include "game.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/block.hpp"

#include <iostream>

const entityx::TimeDelta THRESHOLD = 0.1;
const int SPAWN_X = 800;

class MapSystem : public entityx::System<MapSystem> {
  public:
    MapSystem(Game *game): game(game), local_dt(0) {

    }

    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) {
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
            this->spawn_block(entities, dt);
        }
    }

  private:
    Game *game;
    void spawn_block(entityx::EntityManager &entities, entityx::TimeDelta dt) {
        entityx::Entity block = entities.create();
        float top = 600 - this->game->take_amplitude() * 600;
        std::cout << top << std::endl;
        block.assign<Position>(glm::vec2(SPAWN_X, top));
        block.assign<Drawable>("block", 50, 600);
        block.assign<Block>();
    }
    entityx::TimeDelta local_dt;
};

#endif

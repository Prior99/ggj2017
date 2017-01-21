#ifndef SPAWNERS_HPP
#define SPAWNERS_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/collectable.hpp"
#include "components/block.hpp"
#include "components/player.hpp"

#include "game_config.hpp"

void spawn_block(entityx::EntityManager& entities, float position) {
    entityx::Entity block = entities.create();

    block.assign<Position>(glm::vec2(position, HEIGHT/2.0));
    // TODO htf scale the images
    block.assign<Drawable>("block", BLOCK_WIDTH, BLOCK_HEIGHT);
    block.assign<Block>();
}

void spawn_collectable(entityx::EntityManager& entities, float position, float start_height, float end_height) {
    entityx::Entity collectable = entities.create();
    collectable.assign<Position>(glm::vec2(position, start_height));
    collectable.assign<Drawable>("trash", 20, 20);
    collectable.assign<Collectable>(end_height);
}

entityx::Entity spawn_player(entityx::EntityManager& entities) {
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(300.f, 400.f));
    player.assign<Drawable>("gradient", 100, 100);
    player.assign<Player>();
    return player;
}

#endif

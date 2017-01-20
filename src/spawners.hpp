#ifndef LOLOLROFLINCLUDEGUARD
#define LOLOLROFLINCLUDEGUARD

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/collectable.hpp"
#include "components/block.hpp"
#include "components/player.hpp"

#include "game_config.hpp"

void spawn_block(entityx::EntityManager& entities, float position, float height) {
    entityx::Entity block = entities.create();
    float absolute_height = height * MAX_BLOCK_HEIGHT * HEIGHT;

    block.assign<Position>(glm::vec2(BLOCK_WIDTH, absolute_height));
    // TODO htf scale the images
    block.assign<Drawable>("mock_block", BLOCK_WIDTH, absolute_height);
    block.assign<Block>();
}

void spawn_collectable(entityx::EntityManager& entities, float position, float height) {
    entityx::Entity collectable = entities.create();
    collectable.assign<Position>();
}

void spawn_player(entityx::EntityManager& entities) {
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(300.f, 400.f));
    player.assign<Drawable>("gradient", 100, 100);
    player.assign<Player>();
}

#endif

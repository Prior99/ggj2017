#ifndef SPAWNERS_HPP
#define SPAWNERS_HPP

#include <sstream>
#include <iomanip>

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/collectable.hpp"
#include "components/block.hpp"
#include "components/player.hpp"
#include "components/wall.hpp"

#include "game_config.hpp"

void spawn_block(entityx::EntityManager& entities, float position) {
    entityx::Entity block = entities.create();

    block.assign<Position>(glm::vec2(position, HEIGHT/2.0));
    // TODO htf scale the images
    block.assign<Drawable>("block", BLOCK_WIDTH, BLOCK_HEIGHT);
    block.assign<Block>();
}

void spawn_collectable(entityx::EntityManager& entities, float position, float start_height, float end_height, int textureNr) {
    entityx::Entity collectable = entities.create();
    // TODO specify the size of the collectable here.
    collectable.assign<Position>(glm::vec2(position, start_height), 50, 50);
    std::stringstream stream;
    stream << "trash" << std::setfill('0') << std::setw(2) << textureNr;
    collectable.assign<Drawable>(stream.str(), 40, 40);
    collectable.assign<Collectable>(end_height);
}

entityx::Entity spawn_player(entityx::EntityManager& entities) {
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(300.f, 400.f), 100, 146);
    AnimationCollection anim_collection("jonny");
    anim_collection.addAnimation("normal", 0, 8, 2.0f, glm::vec2(100, 146));
    anim_collection.setAnimation("normal", AnimationPlaybackType::LOOP);
    player.assign<Drawable>("jonny", 100, 146, anim_collection, glm::vec2(50,140));
    player.assign<Player>();
    return player;
}

void spawn_wall(entityx::EntityManager& entities) {
    entityx::Entity wall = entities.create();
    wall.assign<Drawable>("block", BLOCK_WIDTH, BLOCK_HEIGHT);
    wall.assign<Position>(glm::vec2(300.f, 400.f), 100, 146);
    wall.assign<Wall>();
}

#endif

#ifndef SPAWNERS_HPP
#define SPAWNERS_HPP

#include <sstream>
#include <iomanip>

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include "animation.hpp"

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/collectable.hpp"
#include "components/block.hpp"
#include "components/player.hpp"
#include "components/wall.hpp"
#include "components/overlay.hpp"

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
    collectable.assign<Position>(glm::vec2(position, start_height));
    std::stringstream stream;
    stream << "trash" << std::setfill('0') << std::setw(2) << textureNr;
    collectable.assign<Drawable>(stream.str(), 40, 40);
    collectable.assign<Collectable>(end_height);
}

entityx::Entity spawn_player(entityx::EntityManager& entities) {
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(300.f, 400.f));
    AnimationCollection anim_collection("jonny");
    anim_collection.addAnimation("normal", 0, 8, 2.0f, glm::vec2(100, 146));
    anim_collection.setAnimation("normal", AnimationPlaybackType::LOOP);
    player.assign<Drawable>("jonny", 100, 146, anim_collection, glm::vec2(50,140));
    player.assign<Player>();
    return player;
}

void spawn_wall(entityx::EntityManager& entities, float position) {
    entityx::Entity wall = entities.create();
    wall.assign<Drawable>("wall", 160, 732);
    // TODO make random
    wall.assign<Position>(glm::vec2(position, HEIGHT/2.0));
    wall.assign<Wall>();
}

void spawn_mermaid(entityx::EntityManager& entities) {
    entityx::Entity mermaid = entities.create();
    auto merm_pos = glm::vec2(PLAYER_OFFSET + WAVE_GENERATOR_X + 0.5 * BLOCK_WIDTH - 82, HEIGHT - 127 - 60);
    mermaid.assign<Position>(merm_pos);
    AnimationCollection mermaidAnimation = AnimationCollection("mermaid");
    mermaidAnimation.addAnimation("move", 0, 7, 1.5, glm::vec2(125, 127));
    mermaidAnimation.setAnimation("move", AnimationPlaybackType::LOOP);
    mermaid.assign<Drawable>("mermaid", 125, 127, mermaidAnimation);
    mermaid.assign<Overlay>();
}

#endif

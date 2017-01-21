#ifndef SPAWNERS_HPP
#define SPAWNERS_HPP

#include <sstream>
#include <iomanip>
#include <string>

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
#include "components/decay.hpp"

#include "game_config.hpp"

void spawn_block(entityx::EntityManager& entities, float position) {
    entityx::Entity block = entities.create();

    block.assign<Position>(glm::vec2(position, HEIGHT/2.0));
    // TODO htf scale the images
    block.assign<Drawable>("block", BLOCK_WIDTH, BLOCK_HEIGHT);
    block.assign<Block>();
}

void spawn_collectable(entityx::EntityManager& entities, float position, float start_height, float end_height, int textureNr, bool decay = false) {
    entityx::Entity collectable = entities.create();
    // TODO specify the size of the collectable here.
    collectable.assign<Position>(glm::vec2(position, start_height));
    std::stringstream stream;
    stream << "trash" << std::setfill('0') << std::setw(2) << textureNr;
    std::string id = stream.str();
    collectable.assign<Drawable>(id, 50, 50);
    collectable.assign<Collectable>(end_height);
    if(decay) {
        collectable.assign<Decay>();
    }
}

entityx::Entity spawn_player(entityx::EntityManager& entities) {
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(300.f, 400.f));
    AnimationCollection anim_collection("jonny");
    anim_collection.addAnimation("normal", 0, 7, 2.0f, glm::vec2(100, 143));
    anim_collection.setAnimation("normal", AnimationPlaybackType::LOOP);
    player.assign<Drawable>("jonny", 100, 143, anim_collection, glm::vec2(50,140));
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

entityx::Entity spawn_mermaid(entityx::EntityManager& entities) {
    entityx::Entity mermaid = entities.create();
    mermaid.assign<Position>(glm::vec2(0.0, 0.0));
    AnimationCollection mermaidAnimation = AnimationCollection("mermaid");
    auto merm_size = glm::vec2(MERMAID_WIDTH, MERMAID_HEIGHT);
    mermaidAnimation.addAnimation("move", 0, 7, 1.5, merm_size);
    mermaidAnimation.setAnimation("move", AnimationPlaybackType::LOOP);
    mermaid.assign<Drawable>("mermaid", MERMAID_WIDTH, MERMAID_HEIGHT, mermaidAnimation);
    mermaid.assign<Overlay>();
    return mermaid;
}

entityx::Entity spawn_heli(entityx::EntityManager& entities) {
    entityx::Entity heli = entities.create();
    heli.assign<Position>(glm::vec2(0.0, 0.0));
    AnimationCollection heliAnimation = AnimationCollection("heli");
    heliAnimation.addAnimation("animate", 0, 3, 0.3, glm::vec2(HELI_WIDTH, HELI_HEIGHT));
    heliAnimation.setAnimation("animate", AnimationPlaybackType::LOOP);
    heli.assign<Drawable>("heli", HELI_WIDTH, HELI_HEIGHT, heliAnimation);
    return heli;
}

#endif

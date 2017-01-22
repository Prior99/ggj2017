#include <sstream>
#include <iomanip>
#include <string>

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include "animation.hpp"
#include "spawners.hpp"
#include "game_config.hpp"

void spawn_block(entityx::EntityManager& entities, float position) {
    entityx::Entity block = entities.create();

    block.assign<Position>(glm::vec2(position, HEIGHT/2.0));
    // TODO htf scale the images
    block.assign<Drawable>("block", BLOCK_WIDTH, BLOCK_HEIGHT);
    block.assign<Block>();
    block.assign<Layer<1>>();
}

void spawn_collectable(entityx::EntityManager& entities, float position, float start_height, float end_height, int textureNr) {
    entityx::Entity collectable = entities.create();
    // TODO specify the size of the collectable here.
    collectable.assign<Position>(glm::vec2(position, start_height));
    std::stringstream stream;
    stream << "trash" << std::setfill('0') << std::setw(2) << textureNr;
    std::string id = stream.str();
    collectable.assign<Drawable>(id, 50, 50);
    collectable.assign<Collectable>(end_height);
    collectable.assign<Layer<2>>();
}

entityx::Entity spawn_player(entityx::EntityManager& entities) {
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(300.f, 400.f));
    AnimationCollection anim_collection("jonny");
    anim_collection.addAnimation("normal", 0, 7, 2.0f, glm::vec2(100, 143));
    anim_collection.setAnimation("normal", AnimationPlaybackType::LOOP);
    player.assign<Drawable>("jonny", 100, 143, anim_collection, glm::vec2(50,140));
    player.assign<Player>();
    player.assign<Layer<1>>();
    return player;
}

void spawn_wall(entityx::EntityManager& entities, float position) {
    entityx::Entity wall = entities.create();
    wall.assign<Drawable>("wall", 160, 732);

    int y_pos = MAX_WALL_HEIGHT + rand() % (MIN_WALL_HEIGHT - MAX_WALL_HEIGHT);
    wall.assign<Position>(glm::vec2(position, y_pos));
    wall.assign<Obstacle>();
    wall.assign<Layer<2>>();
}

void spawn_cloud(entityx::EntityManager& entities, float position) {
    entityx::Entity cloud = entities.create();
    AnimationCollection anim_collection("cloud");
    anim_collection.addAnimation("normal", 0, 6, 1.0f, glm::vec2(CLOUD_WIDTH, CLOUD_HEIGHT));
    anim_collection.setAnimation("normal", AnimationPlaybackType::LOOP);
    cloud.assign<Drawable>("cloud", CLOUD_WIDTH, CLOUD_HEIGHT, anim_collection);

    int y_pos = MAX_CLOUD_HEIGHT + rand() % (MIN_CLOUD_HEIGHT - MAX_CLOUD_HEIGHT);
    cloud.assign<Position>(glm::vec2(position, y_pos));
    cloud.assign<Obstacle>();
    cloud.assign<Layer<2>>();
}

entityx::Entity spawn_mermaid(entityx::EntityManager& entities) {
    entityx::Entity mermaid = entities.create();
    mermaid.assign<Position>(glm::vec2(0.0, 0.0));
    AnimationCollection mermaidAnimation = AnimationCollection("mermaid");
    auto merm_size = glm::vec2(MERMAID_WIDTH, MERMAID_HEIGHT);
    mermaidAnimation.addAnimation("move", 0, 7, 1.5, merm_size);
    mermaidAnimation.setAnimation("move", AnimationPlaybackType::LOOP);
    mermaid.assign<Drawable>("mermaid", MERMAID_WIDTH, MERMAID_HEIGHT, mermaidAnimation);
    mermaid.assign<Layer<3>>();
    return mermaid;
}

entityx::Entity spawn_heli(entityx::EntityManager& entities) {
    entityx::Entity heli = entities.create();
    heli.assign<Position>(glm::vec2(0.0, 0.0));
    AnimationCollection heliAnimation = AnimationCollection("heli");
    heliAnimation.addAnimation("animate", 0, 3, 0.3, glm::vec2(HELI_WIDTH, HELI_HEIGHT));
    heliAnimation.setAnimation("animate", AnimationPlaybackType::LOOP);
    heli.assign<Drawable>("heli", HELI_WIDTH, HELI_HEIGHT, heliAnimation);
    heli.assign<Layer<3>>();
    return heli;
}

void spawn_death_board(entityx::EntityManager& entities) {
    entityx::Entity death_board = entities.create();
    death_board.assign<Position>(glm::vec2(0.0, 0.0));
    death_board.assign<Drawable>("death_board", DEATH_BOARD_WIDTH, DEATH_BOARD_HEIGHT);
    death_board.assign<Overlay>();
}

void spawn_init_screen(entityx::EntityManager& entities) {
    entityx::Entity init_screen = entities.create();
    init_screen.assign<Position>(glm::vec2(0.0, 0.0));
    init_screen.assign<Drawable>("init_screen", INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT);
    init_screen.assign<Overlay>();
}

#ifndef SPAWNERS_HPP
#define SPAWNERS_HPP

#include "entityx/entityx.h"

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/collectable.hpp"
#include "components/block.hpp"
#include "components/player.hpp"
#include "components/obstacle.hpp"
#include "components/overlay.hpp"
#include "components/layer.hpp"

void spawn_block(entityx::EntityManager& entities, float position);
void spawn_collectable(entityx::EntityManager& entities, float position, float start_height, float end_height, int textureNr);
entityx::Entity spawn_player(entityx::EntityManager& entities);
void spawn_wall(entityx::EntityManager& entities, float position);
void spawn_cloud(entityx::EntityManager& entities, float position);
entityx::Entity spawn_mermaid(entityx::EntityManager& entities);
entityx::Entity spawn_heli(entityx::EntityManager& entities);
void spawn_death_board(entityx::EntityManager& entities);
void spawn_init_screen(entityx::EntityManager& entities);

#endif

#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include "glm/vec2.hpp"
#include <string>

/*
 *  Global config values for easy tweaking and changing
 */

static const bool DEBUG = false;

static const int WIDTH = 1300;
static const int HEIGHT = 768;

static const float MAX_BLOCK_HEIGHT = 0.75;
static const int BLOCK_HEIGHT = 600;
static const int BLOCK_WIDTH = 50;

static const float DESPAWN_OFFSET = 100.0;
static const float BLOCK_SPAWN_OFFSET = 1300.0;

#endif

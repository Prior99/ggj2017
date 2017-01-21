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
static const int PLAYER_OFFSET = 200;

static const float DESPAWN_OFFSET = 400.0;
static const float BLOCK_SPAWN_OFFSET = 1300.0;

static const float COLLECTABLE_BAND = 2; // portion of the screen in which items spawn
static const float PROTECTED_TOP = 120; // pixels at top, in which no items spawn
static const float DROP_SPEED = 800;
static const double TOKEN_SPAWN_PERIOD = 0.8;
static const double TOKEN_SPAWN_VARIATION = 0.5;
static const float TEXT_DURATION = 2.0f;
#endif

#include "main_state.hpp"

#include "components/drawable.hpp"
#include "components/position.hpp"

#include "systems/collision.hpp"
#include "systems/controls.hpp"
#include "systems/draw.hpp"
#include "systems/map.hpp"

#include "spawners.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

MainState::MainState(Game *game) : m_game(game) {
}

MainState::~MainState() {
}

int MainState::init() {
    m_systems.add<DrawSystem>(m_game);
    m_systems.add<ControlSystem>();
    m_systems.add<CollisionSystem>();
    m_systems.add<MapSystem>();
    m_systems.configure();

    spawn_player(m_entities);
    return 0;
}

void MainState::update(double dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            m_game->shutdown();
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                m_game->shutdown();
            }
        }
    }

    m_systems.update_all(dt);
}

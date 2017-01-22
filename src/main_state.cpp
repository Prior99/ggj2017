#include "main_state.hpp"

#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/highscore.hpp"
#include "components/text.hpp"
#include "components/game-text.hpp"
#include "components/overlay.hpp"

#include "systems/collision.hpp"
#include "systems/controls.hpp"
#include "systems/draw.hpp"
#include "systems/map.hpp"
#include "systems/garbage_collector.hpp"
#include "systems/token.hpp"
#include "systems/animation.hpp"

#include "spawners.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

MainState::MainState(Game* game) : game(game) {}

int MainState::init() {
    m_systems.add<DrawSystem>(game);
    m_systems.add<ControlSystem>(game);
    m_systems.add<CollisionSystem>(*this, *game);
    m_systems.add<MapSystem>(game);
    m_systems.add<GarbageCollectionSystem>(game);
    m_systems.add<TokenSystem>(game);
    m_systems.add<AnimationSystem>();
    m_systems.configure();

    game->player = spawn_player(m_entities);

    entityx::Entity highscore = m_entities.create();
    highscore.assign<Position>(glm::vec2(10.f, 10.f));
    highscore.assign<Highscore>();
    highscore.assign<Overlay>();
    highscore.assign<Text>("Score: 0", SDL_Color {14, 255, 14, 255});

    return 0;
}

void MainState::cash() {
    this->score += 10;
    entityx::ComponentHandle<Highscore> highscore;
    entityx::ComponentHandle<Text> text;
    auto text_entity = m_entities.create();
    text_entity.assign<GameText>("+$" + std::to_string(10), SDL_Color{14, 255, 14, 255}, true);
    text_entity.assign<Position>(glm::vec2(0, 0));
    for (entityx::Entity entity : m_entities.entities_with_components(highscore,text)) {
        (void) entity;
        text->setText("$" + std::to_string(this->get_cash()));
    }
}

int MainState::get_cash() {
    return this->score;
}

void MainState::update(double dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            game->shutdown();
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                game->shutdown();
            }
        }
    }

    m_systems.update_all(dt);
}

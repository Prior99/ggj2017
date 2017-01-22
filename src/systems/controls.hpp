#ifndef SYSTEM_CONTROLS_HPP
#define SYSTEM_CONTROLS_HPP

#include "components/position.hpp"
#include "components/player.hpp"

#include "game_config.hpp"
#include "game.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include<iostream>

class ControlSystem : public entityx::System<ControlSystem> {
    public:
        ControlSystem(Game* game): game(game) {}

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Player> player;
            entityx::ComponentHandle<Position> position;

            for (entityx::Entity entity : es.entities_with_components(player, position)) {
                (void) entity;
                const Uint8 *state = SDL_GetKeyboardState(NULL);

                if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
                    if(!player->is_jumping)
                        player->is_jumping = true;
                }
                if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
                    if(!player->is_diving)
                        player->is_diving = true;
                }
                if (player->game_over && (state[SDL_SCANCODE_RETURN] || state[SDL_SCANCODE_KP_ENTER])) {
                    game->restart();
                }
            }
        }
    private:
        Game* game;
};

#endif

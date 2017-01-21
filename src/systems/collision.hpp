#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "components/collectable.hpp"
#include "components/obstacle.hpp"
#include "game.hpp"
#include "main_state.hpp"
#include "../spawners.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <SDL2/SDL.h>

class CollisionSystem : public entityx::System<CollisionSystem> {
  private:
    MainState& state;
    Game& game;
  public:
    CollisionSystem(MainState& state, Game& game): state(state), game(game) {}

    static SDL_Rect get_rekt(
            entityx::ComponentHandle<Position> pos,
            entityx::ComponentHandle<Drawable> dwb,
            int gray_zone) {
        return SDL_Rect {
            (int)(pos->position.x - dwb->offset.x) + gray_zone,
            (int)(pos->position.y - dwb->offset.y) + gray_zone,
            (int)dwb->width - 2*gray_zone,
            (int)dwb->height - 2*gray_zone
        };
    }

    void update(entityx::EntityManager& em, entityx::EventManager& events, double dt) {
        entityx::ComponentHandle<Position> player_pos, garbage_pos, obstacle_pos;
        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Collectable> collectable;
        entityx::ComponentHandle<Player> player;
        entityx::ComponentHandle<Obstacle> obstacle;

        for (entityx::Entity player_entity : em.entities_with_components(player_pos, player, drawable)) {
            (void)player_entity;
            auto player_rect = get_rekt(player_pos, drawable, 20);
            for (entityx::Entity garbage : em.entities_with_components(garbage_pos, collectable, drawable)) {
                auto garbage_rect = get_rekt(garbage_pos, drawable, 5);
                if (SDL_HasIntersection(&player_rect, &garbage_rect)) {
                    garbage.destroy();
                    state.cash();
                }
            }

            for (entityx::Entity obstacle : em.entities_with_components(obstacle_pos, obstacle, drawable)) {
                (void)obstacle;
                auto obstacle_rect = get_rekt(obstacle_pos, drawable, 10);

                if (SDL_HasIntersection(&player_rect, &obstacle_rect)) {
                    if(!player->game_over) {
                        // Initial death vertical velocity.
                        player->vel = -20.0;
                        spawn_death_board(em);
                    }
                    player->game_over = true;

                }
            }
        }
    }
};

#endif

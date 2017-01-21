#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "components/collectable.hpp"
#include "components/wall.hpp"
#include "game.hpp"
#include "main_state.hpp"

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
            entityx::ComponentHandle<Drawable> dwb) {
        return SDL_Rect {
            (int)(pos->position.x - dwb->offset.x),
            (int)(pos->position.y - dwb->offset.y),
            (int)dwb->width,
            (int)dwb->height
        };
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Position> player_pos, garbage_pos, wall_pos;
        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Collectable> collectable;
        entityx::ComponentHandle<Player> player;
        entityx::ComponentHandle<Wall> wall;

        for (entityx::Entity player_entity : es.entities_with_components(player_pos, player, drawable)) {
            (void)player_entity;
            auto player_rect = get_rekt(player_pos, drawable);
            const int REKT = 30;
            player_rect.x += REKT;
            player_rect.y += REKT;
            player_rect.w -= REKT;
            player_rect.h -= REKT;

            for (entityx::Entity garbage : es.entities_with_components(garbage_pos, collectable, drawable)) {
                auto garbage_rect = get_rekt(garbage_pos, drawable);
                if (SDL_HasIntersection(&player_rect, &garbage_rect)) {
                    garbage.destroy();
                    state.cash();
                }
            }

            for (entityx::Entity wall : es.entities_with_components(wall_pos, wall, drawable)) {
                (void)wall;
                auto wall_rect = get_rekt(wall_pos, drawable);
                if (SDL_HasIntersection(&player_rect, &wall_rect)) {
                    player->game_over = true;
                }
            }
        }
    }
};

#endif

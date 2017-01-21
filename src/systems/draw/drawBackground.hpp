#include "game.hpp"

#include "components/position.hpp"
#include "components/text.hpp"
#include "components/overlay.hpp"

#include "systems/draw/drawEntity.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_config.hpp"
#include <iostream>

class BackgroundDrawSystem {
    public:
        BackgroundDrawSystem(Game *game) : game(game) {
            backgroundTexture = SDL_CreateTexture(
                game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

        }

        ~BackgroundDrawSystem() {
            SDL_DestroyTexture(backgroundTexture);
        }

        void draw_parallax(std::string texkey, float parallaxity_x) {
            auto position = this->game->get_player().component<Position>()->position;
            auto tex = game->res_manager().texture(texkey);
            int width, height;
            SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
            int x = (int)(position.x / parallaxity_x) % width;

            SDL_Rect dest1 {-x, -(height - HEIGHT) / 2, width, height};
            SDL_Rect dest2 {-x + width, -(height - HEIGHT) / 2, width, height};

            SDL_RenderCopy(game->renderer(), tex, nullptr, &dest1);
            SDL_RenderCopy(game->renderer(), tex, nullptr, &dest2);
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
            SDL_SetRenderTarget(game->renderer(), backgroundTexture);
            SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 0);
            SDL_RenderClear(game->renderer());

            draw_parallax("bg04", 10);
            draw_parallax("bg03", 8);
            draw_parallax("bg02", 3);
            draw_parallax("bg01", 1);

            entityx::ComponentHandle<Position> position;
            entityx::ComponentHandle<Text> text;
            entityx::ComponentHandle<Overlay> overlay;
        }

        SDL_Texture *getTexture() {
            return this->backgroundTexture;
        }

    private:
        Game *game;
        SDL_Texture *backgroundTexture;
};

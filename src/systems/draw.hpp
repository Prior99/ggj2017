#ifndef SYSTEM_DRAW_HPP
#define SYSTEM_DRAW_HPP

#include "game.hpp"

#include "systems/draw/drawEntity.hpp"
#include "systems/draw/drawOverlay.hpp"
#include "systems/draw/drawBackground.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

class DrawSystem : public entityx::System<DrawSystem> {
  public:
    DrawSystem(Game *game) : game(game), entityDrawSystem(game), backgroundDrawSystem(game), overlayDrawSystem(game) {
        gameTexture = SDL_CreateTexture(
            game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

    }

    ~DrawSystem() {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
        entityDrawSystem.update(es, events, dt);
        backgroundDrawSystem.update(es, events, dt);
        overlayDrawSystem.update(es, events, dt);
        auto src = SDL_Rect{0, 0, WIDTH, HEIGHT};
        auto dest = SDL_Rect{0, 0, WIDTH, HEIGHT};
        auto destScreen = SDL_Rect{0, 0, WIDTH, HEIGHT};
        auto renderer = game->renderer();
        SDL_SetRenderTarget(renderer, gameTexture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        auto backgroundTexture = backgroundDrawSystem.getTexture();
        SDL_RenderCopy(renderer, backgroundTexture, &src, &dest);

        auto entityTexture = entityDrawSystem.getTexture();
        SDL_SetTextureBlendMode(entityTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, entityTexture, &src, &dest);

        auto overlayTexture = overlayDrawSystem.getTexture();
        SDL_SetTextureBlendMode(overlayTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, overlayTexture, &src, &dest);

        float max = game->absolute_max;
         float min = game->absolute_min;
        float current = game->peek_amplitude();
        float bar_width = WIDTH - 40;

        // Bad rect
        SDL_Rect rect_schlecht { 20, HEIGHT - 40, (int)bar_width, 20 };
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_RenderFillRect(renderer, &rect_schlecht);

        // Good rect
        SDL_Rect rect_gut { 20 + (int)(min * bar_width), HEIGHT - 40, (int)(bar_width * (max - min)), 20 };
        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
        SDL_RenderFillRect(renderer, &rect_gut);

        // Border
        SDL_Rect rect_outer { 20, HEIGHT - 40, WIDTH - 40, 20 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect_outer);

        // Current rect
        SDL_Rect rect_inner { (int)((bar_width - 2) * current) + 21, HEIGHT - 40 , 8 , 20 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect_inner);

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, gameTexture, &src, &destScreen);
        SDL_RenderPresent(renderer);
    }

  private:
    Game *game;
    EntityDrawSystem entityDrawSystem;
    BackgroundDrawSystem backgroundDrawSystem;
    OverlayDrawSystem overlayDrawSystem;
    SDL_Texture *gameTexture;
};

#endif

#ifndef SYSTEM_DRAW_ENTITY
#define SYSTEM_DRAW_ENTITY

#include "game.hpp"

#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/uniMultiDrawable.hpp"
#include "components/overlay.hpp"
#include "components/stackedDrawable.hpp"
#include "components/block.hpp"
#include "components/game-text.hpp"
#include "components/layer.hpp"

#include "utils.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include "glm/common.hpp"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_config.hpp"
#include <iostream>

class EntityDrawSystem {
  public:
    EntityDrawSystem(Game *game) : game(game) {
        entityTexture = SDL_CreateTexture(
            game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
    }

    ~EntityDrawSystem() {
        SDL_DestroyTexture(entityTexture);
    }

    static void renderRow(Game* game, glm::vec2 startPos, SDL_Texture* texture, int reps, SDL_Rect* leftClip, SDL_Rect* centerClip, SDL_Rect* rightClip) {
        SDL_Rect leftDest{startPos.x, startPos.y, leftClip->w, leftClip->h};
        SDL_RenderCopy(game->renderer(), texture, leftClip, &leftDest);

        SDL_Rect centerDest{leftDest.x + leftDest.w, startPos.y, centerClip->w, centerClip->h};
        for(int i = 0; i < reps; i++) {
            SDL_RenderCopy(game->renderer(), texture, centerClip, &centerDest);
            centerDest.x += centerClip->w;
        }

        SDL_Rect rightDest{centerDest.x, startPos.y, rightClip->w, rightClip->h};
        SDL_RenderCopy(game->renderer(), texture, rightClip, &rightDest);
    }

    static void renderSinglePart(Game* game, entityx::Entity entity, glm::vec2 offset, double dt) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<Drawable> drawable = entity.component<Drawable>();

        auto pos = position->position - offset;
        SDL_Rect dest{pos.x, pos.y, drawable->width, drawable->height};
        SDL_Rect src{0, 0, drawable->width, drawable->height};
        SDL_Rect *clip = &src;

        SDL_Texture* texture;
        if(drawable->hasAnimation()){
            auto& animation = drawable->getAnimation();
            texture = game->res_manager().texture(animation.getTextureKey());
            clip = animation.getAnimationFrame(clip);
        }
        else
        {
            texture = game->res_manager().texture(drawable->texture_key());
        }
        SDL_RenderCopy(game->renderer(), texture, clip, &dest);

        if(DEBUG) {
            SDL_SetRenderDrawColor(game->renderer(), 255, 100, 100, 255);
            SDL_RenderFillRect(game->renderer(), &dest);
        }
    }

    static void renderStacked(Game* game, entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<StackedDrawable> drawable = entity.component<StackedDrawable>();

        auto pos = position->position - offset;
        std::vector<Drawable> layers = drawable->getLayers();

        for(Drawable layer: layers) {
            SDL_Rect dest{pos.x, pos.y, layer.width, layer.height};
            SDL_Rect src{0, 0, layer.width, layer.height};

            SDL_Texture* texture = game->res_manager().texture(layer.texture_key());
            SDL_Rect *clip = &src;
            if(layer.hasAnimation()){
                auto& animation = layer.getAnimation();
                texture = game->res_manager().texture(animation.getTextureKey());
                clip = animation.getAnimationFrame(clip);
            }
            SDL_RenderCopyEx(game->renderer(), texture, clip, &dest, layer.rotation, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
        }
    }

    static void renderBlock(Game *game, entityx::Entity entity, glm::vec2 offset, double dt) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<Drawable> drawable = entity.component<Drawable>();
        entityx::ComponentHandle<Block> block = entity.component<Block>();
        auto pos = position->position - offset;
        auto playerpos = game->player.component<Position>()->position;
        auto texture = game->res_manager().texture("water");
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        auto off = (int)(playerpos.x * 1.5) % width;
        if (off + pos.x + 50 > width) {
            off -= width;
        }
        SDL_Rect dest{(int)pos.x, (int)pos.y, drawable->width, HEIGHT - pos.y};
        SDL_Rect src{pos.x + off, pos.y, drawable->width, HEIGHT - pos.y};

        SDL_RenderCopy(game->renderer(), texture, &src, &dest);

        auto die_gischt_textur = game->res_manager().texture("wave");
        int gischt_width, gischt_height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        int animstep = ((int)playerpos.x / 50 + block->num) % 4;

        SDL_Rect dest_von_die_gischt{pos.x, pos.y - 20, 50, 36};
        SDL_Rect src_von_die_gischt{50 * animstep, 0, 50, 36};

        SDL_RenderCopy(game->renderer(), die_gischt_textur, &src_von_die_gischt, &dest_von_die_gischt);
    }

    static void renderEntity(Game* game, entityx::Entity entity, glm::vec2 offset, double dt) {
        if (entity.component<Block>()) {
            renderBlock(game, entity, offset, dt);
        } else if (entity.component<Drawable>()) {
            renderSinglePart(game, entity, offset, dt);
        } else if(entity.component<GameText>()) {
            renderText(game, entity, offset, dt);
        }
    }

    static void renderText(Game* game, entityx::Entity entity, glm::vec2 offset, double dt) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<GameText> text = entity.component<GameText>();
        bool fast = text->isFast();
        auto player = game->player;
        auto pp = player.component<Position>()->position;
        auto pos =  (fast ? pp - glm::vec2(150, 100) + position->position: position->position) - offset;
        position->position.y -= 1;
        float progress = text->getTime() / TEXT_DURATION;
        auto surf = TTF_RenderText_Blended(game->res_manager().font("font-small"), text->getText().c_str(), text->getColor());
        if (!fast) {
            SDL_LockSurface(surf);
            SDL_PixelFormat* fmt = surf->format;
            unsigned bpp = fmt->BytesPerPixel;
            for (int y = 0; y < surf->h; y++)
                for (int x = 0; x < surf->w; x++) {
                    Uint32* pixel_ptr = (Uint32 *)((Uint8 *)surf->pixels + y * surf->pitch + x * bpp);
                    Uint8 r, g, b, a;
                    SDL_GetRGBA( *pixel_ptr, fmt, &r, &g, &b, &a );
                    *pixel_ptr = SDL_MapRGBA( fmt, r, g, b, (int)(a * (0.5f + 0.5f * (1.0 - progress))));
                }

            SDL_UnlockSurface(surf);
        }
        auto texture = SDL_CreateTextureFromSurface(game->renderer(), surf);
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        auto progressSize = glm::min(progress * 2, 1.0f);
        int nW = w * progressSize;
        int offsetX = pos.x + (w - nW) / 2;
        // std::cout << "rendertext" << (pos.x + offsetX) << "," << (pos.y - progress * 100) << std::endl;
        SDL_Rect dest{pos.x + offsetX, pos.y - progress * 100, nW, h * progressSize};

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(game->renderer(), texture, NULL, &dest);
        SDL_FreeSurface(surf);
        text->updateTime(dt);
        if (text->getTime() > TEXT_DURATION) {
            entity.destroy();
        }
    }

    template<int L> void render_layer(
            entityx::EntityManager& em,
            entityx::EventManager& events,
            entityx::TimeDelta dt,
            glm::vec2 offset)
    {
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Layer<L>> layer;
        for (entityx::Entity entity : em.entities_with_components(position, layer)) {
            if(!entity.component<Overlay>()) {
                auto privOffset = glm::vec2(0, 0);
                auto draw = entity.component<Drawable>();
                if (draw) {
                    privOffset = draw->offset;
                }
                renderEntity(game, entity, offset + privOffset, dt);
            }
        }
    }

    void update(
            entityx::EntityManager& em,
            entityx::EventManager &events,
            entityx::TimeDelta dt)
    {
        auto player = game->player;
        if(!player.valid())
            return;
        auto position = player.component<Position>();
        glm::vec2 offset = glm::vec2(position->position.x - PLAYER_OFFSET, 0.0);

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(game->renderer(), entityTexture);
        SDL_SetRenderDrawColor(game->renderer(), 128, 128, 255, 0);
        SDL_RenderClear(game->renderer());

        render_layer<1>(em, events, dt, offset);
        render_layer<2>(em, events, dt, offset);
        render_layer<3>(em, events, dt, offset);
    }

    SDL_Texture *getTexture() {
        return this->entityTexture;
    }

  private:
    Game *game;
    SDL_Texture *entityTexture;
};

#endif

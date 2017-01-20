#ifndef SYSTEM_DRAW_ENTITY
#define SYSTEM_DRAW_ENTITY

#include "game.hpp"

#include "components/drawable.hpp"
#include "components/multipartDrawable.hpp"
#include "components/position.hpp"
#include "components/uniMultiDrawable.hpp"
#include "components/overlay.hpp"
#include "components/stackedDrawable.hpp"

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

    static void renderMultiPart(Game* game, entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<MultipartDrawable> multipartDrawable = entity.component<MultipartDrawable>();

        auto pos = position->position() - offset;
        SDL_Rect dest{pos.x, pos.y, multipartDrawable->getWidth(), 0};
        SDL_Rect src{0, 0, multipartDrawable->getWidth(), 0};

        PartialDrawable top = multipartDrawable->getTop();
        if(top.animation.initialized()) {
            top.animation.getAnimationFrame(&src);
        } else {
            src = {0, 0, multipartDrawable->getWidth(), top.height};
        }
        dest.h = top.height;
        SDL_RenderCopy(game->renderer(), game->res_manager().texture(top.texture), &src, &dest);

        dest.y += top.height;
        PartialDrawable middle = multipartDrawable->getMiddle();
        if(middle.animation.initialized()) {
            middle.animation.getAnimationFrame(&src);
        } else {
            src = {0, 0, multipartDrawable->getWidth(), middle.height};
        }
        dest.h = middle.height;
        for(int i = 0; i < (int) multipartDrawable->getRepeat(); i++) {
            SDL_RenderCopy(game->renderer(), game->res_manager().texture(middle.texture), &src, &dest);
            dest.y += middle.height;
        }
        if(compareDoubles(multipartDrawable->getRepeat(), (int) multipartDrawable->getRepeat()) != 0) {
            auto fractRepeat = multipartDrawable->getRepeat() - (int) multipartDrawable->getRepeat();
            src.h = fractRepeat * middle.height;
            dest.h = src.h;
            SDL_RenderCopy(game->renderer(), game->res_manager().texture(middle.texture), &src, &dest);
            dest.y += src.h;
        }
        PartialDrawable bottom = multipartDrawable->getBottom();
        if(bottom.animation.initialized()) {
            bottom.animation.getAnimationFrame(&src);
        } else {
            src = {0, 0, multipartDrawable->getWidth(), bottom.height};
        }
        dest.h = bottom.height;
        SDL_RenderCopy(game->renderer(), game->res_manager().texture(bottom.texture), &src, &dest);
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

    static void renderUniMultiPart(Game* game, entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<UniMultipartDrawable> drawable = entity.component<UniMultipartDrawable>();

        auto texture = game->res_manager().texture(drawable->getTexture());
        glm::vec2 reps = drawable->getRepititions();

        auto pos = position->position() - offset;

        SDL_Rect leftClip, rightClip, centerClip;

        renderRow(game, pos, texture, reps.x, drawable->getLeftTopClip(&leftClip), drawable->getCenterTopClip(&centerClip), drawable->getRightTopClip(&rightClip));

        for(int row = 0; row < reps.y; row++) {
            auto newPos = pos + glm::vec2(0, drawable->getTop() + row * drawable->getCenterY());
            renderRow(game, newPos, texture, reps.x, drawable->getLeftCenterClip(&leftClip), drawable->getCenterClip(&centerClip), drawable->getRightCenterClip(&rightClip));
        }

        auto bottomPos = pos + glm::vec2(0, drawable->getTop() + reps.y * drawable->getCenterY());
        renderRow(game, bottomPos, texture, reps.x, drawable->getLeftBottomClip(&leftClip), drawable->getCenterBottomClip(&centerClip), drawable->getRightBottomClip(&rightClip));
    }

    static void renderSinglePart(Game* game, entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<Drawable> drawable = entity.component<Drawable>();

        auto pos = position->position() - offset;
        SDL_Rect dest{pos.x, pos.y, drawable->getWidth(), drawable->getHeight()};
        SDL_Rect src{0, 0, drawable->getWidth(), drawable->getHeight()};

        SDL_Texture* texture = game->res_manager().texture(drawable->texture_key());
        SDL_Rect *clip = &src;
        if(drawable->hasAnimation()){
            auto& animation = drawable->getAnimation();
            //if(animation.initialized() && animation.isRunning()) {
                texture = game->res_manager().texture(animation.getTextureKey());
                clip = animation.getAnimationFrame(clip);
            //}
        }
        SDL_RenderCopy(game->renderer(), texture, clip, &dest);
    }

    static void renderStacked(Game* game, entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<StackedDrawable> drawable = entity.component<StackedDrawable>();

        auto pos = position->position() - offset;
        std::vector<Drawable> layers = drawable->getLayers();

        for(Drawable layer: layers) {
            SDL_Rect dest{pos.x, pos.y, layer.getWidth(), layer.getHeight()};
            SDL_Rect src{0, 0, layer.getWidth(), layer.getHeight()};

            SDL_Texture* texture = game->res_manager().texture(layer.texture_key());
            SDL_Rect *clip = &src;
            if(layer.hasAnimation()){
                auto& animation = layer.getAnimation();
                texture = game->res_manager().texture(animation.getTextureKey());
                clip = animation.getAnimationFrame(clip);
            }
            SDL_RenderCopyEx(game->renderer(), texture, clip, &dest, layer.getRotation(), NULL, SDL_RendererFlip::SDL_FLIP_NONE);
        }
    }

    static void renderEntity(Game* game, entityx::Entity entity, glm::vec2 offset, double dt) {
        if(entity.component<MultipartDrawable>()) {
            renderMultiPart(game, entity, offset);
        } else if (entity.component<Drawable>()) {
            renderSinglePart(game, entity, offset);
        } else if (entity.component<UniMultipartDrawable>()) {
            renderUniMultiPart(game, entity, offset);
        } else if(entity.component<StackedDrawable>()) {
            renderStacked(game, entity, offset);
        }
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {
        auto player = game->get_player();
        auto position = player.component<Position>();
        glm::vec2 offset = glm::vec2(position->get_x(), 0.0);

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(game->renderer(), entityTexture);
        SDL_SetRenderDrawColor(game->renderer(), 128, 128, 255, 0);
        SDL_RenderClear(game->renderer());

        std::cout << "sdkasldkajsldkja" << std::endl;
        for (entityx::Entity entity : es.entities_with_components(position)) {
            if(!entity.component<Overlay>()) {
                auto privOffset = glm::vec2(0, 0);
                auto draw = entity.component<Drawable>();
                if (draw) {
                    privOffset = draw->getOffset();
                }
                std::cout << (offset + privOffset).x << std::endl;
                renderEntity(game, entity, offset + privOffset, dt);
            }
        }
        std::cout << "sdkasldkajsldkjaalskdjalskdj" << std::endl;
    }

    SDL_Texture *getTexture() {
        return this->entityTexture;
    }

  private:
    Game *game;
    SDL_Texture *entityTexture;
};

#endif

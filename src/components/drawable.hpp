#ifndef COMPONENT_DRAWABLE_HPP
#define COMPONENT_DRAWABLE_HPP

#include "entityx/entityx.h"

#include "animation.hpp"

#include <SDL2/SDL.h>

class Drawable : entityx::Component<Drawable> {
private:
    std::string textureKey;
    AnimationCollection animation;

public:
    float width;
    float height;
    double rotation;
    glm::vec2 offset;

    Drawable(
            std::string key,
            float width,
            float height,
            AnimationCollection animation = { "" },
            glm::vec2 offset = glm::vec2(0, 0)):
        textureKey(key),
        animation(animation),
        width(width),
        height(height),
        rotation(0.0),
        offset(offset) {}

    std::string texture_key() {
        return textureKey;
    }

    bool hasAnimation() {
        return !animation.getTextureKey().empty();
    }

    AnimationCollection& getAnimation() {
        return animation;
    }
};
#endif

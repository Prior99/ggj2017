#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>

#include "game.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"

#include "components/position.hpp"
#include "components/drawable.hpp"
#include "components/block.hpp"

#include "../events.hpp"
#include "../spawners.hpp"

#define BLOCK_SIZE 50
#define MAX_CHANGE 15

class MapSystem : public entityx::System<MapSystem>,
                  public entityx::Receiver<MapSystem> {
private:
    Game *game;
    entityx::TimeDelta local_dt;
    float last_block_spawned_at;
    float last_wall_spawned_at = 1700.0;
    float random_wall_offset = 0.0;
    float last_height = HEIGHT / 2.0f;
    float WAVE_GENERATOR_X = WIDTH * 0.4;
public:
    MapSystem(Game *game): game(game), local_dt(0), last_block_spawned_at(0.0) {}
    void configure(entityx::EntityManager& entities, entityx::EventManager& events) override {
        events.subscribe<CollisionEvent>(*this);
    }

    void wave_it(entityx::EntityManager& entities) {
        auto player = game->get_player();
        auto player_position = player.component<Position>();
        auto mid_position = player_position->position.x + WAVE_GENERATOR_X;
        entityx::ComponentHandle<Block> block;
        entityx::ComponentHandle<Position> position;
        float band = HEIGHT * MAX_BLOCK_HEIGHT;
        float lower_offset = (HEIGHT - band) / 2.0f;
        float amplitude = this->game->take_amplitude();
        float inverted_height = lower_offset + amplitude * band;
        float height = HEIGHT - inverted_height;
        height = glm::clamp(height, last_height - MAX_CHANGE, last_height + MAX_CHANGE);
        this->last_height = height;
        for (auto entity: entities.entities_with_components(block, position)) {
            (void) entity;
            auto x = position->position.x;
            if (x > mid_position) {
                float amplitude = height - HEIGHT / 2;
                float distance_to_mid = x - mid_position;
                float max_distance = WIDTH / 2;
                float pi = glm::pi<float>();
                float y = glm::sin((distance_to_mid / max_distance) * pi - pi) * amplitude + height;
                position->position.y = y;
            }
        }
    }

    static float mix(float l, float r, float p, float l_v, float r_v) {
        float dist = r - l;
        float fac_left = (r - p) / dist;
        float fac_right = 1.0 - fac_left;
        return fac_left * l_v + fac_right * r_v;
    }

    void move_it(entityx::EntityManager& entities) {
        auto player = game->get_player();
        auto player_position = player.component<Position>();
        auto player_x = player_position->position.x;

        entityx::Entity closest[3];
        float distances[3];
        for(int i=0; i<3; i++)
            distances[i] = 1.0/0.0;

        entityx::ComponentHandle<Block> block;
        entityx::ComponentHandle<Position> position;

        // Find the left and right blocks closest to the player.
        for (auto entity: entities.entities_with_components(block, position))
        {
            auto x = position->position.x;
            float dist = glm::abs(x-player_x);
            for(int i=0; i<3; i++)
            {
                if(dist < distances[i])
                {
                    for(int j=1; j>=i; j--)
                    {
                        closest[j+1] = closest[j];
                        distances[j+1] = distances[j];
                    }

                    closest[i] = entity;
                    distances[i] = dist;
                    break;
                }
            }
        }

        entityx::Entity left;
        float left_pos = 1.0/0.0;
        entityx::Entity center;
        entityx::Entity right;
        float right_pos = -1.0/0.0;

        for(int i=0; i<3; i++)
        {
            float pos = closest[i].component<Position>()->position.x;
            if(pos < left_pos)
            {
                left = closest[i];
                left_pos = pos;
            }
        }
        for(int i=0; i<3; i++)
        {
            float pos = closest[i].component<Position>()->position.x;
            if(pos > right_pos)
            {
                right = closest[i];
                right_pos = pos;
            }
        }

        for(int i=0; i<3; i++)
        {
            if(closest[i] != left && closest[i] != right)
            {
                center = closest[i];
                break;
            }
        }

        auto left_position = left.component<Position>();
        auto center_position = center.component<Position>();
        auto right_position = right.component<Position>();

        float l_border = (left_position->position.x + center_position->position.x) / 2.0;
        float r_border = l_border + BLOCK_WIDTH;
        float l_val = (left_position->position.y + center_position->position.y) / 2.0;
        float r_val = (right_position->position.y + center_position->position.y) / 2.0;

        float p_height = mix(l_border, r_border, player_x, l_val, r_val);
        player_position->position.y = p_height;
    }

	void update(entityx::EntityManager& entities, entityx::EventManager &events, entityx::TimeDelta dt) {
        auto player = game->get_player();
        auto player_comp = player.component<Player>();
        auto position = player.component<Position>();

        if(player_comp->game_over) {
            player_comp->vel += glm::vec2(0.0,-1.0);
            position->position += player_comp->vel;
        }

        float new_player_x = position->position.x + PLAYER_SPEED;
        position->position = glm::vec2(new_player_x, position->position.y);

        while(new_player_x + BLOCK_SPAWN_OFFSET > last_block_spawned_at) {
            last_block_spawned_at += BLOCK_WIDTH;
            spawn_block(entities, last_block_spawned_at);
        }

        if(new_player_x > last_wall_spawned_at + WALL_SPAWN_OFFSET + random_wall_offset)
        {
            last_wall_spawned_at = new_player_x + BLOCK_SPAWN_OFFSET;
            random_wall_offset = (float)(rand() % RAND_WALL_BAND);
            spawn_wall(entities, last_wall_spawned_at);
        }

        this->wave_it(entities);
        this->move_it(entities);
    }

    void receive(const CollisionEvent& ce) {
        if(ce.m_first.has_component<Player>()) {
        }
    }
};

#endif

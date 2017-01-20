#ifndef GAME_HPP
#define GAME_HPP

#include "components/position.hpp"

#include "strapon/state/state.hpp"
#include "strapon/resource_manager/resource_manager.hpp"
#include "glm/common.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>
#include <vector>

#include <stack>
#include <string>
#include <pulse/simple.h>
#include <pulse/error.h>

class Game {
  public:
    ~Game();

    int init();
    void mainloop();
    bool is_running();
    void shutdown();
    void popstate();

    void set_player(entityx::Entity player);
    entityx::Entity get_player(void);
    bool init_audio();
    void tick_audio();
    float take_amplitude();

    std::stack<std::pair<std::string, std::unique_ptr<State>>> &states();
    const std::string &statename() const;

    SDL_Renderer *renderer();
    ResourceManager &res_manager();
    SDL_Window *window();
    const SDL_Rect &world_size() const;

  private:
    bool m_running = true;
    int m_last_frame_time = 0;
    float *data;
    float absolute_max = 0.0f;
    float absolute_min = 0.0f;
    std::vector<float> amplitudes;
    SDL_Rect m_world_size = {0, 0, 1600, 1200};

    SDL_Renderer *m_render;
    SDL_Window *m_window;
    pa_simple *pa;
    std::stack<std::pair<std::string, std::unique_ptr<State>>> m_states;
    entityx::EntityX m_ex;
    ResourceManager m_res_manager;
    entityx::Entity player;
};

#endif /* end of include guard: GAME_HPP */

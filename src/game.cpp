#include "game.hpp"

#include "main_state.hpp"
#include "game_config.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <iostream>
#include <pulse/simple.h>
#include <pulse/error.h>

Game::~Game() {
    m_res_manager.shutdown();
    TTF_Quit();
    SDL_CloseAudio();
    SDL_DestroyRenderer(m_render);
    SDL_DestroyWindow(m_window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::init_audio() {
    pa_sample_spec ss;
    this->data = new float[2048];

    ss.format = PA_SAMPLE_FLOAT32;
    ss.rate = 44100;
    ss.channels = 1;
    int error;
    this->pa = pa_simple_new(NULL, "GGJ2017", PA_STREAM_RECORD, NULL,  "record", &ss, NULL, NULL, &error);
    if (!this->pa) {
        std::cout << pa_strerror(error) << std::endl;
        return false;
    }
    return true;
}

int Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!init_audio()) {
        return 1;
    }

    m_window = SDL_CreateWindow("Innojam #10", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    m_render =
        SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_render == nullptr) {
        SDL_DestroyWindow(m_window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError()
                  << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    m_res_manager.load_texture("gradient", "res/gradient.png", m_render);
    m_res_manager.load_texture("mock_block", "res/mock-block.png", m_render);
    m_res_manager.load_font("font20", "res/DejaVuSans.ttf", 20);

    SDL_RenderSetLogicalSize(m_render, WIDTH, HEIGHT);

    this->player = m_ex.entities.create();

    m_states.push({"main", std::make_unique<MainState>(this)});
    m_states.top().second->init();

    return 0;
}

entityx::Entity Game::getPlayer() {
    return this->player;
}

void Game::tick_audio() {
    int len = 1600;
    int error;
    pa_simple_read(this->pa, this->data, len * 4, &error);

    float avg = 0;
    for (int i = 0; i < len; ++i) {
        avg += glm::max(0.0f, this->data[i]) / len;
    }

    this->absolute_max = glm::max(this->absolute_max, avg);
    this->absolute_min = glm::min(this->absolute_min, avg);
    this->current_amplitude = (avg / this->absolute_max) - this->absolute_min;
}

void Game::mainloop() {
    int current_time = SDL_GetTicks();
    double dt = (current_time - m_last_frame_time) / 1000.0;
    m_last_frame_time = current_time;
    this->tick_audio();
    m_states.top().second->update(dt);
}

SDL_Renderer *Game::renderer() {
    return m_render;
}

SDL_Window *Game::window() {
    return m_window;
}

ResourceManager &Game::res_manager() {
    return m_res_manager;
}

std::stack<std::pair<std::string, std::unique_ptr<State>>> &Game::states() {
    return m_states;
}

bool Game::is_running() {
    return m_running;
}

void Game::shutdown() {
    m_running = false;
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
}

void Game::popstate() {
    m_states.pop();
}

const std::string &Game::statename() const {
    return m_states.top().first;
}

const SDL_Rect &Game::world_size() const {
    return m_world_size;
}

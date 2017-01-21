#include "game.hpp"

#include "main_state.hpp"
#include "game_config.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <iostream>
#include <pulse/simple.h>
#include <pulse/error.h>

#define buffer_size 1600

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
    this->data = new float[buffer_size];

    pa_sample_spec sample_spec;
    sample_spec.format = PA_SAMPLE_FLOAT32;
    sample_spec.rate = 48000;
    sample_spec.channels = 1;

    pa_buffer_attr buffer_attr;
    buffer_attr.maxlength = buffer_size * 4;
    buffer_attr.fragsize = buffer_size * 4;

    int error;
    this->pa = pa_simple_new(NULL, "GGJ2017", PA_STREAM_RECORD, NULL,  "record", &sample_spec, NULL, &buffer_attr, &error);
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
    m_res_manager.load_texture("block", "res/gj-block.png", m_render);
    m_res_manager.load_texture("water", "res/water.png", m_render);
    m_res_manager.load_texture("trash", "res/mock-trash.png", m_render);
    m_res_manager.load_texture("jonny", "res/jonny8frames.png", m_render);
    m_res_manager.load_texture("bg01", "res/bg/01.png", m_render);
    m_res_manager.load_texture("bg02", "res/bg/02.png", m_render);
    m_res_manager.load_texture("bg03", "res/bg/03.png", m_render);
    m_res_manager.load_texture("bg04", "res/bg/04.png", m_render);
    m_res_manager.load_font("font20", "res/DejaVuSans.ttf", 20);

    SDL_RenderSetLogicalSize(m_render, WIDTH, HEIGHT);

    m_states.push({"main", std::make_unique<MainState>(this)});
    m_states.top().second->init();

    return 0;
}

void Game::set_player(entityx::Entity player) {
    this->player = player;
}

entityx::Entity Game::get_player() {
    return this->player;
}

void Game::tick_audio() {
    int len = buffer_size;
    int error;
    pa_simple_read(this->pa, this->data, len * 4, &error);
    pa_simple_flush(this->pa, &error);
    // std::cout << "Latency " << (int)pa_simple_get_latency(this->pa, &error) << std::endl;

    float avg = 0;
    for (int i = 0; i < len; ++i) {
        avg += glm::max(0.0f, this->data[i]) / len;
    }

    this->absolute_max = glm::max(this->absolute_max, avg);
    this->absolute_min = glm::min(this->absolute_min, avg);
    this->amplitudes.push_back((avg / this->absolute_max) - this->absolute_min);
}

float Game::take_amplitude() {
    float max;
    for (std::vector<float>::iterator it = this->amplitudes.begin(); it != this->amplitudes.end(); ++it) {
        max = glm::max(max, *it);
    }
    this->amplitudes.clear();
    return max;
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

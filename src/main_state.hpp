#ifndef MAIN_STATE_MAIN_HPP
#define MAIN_STATE_MAIN_HPP

#include "game.hpp"
#include "strapon/state/state.hpp"
#include "entityx/entityx.h"

class MainState : public State {
private:
    int score = 0;
    Game *game;
    entityx::EventManager m_events;
    entityx::EntityManager m_entities{m_events};
    entityx::SystemManager m_systems{m_entities, m_events};

public:
    MainState(Game *game);
    virtual ~MainState(void) {}
    int init(void) override;
    void update(double dt) override;
    void cash(void);
    int get_cash(void);
};

#endif

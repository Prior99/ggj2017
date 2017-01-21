#ifndef COMPONENT_DECAYSADASDASLDKASLDK
#define COMPONENT_DECAYSADASDASLDKASLDK

#include "entityx/entityx.h"
#include <iostream>

class Decay : entityx::Component<Decay> {
    public:
        float dt;
        float timeout;
        Decay() : dt(0.0), timeout(3.0) {}
        Decay(float t) : dt(0.0), timeout(t) {}
};

#endif

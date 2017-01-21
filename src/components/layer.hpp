#ifndef COMPONENT_OMGITWORKSONEONEELEVEN
#define COMPONENT_OMGITWORKSONEONEELEVEN

#include "entityx/entityx.h"
#include <iostream>

template<int L> class Layer : entityx::Component<Layer<L>> {};

#endif

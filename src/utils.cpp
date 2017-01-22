
#include "utils.hpp"
#include <cstdlib>
#include <limits>

int compareDoubles (double a, double b){
    if (abs(a - b) < std::numeric_limits<double>::epsilon()){
        return 0;
    }
    else if (a < b){
        return -1;
    }
    else{
        return 1;
    }
}

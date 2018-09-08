#include <random>

namespace Labyrinth {

    #if defined(LF_64)
        using Random = std::mt19937_64;
    #else
        using Random = std::mt19937;
    #endif

}
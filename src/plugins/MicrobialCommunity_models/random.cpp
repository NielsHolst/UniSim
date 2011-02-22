#include "random.h"

namespace MicrobialCommunity {

    Random::Random(){
        srand((unsigned)time(NULL));
    }

    int Random::getInt(int min, int max) {
        return min + rand() % (max - min + 1);
    }

    double Random::getDouble(double min, double max) {
        return min + (max - min) * double(rand()) / double(RAND_MAX);
    }

}

#include <iostream>
#include <cstdint>
using namespace std;

// L_1 : p(x) = x^30 + x^6 + x^4 + x + 1
// L_2 : p(x) = x^31 + x^3 + 1
// L_3 : p(x) = x^32 + x^7 + x^5 + x^3 + x^2 + x + 1
// alpha = 0.01

static const uint32_t start_state = (1 << 16) | (1 << 8) | 1;

uint32_t L_1() {

    uint32_t lsfr = start_state;
    uint32_t bit;
    uint32_t period = 0;

    do {
        bit  = ((lsfr >> 2) ^ (lsfr >> 26) ^ (lsfr >> 28) ^ (lsfr >> 31)) & 1ULL;
        lsfr = (lsfr >> 1) | (bit << 63);
        ++period;
    } while (lsfr != start_state);

    return period;

}

uint32_t L_2() {

    uint32_t lsfr = start_state;
    uint32_t bit;
    uint32_t period = 0;

    do {
        bit  = ((lsfr >> 1) ^ (lsfr >> 29)) & 1ULL;
        lsfr = (lsfr >> 1) | (bit << 63);
        ++period;
    } while (lsfr != start_state);

    return period;

}

uint32_t L_3() {

    uint32_t lsfr = start_state;
    uint32_t bit;
    uint32_t period = 0;

    do {
        bit  = ((lsfr >> 0) ^ (lsfr >> 25) ^ (lsfr >> 27) ^ (lsfr >> 29) ^ (lsfr >> 30) ^ (lsfr >> 31)) & 1ULL;
        lsfr = (lsfr >> 1) | (bit << 63);
        ++period;
    } while (lsfr != start_state);

    return period;

}


int main()
{

    return 0;
}

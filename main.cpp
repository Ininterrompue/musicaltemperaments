#include "temperament.hpp"

int main(int argc, char *argv[]) {
    Temperament t0(440, 12, "E-flat", 4);
    t0.display_universal_info();
    
    t0.young1();
    t0.calculate_frequencies();
    t0.calculate_cents_bps();
    t0.display_temperament_table();
    
    return 0;
}
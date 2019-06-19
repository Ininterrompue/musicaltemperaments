#include "tuning.hpp"
#include <iostream>
#include <map>


int main(int argc, char *argv[]) {
    double concertA4; int octave;

    if (argc == 3) {
        concertA4 = atof(argv[1]);
        octave = atoi(argv[2]);
    }
    else if (argc == 1) {
        concertA4 = 440;
        octave = 4;
    }
    else {
        std::cout << "./main [A4] [octave] temperament." << std::endl;
        std::cout << "Default is A4 = 440 Hz, 4th octave." << std::endl;
        return 1;
    }

    Temperament t0(concertA4, 12, "A", octave);
    t0.meantone4();

    t0.display_universal_info();
    t0.calculate_frequencies();
    t0.calculate_cents_bps();
    t0.display_tuning_table();
    
    return 0;
}
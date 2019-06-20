#include "tuning.hpp"
#include <iostream>
#include <map>


int main(int argc, char *argv[]) {
    double concertA4; int octave; std::string starting_note;

    if (argc == 4) {
        concertA4 = atof(argv[1]);
        octave = atoi(argv[2]);
        starting_note = argv[3];
    }
    else if (argc == 1) {
        concertA4 = 440;
        octave = 4;
        starting_note = "E-flat";
    }
    else {
        std::cout << "./main [A4] [octave] [starting note]." << std::endl;
        std::cout << "Default is A4 = 440 Hz, 4th octave, starting note E-flat." 
                  << std::endl;
        return 1;
    }

    Temperament t0(concertA4, 12, starting_note, octave);
    t0.meantone(1./12, Constants::pythagorean_comma_);

    t0.display_universal_info();
    t0.calculate_frequencies();
    t0.calculate_cents_bps();
    t0.display_tuning_table();
    
    return 0;
}
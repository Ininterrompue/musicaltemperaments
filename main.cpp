#include "tuning.hpp"
#include <iostream>
#include <map>


int main(int argc, char *argv[]) {
    double concertA4; 
    std::string starting_note; 
    int octave;

    if (argc == 4) {
        concertA4 = atof(argv[1]);
        starting_note = argv[2];
        octave = atoi(argv[3]);
    }
    else if (argc == 1) {
        concertA4 = 440;
        starting_note = "E-flat";
        octave = 4;
    }
    else {
        std::cout << "./main [A4] [starting note] [octave]." << std::endl;
        std::cout << "Default is A4 = 440 Hz, starting note E-flat, 4th octave." 
                  << std::endl;
        return 1;
    }

    // Temperament t0(concertA4, starting_note, octave);
    // t0.meantone(1./12, Constants::pythagorean_comma_);

    EqualTemperament t0(concertA4, starting_note, octave);
    t0.equaln(55);

    t0.display_universal_info();
    t0.calculate_frequencies();
    t0.calculate_cents_bps();
    t0.display_tuning_table();
    
    return 0;
}
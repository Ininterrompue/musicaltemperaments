#include <iostream>
#include <cmath>
#include "temperament.hpp"

int main(int argc, char *argv[]) {
    Temperament t0(440, 12, "E-flat", 4);

    t0.meantone6();
    t0.calculate_frequencies();

    cout << "A4 = " << t0.get_concertA4() << " Hz, " 
         << t0.get_ntones() << " tones per octave, " 
         << "octave " << t0.get_octave() << "." << endl;
    for (auto &i : t0.frequencies_)
        cout << i << endl;
    return 0;
}
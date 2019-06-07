#include <iostream>
#include <cmath>
#include "temperament.hpp"

int main(int argc, char *argv[]) {
    Temperament t0;
    Constants C;
    cout << t0.get_concertA4() << " Hz, " 
         << t0.get_ntones() << " tones, " 
         << t0.get_based() << ", "
         << t0.get_octave() << "th octave" << endl;

    cout << "E4 = " 
         << t0.get_concertA4() * pow(0.5, 1) * 3.0/2.0 / pow(C.pythagorean_comma_, 1.0/12.0) 
         << " Hz" << endl;

    cout << "A-flat4 = " 
         << t0.get_concertA4() * pow(0.5, 3) * pow(3.0/2.0 / pow(C.pythagorean_comma_, 1.0/12.0), 5)
         << " Hz" << endl;
    return 0;
}
#include <iostream>
#include <stdio.h>
#include <cmath>
#include "temperament.hpp"

int main(int argc, char *argv[]) {
    Temperament t0(440, 12, "E-flat", 4);

    cout << "A4 = " << t0.get_concertA4() << " Hz, " 
         << t0.get_ntones() << " tones per octave, " 
         << "octave " << t0.get_octave() << "." << endl;
    
    t0.vallotti();
    t0.calculate_frequencies();
    t0.calculate_cents_bps();
    
    printf("PC\tHz\tP5c\tM3c\tm3c\tP5b\tM3b\tm3b\n");
    for (int i = 0; i < t0.get_ntones(); i++)
        printf("%d\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\n",
            i, t0.frequencies_[i], 
            t0.centsP5_[i], t0.centsM3_[i], t0.centsm3_[i],
            t0.bpsP5_[i],   t0.bpsM3_[i],   t0.bpsm3_[i]);
    return 0;
}
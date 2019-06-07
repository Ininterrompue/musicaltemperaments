#include "temperament.hpp"
#include <cmath>
#include <iostream>

using namespace std;


Constants::Constants() :
    pythagorean_comma_(531441.0 / 524288.0), 
    syntonic_comma_   (    81.0 /     80.0),
    lesser_diesis_    (   128.0 /    125.0),
    greater_diesis_   (   648.0 /    625.0),
    schisma_          ( 32805.0 /  32768.0) { 
}

double Constants::convert_to_cents(double comma) {
    return 1200.0 * log2(comma);
}


Temperament::Temperament(double concertA4, int ntones, string based, int octave) {
    concertA4_ = concertA4;
    ntones_    = ntones;
    based_     = based;
    octave_    = octave;
}

double Temperament::get_concertA4() const {
    return concertA4_;
}

int Temperament::get_ntones() const {
    return ntones_;
}

string Temperament::get_based() const {
    return based_ + "-based";
}

int Temperament::get_octave() const {
    return octave_;
}

void Temperament::equal_temperament() {
    for (int i = 0; i < ntones_; i++)
        temperedP5_.push_back(-1.0/ntones_);
}

void Temperament::calculate_frequencies() {
    for (int i = 0; i < ntones_; i++) {
        
    }
}
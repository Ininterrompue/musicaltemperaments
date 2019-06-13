#include "temperament.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

Constants::Constants(double pythagorean_comma, double syntonic_comma, double schisma) {
    pythagorean_comma_ = pythagorean_comma;
    syntonic_comma_    = syntonic_comma;
    schisma_           = schisma;
}

double Constants::convert_to_cents(double comma) {
    return 1200.0 * log2(comma);
}

Constants C;

Temperament::Temperament(double concertA4, int ntones, string starting_note, int octave) {
    concertA4_     = concertA4;
    ntones_        = ntones;
    starting_note_ = starting_note;
    octave_        = octave;
}

double Temperament::get_concertA4() const {
    return concertA4_;
}

int Temperament::get_ntones() const {
    return ntones_;
}

string Temperament::get_starting_note() const {
    return starting_note_;
}

int Temperament::get_octave() const {
    return octave_;
}

/*
Creates a dictionary corresponding to integer notation.
Sorts vector<int> pitchclass_ starting from the starting_note and progresses
by perfect fifths upwards. vector<int> pitchclass_ has ntones_ elements.
 */
void Temperament::pitchclass_array() {
    map<string, int> pitchclass_dict = {
        {"C-flat", 11}, {"C", 0},  {"C-sharp", 1},
        {"D-flat", 1},  {"D", 2},  {"D-sharp", 3},
        {"E-flat", 3},  {"E", 4},  {"E-sharp", 5},
        {"F-flat", 4},  {"F", 5},  {"F-sharp", 6},
        {"G-flat", 6},  {"G", 7},  {"G-sharp", 8},
        {"A-flat", 8},  {"A", 9},  {"A-sharp", 10},
        {"B-flat", 10}, {"B", 11}, {"B-sharp", 0}
    };
    int starting_note_int = pitchclass_dict[starting_note_];
    for (int i = 0; i < ntones_; i++)
        pitchclass_.push_back( (7*i + starting_note_int) % ntones_ );
    vector<int>::iterator it = find(pitchclass_.begin(), pitchclass_.end(), 9);
    A4_starting_position_ = distance(pitchclass_.begin(), it);
}

void Temperament::equal() {
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1.0/ntones_);
        temperedcommas_.push_back(C.pythagorean_comma_);
    }
}

void Temperament::pythagorean() {
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(0.0);
        temperedcommas_.push_back(0.0);
    }
}

void Temperament::meantone6() {
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1.0/6.0);
        temperedcommas_.push_back(C.syntonic_comma_);
    }
}

void Temperament::kirnberger3() {
    starting_note_ = "C";
    temperedfractions_ = {0.25, 0.25, 0.25, 0.25, 0, 0, 1, 0, 0, 0, 0};
    temperedcommas_ = {
        C.syntonic_comma_, C.syntonic_comma_, 
        C.syntonic_comma_, C.syntonic_comma_,
        0, 0, C.schisma_, 0, 0, 0, 0
    };
}

void Temperament::calculate_frequencies() {
    pitchclass_array();
    for (int i = 0; i < ntones_; i++)
        frequencies_.push_back(0.0);
    frequencies_[9] = concertA4_;

    // A -> E -> ...
    for (unsigned int i = A4_starting_position_ + 1; i < pitchclass_.size(); i++) {
        frequencies_[pitchclass_[i]] = 3.0/2.0 * frequencies_[pitchclass_[i-1]] /
            pow(temperedcommas_[i-1], temperedfractions_[i-1]);
        if (pitchclass_[i] - pitchclass_[i-1] < 0)
            frequencies_[pitchclass_[i]] *= 0.5;
    }

    // ... <- D <- A
    for (int i = A4_starting_position_ - 1; i >= 0; i--) {
        frequencies_[pitchclass_[i]] = 2.0/3.0 * frequencies_[pitchclass_[i+1]] *
            pow(temperedcommas_[i+1], temperedfractions_[i+1]);
        if (pitchclass_[i] - pitchclass_[i+1] > 0)
            frequencies_[pitchclass_[i]] *= 2.0;
    }

    for (auto &i : frequencies_) {
        i *= exp2(octave_ - 4);
    }
}
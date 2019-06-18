#include "temperament.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

Constants::Constants(double pythagorean_comma, double syntonic_comma, double schisma) {
    pythagorean_comma_ = pythagorean_comma;
    syntonic_comma_    = syntonic_comma;
    schisma_           = schisma;
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

double Temperament::convert_to_cents(double ratio) {
    return 1200 * log2(ratio);
}

void Temperament::display_universal_info() const {
    cout << "A4 = " << concertA4_ << " Hz, " 
        << ntones_ << " tones per octave, " 
        << "octave " << octave_ << "." << endl;
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
    cout << "Equal temperament." << endl;
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./ntones_);
        temperedcommas_.push_back(C.pythagorean_comma_);
    }
}

void Temperament::pythagorean() {
    cout << "Pythagorean tuning." << endl;
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(0.0);
        temperedcommas_.push_back(0.0);
    }
}

void Temperament::meantone3() {
    cout << "Third-comma meantone." << endl;
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./3);
        temperedcommas_.push_back(C.syntonic_comma_);
    }
}

void Temperament::meantone4() {
    cout << "Quarter-comma meantone." << endl;
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./4);
        temperedcommas_.push_back(C.syntonic_comma_);
    }
}

void Temperament::meantone6() {
    cout << "Sixth-comma meantone." << endl;
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./6);
        temperedcommas_.push_back(C.syntonic_comma_);
    }
}

void Temperament::werckmeister3() {
    cout << "Werckmeister III." << endl;
    starting_note_ = "C";
    temperedfractions_ = {1./4, 1./4, 1./4, 0, 0, 1./4, 0, 0, 0, 0, 0};
    temperedcommas_ = {
        C.pythagorean_comma_, C.pythagorean_comma_, C.pythagorean_comma_,
        0, 0, C.pythagorean_comma_, 0, 0, 0, 0, 0
    };
}

void Temperament::kirnberger2() {
    cout << "Kirnberger II." << endl;
    starting_note_ = "C";
    temperedfractions_ = {0, 0, 1./2, 1./2, 0, 0, 1, 0, 0, 0, 0};
    temperedcommas_ = {
        0, 0, C.syntonic_comma_, C.syntonic_comma_,
        0, 0, C.schisma_, 0, 0, 0, 0
    };
}

void Temperament::kirnberger3() {
    cout << "Kirnberger III." << endl;
    starting_note_ = "C";
    temperedfractions_ = {1./4, 1./4, 1./4, 1./4, 0, 0, 1, 0, 0, 0, 0};
    temperedcommas_ = {
        C.syntonic_comma_, C.syntonic_comma_, 
        C.syntonic_comma_, C.syntonic_comma_,
        0, 0, C.schisma_, 0, 0, 0, 0
    };
}

void Temperament::vallotti() {
    cout << "Vallotti." << endl;
    starting_note_ = "F"; 
    temperedfractions_ = {1./6, 1./6, 1./6, 1./6, 1./6, 1./6, 0, 0, 0, 0, 0};
    temperedcommas_ = {
        C.pythagorean_comma_, C.pythagorean_comma_, C.pythagorean_comma_,
        C.pythagorean_comma_, C.pythagorean_comma_, C.pythagorean_comma_,
        0, 0, 0, 0, 0
    };
}

void Temperament::young1() {
    cout << "Young I." << endl;
    starting_note_ = "E-flat";
    temperedfractions_ = {0, 1./16, 1./16, 3./16, 3./16, 3./16, 3./16, 1./16, 1./16, 0, 0};
    temperedcommas_ = {
        0, 
        C.pythagorean_comma_ * pow(C.schisma_, 3), C.pythagorean_comma_ * pow(C.schisma_, 3),
        C.syntonic_comma_, C.syntonic_comma_, C.syntonic_comma_, C.syntonic_comma_,
        C.pythagorean_comma_ * pow(C.schisma_, 3), C.pythagorean_comma_ * pow(C.schisma_, 3),
        0, 0
    };
}

void Temperament::calculate_frequencies() {
    pitchclass_array();
    for (int i = 0; i < ntones_; i++)
        frequencies_.push_back(0.0);
    frequencies_[9] = concertA4_;

    // A -> E -> ...
    for (unsigned int i = A4_starting_position_ + 1; i < pitchclass_.size(); i++) {
        frequencies_[pitchclass_[i]] = 3./2 * frequencies_[pitchclass_[i-1]] /
            pow(temperedcommas_[i-1], temperedfractions_[i-1]);
        if (pitchclass_[i] - pitchclass_[i-1] < 0)
            frequencies_[pitchclass_[i]] *= 0.5;
    }

    // ... <- D <- A
    for (int i = A4_starting_position_ - 1; i >= 0; i--) {
        frequencies_[pitchclass_[i]] = 2./3 * frequencies_[pitchclass_[i+1]] *
            pow(temperedcommas_[i], temperedfractions_[i]);
        if (pitchclass_[i] - pitchclass_[i+1] > 0)
            frequencies_[pitchclass_[i]] *= 2.0;
    }

    for (auto &f : frequencies_) {
        f *= exp2(octave_ - 4);
    }
}

void Temperament::calculate_cents_bps() {
    vector<double> P5_info, M3_info, m3_info;
    for (auto &f : frequencies_) {
        P5_info.push_back(3*f);
        M3_info.push_back(5*f);
        m3_info.push_back(6*f);
    }

    for (int i = 0; i < ntones_; i++) {
        int i_P5 = (7 + i) % ntones_;
        if (i_P5 >= 7)
            P5_info.push_back(2*frequencies_[i_P5]);
        else
            P5_info.push_back(4*frequencies_[i_P5]);

        int i_M3 = (4 + i) % ntones_;
        if (i_M3 >= 4)
            M3_info.push_back(4*frequencies_[i_M3]);
        else
            M3_info.push_back(8*frequencies_[i_M3]);
        
        int i_m3 = (3 + i) % ntones_;
        if (i_m3 >= 3)
            m3_info.push_back(5*frequencies_[i_m3]);
        else
            m3_info.push_back(10*frequencies_[i_m3]); 
    }

    for (int i = 0; i < ntones_; i++) {
        centsP5_.push_back(convert_to_cents(P5_info[i+ntones_] / P5_info[i]));
        centsM3_.push_back(convert_to_cents(M3_info[i+ntones_] / M3_info[i]));
        centsm3_.push_back(convert_to_cents(m3_info[i+ntones_] / m3_info[i]));
        bpsP5_.push_back(abs(P5_info[i+ntones_] - P5_info[i]));
        bpsM3_.push_back(abs(M3_info[i+ntones_] - M3_info[i]));
        bpsm3_.push_back(abs(m3_info[i+ntones_] - m3_info[i]));
    }
}

void Temperament::display_temperament_table() const {
    printf("PC\tHz\tP5c\tM3c\tm3c\tP5b\tM3b\tm3b\n");
    for (int i = 0; i < ntones_; i++)
    printf("%d\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\n",
        i, frequencies_[i], 
        centsP5_[i], centsM3_[i], centsm3_[i],
        bpsP5_[i],   bpsM3_[i],   bpsm3_[i]);
}
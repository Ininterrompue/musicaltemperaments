#include "tuning.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>


double Constants::pythagorean_comma_ = 1.0136432647705078125;
double Constants::syntonic_comma_    = 1.0125;
double Constants::schisma_           = 1.001129150390625;


TuningSystem::TuningSystem(double concertA4, int ntones, std::string starting_note, int octave) {
    concertA4_     = concertA4;
    ntones_        = ntones;
    starting_note_ = starting_note;
    octave_        = octave;

    pitchclass_dict_ = {
        {"C-flat", 11}, {"C", 0},  {"C-sharp", 1},
        {"D-flat", 1},  {"D", 2},  {"D-sharp", 3},
        {"E-flat", 3},  {"E", 4},  {"E-sharp", 5},
        {"F-flat", 4},  {"F", 5},  {"F-sharp", 6},
        {"G-flat", 6},  {"G", 7},  {"G-sharp", 8},
        {"A-flat", 8},  {"A", 9},  {"A-sharp", 10},
        {"B-flat", 10}, {"B", 11}, {"B-sharp", 0}
    };
}

double TuningSystem::get_concertA4() const {
    return concertA4_;
}

int TuningSystem::get_ntones() const {
    return ntones_;
}

std::string TuningSystem::get_starting_note() const {
    return starting_note_;
}

int TuningSystem::get_octave() const {
    return octave_;
}

double TuningSystem::convert_to_cents(double ratio) {
    return 1200 * log2(ratio);
}

void TuningSystem::display_universal_info() const {
    std::cout << "A4 = " << concertA4_ << " Hz, " 
              << ntones_ << " tones per octave, " 
              << "octave " << octave_ << "." << std::endl;
}

/*
Sorts vector<int> pitchclass_ starting from the starting_note and progresses
by perfect fifths upwards. vector<int> pitchclass_ has ntones_ elements.
 */
void TuningSystem::pitchclass_array() {
    int starting_note_int = pitchclass_dict_[starting_note_];
    for (int i = 0; i < ntones_; i++)
        pitchclass_.push_back( (7*i + starting_note_int) % ntones_ );
    std::vector<int>::iterator it = find(pitchclass_.begin(), pitchclass_.end(), 9);
    starting_position_ = distance(pitchclass_.begin(), it);
}

void TuningSystem::calculate_cents_bps() {
    std::vector<double> P5_info, M3_info, m3_info;
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

    bpsP5_.clear();
    bpsM3_.clear();
    bpsm3_.clear();
    for (int i = 0; i < ntones_; i++) {
        centsP5_.push_back(convert_to_cents(P5_info[i+ntones_] / P5_info[i]));
        centsM3_.push_back(convert_to_cents(M3_info[i+ntones_] / M3_info[i]));
        centsm3_.push_back(convert_to_cents(m3_info[i+ntones_] / m3_info[i]));
        bpsP5_.push_back(fabs(P5_info[i+ntones_] - P5_info[i]));
        bpsM3_.push_back(fabs(M3_info[i+ntones_] - M3_info[i]));
        bpsm3_.push_back(fabs(m3_info[i+ntones_] - m3_info[i]));
    }
    for (int i = 0; i < ntones_; i++) {
        std::cout << P5_info[i+ntones_] - P5_info[i] << std::endl;
    }
    
}

void TuningSystem::display_tuning_table() const {
    printf("PC\tHz\tP5c\tM3c\tm3c\tP5b\tM3b\tm3b\n");
    for (int i = 0; i < ntones_; i++)
        printf("%d\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\n",
            i, frequencies_[i], 
            centsP5_[i], centsM3_[i], centsm3_[i],
            bpsP5_[i],   bpsM3_[i],   bpsm3_[i]);
}


int JustIntonation::calculate_dist() {
    starting_position_ = pitchclass_dict_[starting_note_];
    return pitchclass_dict_["A"] - starting_position_;
}

void JustIntonation::calculate_frequencies() {
    std::cout << "Just intonation, centered on " << starting_note_ << "." << std::endl;
    std::vector<double> ratios = 
        {1, 16./15, 9./8, 6./5, 5./4, 4./3, 45./32, 3./2, 8./5, 5./3, 16./9, 15./8};

    for (int i = 0; i < ntones_; i++) {
        frequencies_.push_back(0.0);
        pitchclass_.push_back(i);
    }
    frequencies_[9] = concertA4_;

    int dist = calculate_dist();
    if (dist > 0)
        frequencies_[starting_position_] = concertA4_ / ratios[abs(dist)];
    else
        frequencies_[starting_position_] = concertA4_ * ratios[abs(dist)];

    for (int i = 1; i < ntones_; i++) {
        unsigned int j = (starting_position_ + i) % ntones_;
        frequencies_[j] = frequencies_[starting_position_] * ratios[i];
        if (j < starting_position_)
            frequencies_[j] *= 0.5;
    }

    for (auto &f : frequencies_)
        f *= exp2(octave_ - 4);
}


void Temperament::calculate_frequencies() {
    pitchclass_array();
    for (int i = 0; i < ntones_; i++)
        frequencies_.push_back(0.0);
    frequencies_[9] = concertA4_;

    // A -> E -> ...
    for (unsigned int i = starting_position_ + 1; i < pitchclass_.size(); i++) {
        frequencies_[pitchclass_[i]] = 3./2 * frequencies_[pitchclass_[i-1]] /
            pow(temperedcommas_[i-1], temperedfractions_[i-1]);
        if (pitchclass_[i] - pitchclass_[i-1] < 0)
            frequencies_[pitchclass_[i]] *= 0.5;
    }

    // ... <- D <- A
    for (int i = starting_position_ - 1; i >= 0; i--) {
        frequencies_[pitchclass_[i]] = 2./3 * frequencies_[pitchclass_[i+1]] *
            pow(temperedcommas_[i], temperedfractions_[i]);
        if (pitchclass_[i] - pitchclass_[i+1] > 0)
            frequencies_[pitchclass_[i]] *= 2.0;
    }

    for (auto &f : frequencies_)
        f *= exp2(octave_ - 4);
}

void Temperament::equal() {
    std::cout << "Equal temperament." << std::endl;
    starting_note_ = "E-flat";
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./ntones_);
        temperedcommas_.push_back(Constants::pythagorean_comma_);
    }
}

void Temperament::pythagorean() {
    std::cout << "Pythagorean tuning." << std::endl;
    starting_note_ = "E-flat";
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(0.0);
        temperedcommas_.push_back(0.0);
    }
}

void Temperament::meantone3() {
    std::cout << "Third-comma meantone." << std::endl;
    starting_note_ = "E-flat";
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./3);
        temperedcommas_.push_back(Constants::syntonic_comma_);
    }
}

void Temperament::meantone4() {
    std::cout << "Quarter-comma meantone." << std::endl;
    starting_note_ = "E-flat";
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./4);
        temperedcommas_.push_back(Constants::syntonic_comma_);
    }
}

void Temperament::meantone6() {
    std::cout << "Sixth-comma meantone." << std::endl;
    starting_note_ = "E-flat";
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(1./6);
        temperedcommas_.push_back(Constants::syntonic_comma_);
    }
}

void Temperament::werckmeister3() {
    std::cout << "Werckmeister III." << std::endl;
    starting_note_ = "C";
    temperedfractions_ = {1./4, 1./4, 1./4, 0, 0, 1./4, 0, 0, 0, 0, 0};
    temperedcommas_ = {
        Constants::pythagorean_comma_, Constants::pythagorean_comma_, 
        Constants::pythagorean_comma_, 0, 0, Constants::pythagorean_comma_, 
        0, 0, 0, 0, 0
    };
}

void Temperament::kirnberger2() {
    std::cout << "Kirnberger II." << std::endl;
    starting_note_ = "C";
    temperedfractions_ = {0, 0, 1./2, 1./2, 0, 0, 1, 0, 0, 0, 0};
    temperedcommas_ = {
        0, 0, Constants::syntonic_comma_, Constants::syntonic_comma_,
        0, 0, Constants::schisma_, 0, 0, 0, 0
    };
}

void Temperament::kirnberger3() {
    std::cout << "Kirnberger III." << std::endl;
    starting_note_ = "C";
    temperedfractions_ = {1./4, 1./4, 1./4, 1./4, 0, 0, 1, 0, 0, 0, 0};
    temperedcommas_ = {
        Constants::syntonic_comma_, Constants::syntonic_comma_, 
        Constants::syntonic_comma_, Constants::syntonic_comma_,
        0, 0, Constants::schisma_, 0, 0, 0, 0
    };
}

void Temperament::vallotti() {
    std::cout << "Vallotti." << std::endl;
    starting_note_ = "F"; 
    temperedfractions_ = {1./6, 1./6, 1./6, 1./6, 1./6, 1./6, 0, 0, 0, 0, 0};
    temperedcommas_ = {
        Constants::pythagorean_comma_, Constants::pythagorean_comma_, 
        Constants::pythagorean_comma_, Constants::pythagorean_comma_, 
        Constants::pythagorean_comma_, Constants::pythagorean_comma_,
        0, 0, 0, 0, 0
    };
}

void Temperament::young1() {
    std::cout << "Young I." << std::endl;
    starting_note_ = "E-flat";
    temperedfractions_ = {0, 1./16, 1./16, 3./16, 3./16, 3./16, 3./16, 1./16, 1./16, 0, 0};
    temperedcommas_ = {
        0, 
        Constants::pythagorean_comma_ * pow(Constants::schisma_, 3), 
        Constants::pythagorean_comma_ * pow(Constants::schisma_, 3),
        Constants::syntonic_comma_, Constants::syntonic_comma_, 
        Constants::syntonic_comma_, Constants::syntonic_comma_,
        Constants::pythagorean_comma_ * pow(Constants::schisma_, 3), 
        Constants::pythagorean_comma_ * pow(Constants::schisma_, 3),
        0, 0
    };
}


void EqualBeating::calculate_frequencies() {
    pitchclass_array();
    for (int i = 0; i < ntones_; i++)
        frequencies_.push_back(0.0);
    frequencies_[9] = concertA4_;

    for (auto &f : bpsP5_)
        f *= concertA4_/440;

    // A -> E -> ...
    for (unsigned int i = starting_position_ + 1; i < pitchclass_.size(); i++) {
        frequencies_[pitchclass_[i]] = 3./2 * frequencies_[pitchclass_[i-1]] -
            1./2 * bpsP5_[i-1];
        if (pitchclass_[i] - pitchclass_[i-1] < 0)
            frequencies_[pitchclass_[i]] *= 0.5;
    }

    // ... <- D <- A
    for (int i = starting_position_ - 1; i >= 0; i--) {
        frequencies_[pitchclass_[i]] = 2./3 * frequencies_[pitchclass_[i+1]] +
            1./3 * bpsP5_[i];
        if (pitchclass_[i] - pitchclass_[i+1] > 0) {
            frequencies_[pitchclass_[i]] *= 2.0;
            frequencies_[pitchclass_[i]] -= 1./3 * bpsP5_[i];
        }
    }

    for (auto &f : frequencies_)
        f *= exp2(octave_ - 4);
}

void EqualBeating::prelleur() {
    std::cout << "Prelleur." << std::endl;
    starting_note_ = "A-flat";
    bpsP5_ = {0, 0.4, 0.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1, 1};
}

#include "tuning.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>


double Constants::pythagorean_comma_ = 1.0136432647705078125;
double Constants::syntonic_comma_    = 1.0125;
double Constants::schisma_           = 1.001129150390625;


TuningSystem::TuningSystem(double concertA4, std::string starting_note, int octave) {
    concertA4_     = concertA4;
    ntones_        = 12;
    starting_note_ = starting_note;
    octave_        = octave;

    P5steps_ = 7;
    M3steps_ = 4;
    m3steps_ = 3;

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
    int starting_note_int = pitchclass_dict_.at(starting_note_);
    for (int i = 0; i < ntones_; i++)
        pitchclass_.push_back( (7*i + starting_note_int) % ntones_ );
    std::vector<int>::iterator it = find(pitchclass_.begin(), pitchclass_.end(), 9);
    starting_position_ = distance(pitchclass_.begin(), it);
}

void TuningSystem::correct_octave() {
    for (auto &f : frequencies_)
        f *= exp2(octave_ - 4);
}

void TuningSystem::calculate_cents_bps() {
    std::vector<double> P5_info, M3_info, m3_info;
    for (auto &f : frequencies_) {
        P5_info.push_back(3*f);
        M3_info.push_back(5*f);
        m3_info.push_back(6*f);
    }

    for (int i = 0; i < ntones_; i++) {
        int i_P5 = (P5steps_ + i) % ntones_;
        i_P5 >= P5steps_ ? P5_info.push_back(2*frequencies_.at(i_P5))
                         : P5_info.push_back(4*frequencies_.at(i_P5));

        int i_M3 = (M3steps_ + i) % ntones_;
        i_M3 >= M3steps_ ? M3_info.push_back(4*frequencies_.at(i_M3))
                         : M3_info.push_back(8*frequencies_.at(i_M3));

        int i_m3 = (m3steps_ + i) % ntones_;
        i_m3 >= m3steps_ ? m3_info.push_back(5*frequencies_.at(i_m3))
                         : m3_info.push_back(10*frequencies_.at(i_m3)); 
    }

    bpsP5_.clear();
    bpsM3_.clear();
    bpsm3_.clear();
    for (int i = 0; i < ntones_; i++) {
        centsP5_.push_back(convert_to_cents(P5_info.at(i+ntones_) / P5_info.at(i)));
        centsM3_.push_back(convert_to_cents(M3_info.at(i+ntones_) / M3_info.at(i)));
        centsm3_.push_back(convert_to_cents(m3_info.at(i+ntones_) / m3_info.at(i)));
        bpsP5_.push_back(std::abs(P5_info.at(i+ntones_) - P5_info.at(i)));
        bpsM3_.push_back(std::abs(M3_info.at(i+ntones_) - M3_info.at(i)));
        bpsm3_.push_back(std::abs(m3_info.at(i+ntones_) - m3_info.at(i)));
    }
}

void TuningSystem::display_tuning_table() const {
    printf("PC\tHz\tP5c\tM3c\tm3c\tP5b\tM3b\tm3b\n");
    for (int i = 0; i < ntones_; i++)
        printf("%d\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\n",
            i, frequencies_.at(i), 
            centsP5_.at(i), centsM3_.at(i), centsm3_.at(i),
            bpsP5_.at(i),   bpsM3_.at(i),   bpsm3_.at(i));
}


int JustIntonation::calculate_dist() {
    starting_position_ = pitchclass_dict_.at(starting_note_);
    return pitchclass_dict_.at("A") - starting_position_;
}

void JustIntonation::calculate_frequencies() {
    std::cout << "Just intonation, centered on " << starting_note_ << "." << std::endl;
    std::vector<double> ratios = 
        {1, 16./15, 9./8, 6./5, 5./4, 4./3, 45./32, 3./2, 8./5, 5./3, 16./9, 15./8};

    for (int i = 0; i < ntones_; i++) {
        frequencies_.push_back(0.0);
        pitchclass_.push_back(i);
    }
    frequencies_.at(9) = concertA4_;

    int dist = calculate_dist();
    dist > 0 ? frequencies_.at(starting_position_) = concertA4_ / ratios.at(std::abs(dist))
             : frequencies_.at(starting_position_) = concertA4_ * ratios.at(std::abs(dist));

    for (int i = 1; i < ntones_; i++) {
        unsigned int j = (starting_position_ + i) % ntones_;
        frequencies_.at(j) = frequencies_.at(starting_position_) * ratios.at(i);
        if (j < starting_position_)
            frequencies_.at(j) *= 0.5;
    }

    correct_octave();
}


void Temperament::calculate_frequencies() {
    pitchclass_array();
    for (int i = 0; i < ntones_; i++)
        frequencies_.push_back(0.0);
    frequencies_.at(9) = concertA4_;

    // A -> E -> ...
    for (unsigned int i = starting_position_ + 1; i < pitchclass_.size(); i++) {
        frequencies_.at(pitchclass_.at(i)) = 3./2 * frequencies_.at(pitchclass_.at(i-1))
            / pow(temperedcommas_.at(i-1), temperedfractions_.at(i-1));
        if (pitchclass_.at(i) - pitchclass_.at(i-1) < 0)
            frequencies_.at(pitchclass_.at(i)) *= 0.5;
    }

    // ... <- D <- A
    for (int i = starting_position_ - 1; i >= 0; i--) {
        frequencies_.at(pitchclass_.at(i)) = 2./3 * frequencies_.at(pitchclass_.at(i+1))
            * pow(temperedcommas_.at(i), temperedfractions_.at(i));
        if (pitchclass_.at(i) - pitchclass_.at(i+1) > 0)
            frequencies_.at(pitchclass_.at(i)) *= 2.0;
    }

    correct_octave();
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

void Temperament::meantoneN(double temper, double comma) {
    std::cout << "Custom meantone." << std::endl;
    for (int i = 0; i < ntones_ - 1; i++) {
        temperedfractions_.push_back(temper);
        temperedcommas_.push_back(comma);
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


void EqualTemperament::calculate_frequencies() {
    std::cout << ntones_ << " EDO." << std::endl;
    stepsize_ = exp2(1./ntones_);
    int M6steps_ = ntones_ - m3steps_;

    for (int i = 0; i < ntones_; i++)
        frequencies_.push_back(concertA4_ * pow(stepsize_, i - M6steps_));

    correct_octave();
}

void EqualTemperament::equalN(int ntones) {
    ntones_ = ntones;

    std::vector<double> P5dist;
    std::vector<double> M3dist;
    std::vector<double> m3dist;

    for (int i = 0; i < ntones_; i++) {
        P5dist.push_back(std::abs(3./2 - exp2(1.*i/ntones_)));
        M3dist.push_back(std::abs(5./4 - exp2(1.*i/ntones_)));
        m3dist.push_back(std::abs(6./5 - exp2(1.*i/ntones_)));
    }
    P5steps_ = std::min_element(P5dist.begin(), P5dist.end()) - P5dist.begin();
    M3steps_ = std::min_element(M3dist.begin(), M3dist.end()) - M3dist.begin();
    m3steps_ = std::min_element(m3dist.begin(), m3dist.end()) - m3dist.begin();
}


void BpsTemperament::calculate_frequencies() {
    pitchclass_array();
    for (int i = 0; i < ntones_; i++)
        frequencies_.push_back(0.0);
    frequencies_.at(9) = concertA4_;

    for (auto &f : bpsP5_)
        f *= concertA4_/440;

    // A -> E -> ...
    for (unsigned int i = starting_position_ + 1; i < pitchclass_.size(); i++) {
        frequencies_.at(pitchclass_.at(i)) = 3./2 * frequencies_.at(pitchclass_.at(i-1))
            - 1./2 * bpsP5_.at(i-1);
        if (pitchclass_.at(i) - pitchclass_.at(i-1) < 0)
            frequencies_.at(pitchclass_.at(i)) *= 0.5;
    }

    // ... <- D <- A
    for (int i = starting_position_ - 1; i >= 0; i--) {
        frequencies_.at(pitchclass_.at(i)) = pitchclass_.at(i) - pitchclass_.at(i+1) < 0
            ? 2./3 * frequencies_.at(pitchclass_.at(i+1)) + 1./3 * bpsP5_.at(i)
            : 4./3 * frequencies_.at(pitchclass_.at(i+1)) + 1./3 * bpsP5_.at(i);
    }

    correct_octave();
}

void BpsTemperament::prelleur() {
    std::cout << "Prelleur." << std::endl;
    starting_note_ = "A-flat";
    bpsP5_ = {0, 0.4, 0.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1, 1};
}

void BpsTemperament::tansur() {
    std::cout << "Tans'ur." << std::endl;
    starting_note_ = "E-flat";
    bpsP5_ = {0, 0, 0, 1.3, 2.6, 2.6, 2.6, 2.6, 2.6, 0, 0};
}

void BpsTemperament::ellis1875() {
    std::cout << "Ellis's 1875 quasiequal." << std::endl;
    starting_note_ = "C";
    bpsP5_ = {1, 1.5, 1, 1.5, 1, 1.5, 1.5, 1, 1.5, 1, 1.5};
}

void BpsTemperament::ellis1885() {
    std::cout << "Ellis's 1885 quasiequal." << std::endl;
    starting_note_ = "C";
    bpsP5_ = {1, 1.2, 1, 1.2, 1, 2, 1.2, 1, 1.2, 1, 2};
}

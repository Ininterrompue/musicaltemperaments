#include "temperament.hpp"

Temperament::Temperament(double concertA4, int ntones, string based) {
    concertA4_ = concertA4;
    ntones_ = ntones;
    based_ = based;
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
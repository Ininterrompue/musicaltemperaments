#include <string>

using namespace std;

class Temperament {
private:
    double concertA4_;
    int ntones_;
    string based_;
    

public:
    Temperament(double concertA4_ = 440.0, int ntones_ = 12, string based_ = "C");

    double get_concertA4() const;
    int get_ntones() const;
    string get_based() const;
};
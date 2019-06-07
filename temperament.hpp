#include <string>
#include <vector>

using namespace std;

class Constants {
public:
    double pythagorean_comma_;
    double syntonic_comma_;
    double lesser_diesis_;
    double greater_diesis_;
    double schisma_;

    Constants();

    double convert_to_cents(double comma);
};

class Temperament {
protected:
    double concertA4_;
    int ntones_;
    string based_;
    int octave_;

public: 
    vector<double> temperedP5_;
    vector<double> frequencies_;

    Temperament(double concertA4 = 440.0, 
                int ntones = 12, 
                string based = "C",
                int octave = 4);

    double get_concertA4() const;
    int get_ntones() const;
    string get_based() const;
    int get_octave() const;
    void equal_temperament();
    void calculate_frequencies();
};
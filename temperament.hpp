#include <string>
#include <vector>

using namespace std;

class Constants {
public:
    double pythagorean_comma_;
    double syntonic_comma_;
    double schisma_;

    Constants(double pythagorean_comma = 1.0136432647705078125, 
              double syntonic_comma    = 1.0125, 
              double schisma           = 1.001129150390625);

    double convert_to_cents(double comma);
};

class Temperament {
protected:
    double concertA4_;
    int ntones_;
    string starting_note_;
    int octave_;
    unsigned int A4_starting_position_;

public: 
    vector<int>    pitchclass_;
    vector<double> temperedfractions_;
    vector<double> temperedcommas_;
    vector<double> frequencies_;

    Temperament(double concertA4 = 440.0,        int ntones = 12, 
                string starting_note = "E-flat", int octave = 4);

    double get_concertA4() const;
    int get_ntones() const;
    string get_starting_note() const;
    int get_octave() const;
    void pitchclass_array();

    void equal();
    void pythagorean();
    void meantone4();
    void meantone6();
    void kirnberger3();

    void calculate_frequencies();
};
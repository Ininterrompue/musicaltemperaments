#include <string>
#include <vector>
#include <map>


class Constants {
public:
    double pythagorean_comma_;
    double syntonic_comma_;
    double schisma_;

    Constants(double pythagorean_comma = 1.0136432647705078125, 
              double syntonic_comma    = 1.0125, 
              double schisma           = 1.001129150390625);
};


class TuningSystem {
protected:
    double concertA4_;
    int ntones_;
    std::string starting_note_;
    int octave_;
    unsigned int starting_position_;

public: 
    std::vector<int>    pitchclass_;
    std::map<std::string, int> pitchclass_dict_;
    std::vector<double> frequencies_;
    std::vector<double> centsP5_;
    std::vector<double> centsM3_;
    std::vector<double> centsm3_;
    std::vector<double> bpsP5_;
    std::vector<double> bpsM3_;
    std::vector<double> bpsm3_;

    TuningSystem(double concertA4 = 440.0,             int ntones = 12, 
                 std::string starting_note = "E-flat", int octave = 4);

    double get_concertA4() const;
    int get_ntones() const;
    std::string get_starting_note() const;
    int get_octave() const;
    void display_universal_info() const;
    double convert_to_cents(double ratio);
    void pitchclass_array();

    void calculate_cents_bps();
    void display_tuning_table() const;
};


class JustIntonation: public TuningSystem {
public:
    using TuningSystem::TuningSystem;

    int calculate_dist();
    void calculate_frequencies();
};

class Temperament: public TuningSystem {
public:
    std::vector<double> temperedfractions_;
    std::vector<double> temperedcommas_;

    using TuningSystem::TuningSystem;

    // Meantone temperaments
    void equal();
    void pythagorean();
    void meantone3();
    void meantone4();
    void meantone6();

    // Well temperaments
    void werckmeister3();
    void kirnberger2();
    void kirnberger3();
    void vallotti();
    void young1();

    void calculate_frequencies();
};

class EqualBeating: public TuningSystem {
public:
    using TuningSystem::TuningSystem;

    void prelleur();

    void calculate_frequencies();
};
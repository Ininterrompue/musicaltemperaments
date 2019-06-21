#include <string>
#include <vector>
#include <map>


class Constants {
public:
    static double pythagorean_comma_;
    static double syntonic_comma_;
    static double schisma_;
};

class TuningSystem {
protected:
    double concertA4_;
    int ntones_;
    std::string starting_note_;
    int octave_;
    int P5steps_;
    int M3steps_;
    int m3steps_;
    unsigned int starting_position_;

    std::vector<int> pitchclass_;
    std::map<std::string, int> pitchclass_dict_;
    std::vector<double> frequencies_;
    std::vector<double> centsP5_;
    std::vector<double> centsM3_;
    std::vector<double> centsm3_;
    std::vector<double> bpsP5_;
    std::vector<double> bpsM3_;
    std::vector<double> bpsm3_;

public: 
    TuningSystem(double concertA4 = 440.0, 
                 std::string starting_note = "E-flat", 
                 int octave = 4);

    void display_universal_info() const;
    double convert_to_cents(double ratio);
    void pitchclass_array();
    void correct_octave();
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
protected:
    std::vector<double> temperedfractions_;
    std::vector<double> temperedcommas_;

public:
    using TuningSystem::TuningSystem;

    // Meantone temperaments
    void equal();
    void pythagorean();
    void meantone3();
    void meantone4();
    void meantone6();
    void meantone(double temper, double comma);

    // Well temperaments
    void werckmeister3();
    void kirnberger2();
    void kirnberger3();
    void vallotti();
    void young1();

    void calculate_frequencies();
};

class EqualTemperament: public TuningSystem {
private:
    double stepsize_;

public:
    using TuningSystem::TuningSystem;

    void equal12();
    void equal19();
    void equal31();
    void equal53();
    void equaln(int ntones);

    void calculate_frequencies();
};

class EqualBeating: public TuningSystem {
public:
    using TuningSystem::TuningSystem;

    void prelleur();

    void calculate_frequencies();
};

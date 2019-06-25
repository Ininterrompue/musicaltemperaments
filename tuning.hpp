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
    std::vector<double> cents_from_ET_;

public: 
    TuningSystem(double concertA4 = 440, 
                 std::string starting_note = "E-flat", 
                 int octave = 4);

    void display_universal_info() const;
    double convert_to_cents(double ratio);
    void pitchclass_array();
    void correct_octave();
    void calculate_cents_bps();
    void calculate_et_deviations();
    void display_tuning_table() const;
    void display_tuning_table(std::string filename) const;
};

class JustIntonation: public TuningSystem {
protected:
    std::vector<double> ratios_;

public:
    using TuningSystem::TuningSystem;

    void just5();

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
    void meantoneN(double temper, double comma);

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

    void equalN(int ntones);

    void calculate_frequencies();
};

class BpsTemperament: public TuningSystem {
public:
    using TuningSystem::TuningSystem;

    void prelleur();
    void tansur();
    void ellis1875();
    void ellis1885();

    void calculate_frequencies();
};

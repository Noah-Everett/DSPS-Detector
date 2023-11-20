#ifndef NESTDetector_hh
#define NESTDetector_hh

#include "VDetector.hh"

using std::vector;

class NESTDetector : public VDetector {
    public:
        NESTDetector();
       ~NESTDetector() override = default;

        void Initialization() override;

        double           FitEF   ( double, double, double ) override;
        vector< double > FitDirEF( double, double, double ) override;
};

#endif
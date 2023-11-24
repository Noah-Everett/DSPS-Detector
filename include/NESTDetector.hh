//*/////////////////////////////////////////////////////////////////////////*//
//*//                    G4-DSPS-Detector-Simulation                      //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*// Author:                                                             //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                       //*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//

#ifndef NESTDetector_hh
#define NESTDetector_hh

#include "VDetector.hh"

#include "DetectorConstruction.hh"

using std::vector;

class NESTDetector : public VDetector 
{
    public:
        NESTDetector( DetectorConstruction* );
       ~NESTDetector(                       ) override = default;

        void Initialization() override;

        double           FitEF   ( double, double, double ) override;
        vector< double > FitDirEF( double, double, double ) override;

    protected:
        DetectorConstruction* m_detectorConstruction{ nullptr };
};

#endif
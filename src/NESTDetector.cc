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

#include "NESTDetector.hh"

NESTDetector::NESTDetector( DetectorConstruction* t_detectorConstruction ) :
    m_detectorConstruction( t_detectorConstruction ) {
    Initialization();
}

void NESTDetector::Initialization() {
    // Primary Scintillation (S1) parameters
    g1 = 0.113569;  // 0.114 +/- 0.002  // phd per S1 phot at dtCntr (not phe).
                    // Divide out 2-PE effect
    sPEres = 0.31;  // single phe resolution (Gaussian assumed)
    sPEthr = 0.10;  // POD threshold in phe, usually used IN PLACE of sPEeff
    sPEeff = 1.0;   // actual efficiency, can be used in lieu of POD threshold
    noiseBaseline[0] = 0.0;  // baseline noise mean and width in PE (Gaussian)
    noiseBaseline[1] = 0.0;  // baseline noise mean and width in PE (Gaussian)
    noiseBaseline[2] = 0.;
    noiseBaseline[3] = 0.;
    P_dphe =
        0.214;  // chance 1 photon makes 2 phe instead of 1 in Hamamatsu PMT

    coinWind = 300;  // S1 coincidence window in ns
    coinLevel = 3;   // how many PMTs have to fire for an S1 to count
    numPMTs = 484;  // Taking into account turned-off PMTs    // For coincidence
                    // calculation

    OldW13eV = true;
    noiseLinear[0] = 0.;
    noiseLinear[1] = 0.;

    // Ionization and Secondary Scintillation (S2) parameters
    g1_gas = 0.092103545;  // 0.092 +/- 0.002 // phd per S2 photon in gas, used
                           // to get SE size
    s2Fano = 2.0;          // Fano-like fudge factor for SE width
    s2_thr =
        600. *
        (1.0 +
         P_dphe);  // the S2 threshold in phe or PE, *not* phd. Affects NR most
    E_gas = 0;//8.42417;   // field in kV/cm between liquid/gas border and anode
    eLife_us = 6500.;  // the drift electron mean lifetime in micro-seconds

    // Thermodynamic Properties
    inGas = false;
    T_Kelvin = 174.1;  // for liquid drift speed calculation
    p_bar = 1.79;      // gas pressure in units of bars, it controls S2 size
    // if you are getting warnings about being in gas, lower T and/or raise p

    // Data Analysis Parameters and Geometry
    dtCntr = 462.5;  // central correction bin is between 425-500us // center of
                     // detector for S1 corrections, in usec.
    dt_min = 86.;    // minimum. Top of detector fiducial volume
    dt_max = 936.5;  // maximum. Bottom of detector fiducial volume

    radius = 688.;  // millimeters (fiducial radius)
    radmax = 728.;  // actual physical geo. limit

    TopDrift = 1461.;  // mm not cm or us (but, this *is* where dt=0)
    // a z-axis value of 0 means the bottom of the detector (cathode OR bottom
    // PMTs)
    // In 2-phase, TopDrift=liquid/gas border. In gas detector it's GATE, not
    // anode!
    anode = 1469.;  // the level of the anode grid-wire plane in mm
    // In a gas TPC, this is not TopDrift (top of drift region), but a few mm
    // above it
    gate = 1456.;  // mm. This is where the E-field changes (higher)
    // in gas detectors, the gate is still the gate, but it's where S2 starts
    cathode = 0.;  // mm. Defines point below which events are gamma-X

    // 2-D (X & Y) Position Reconstruction
    // Set these to zero to implement "perfect" position corrections
    // Note: LZ used spatial maps to implement S1 and S2 corrections,
    // but for simplicity with this header file, we'll circumvent
    // the need for corrections entirely.
    PosResExp = 0.0;   // exp increase in pos recon res at hi r, 1/mm
    PosResBase = 0.0;  // baseline unc in mm, see NEST.cpp for usage
}

double NESTDetector::FitS1( double t_xPos_mm, double t_yPos_mm, double t_zPos_mm, LCE t_map ) {
    return 0;
}

double NESTDetector::FitS2( double t_xPos_mm, double t_yPos_mm, LCE t_map ) {
    return 0;
}

double NESTDetector::FitEF( double t_xPos_mm, double t_yPos_mm, double t_zPos_mm ) {
    return 0;
}

vector< double > NESTDetector::FitTBA( double t_xPos_mm, double t_yPos_mm, double t_zPos_mm ) {
    return { 0, 0 };
}

double NESTDetector::OptTrans( double t_xPos_mm, double t_yPos_mm, double t_zPos_mm ) {
    return 0;
}

vector< double > NESTDetector::SinglePEWaveForm( double t_xPos_mm, double t_yPos_mm ) {
    return vector< double >( 7 );
}
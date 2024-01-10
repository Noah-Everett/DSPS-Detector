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

#ifndef Materials_hh
#define Materials_hh

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"

#include "ConstructionMessenger.hh"

using std::vector;

class Materials
{
    public:
        static Materials* get_instance   ();
        static void       delete_instance();

        void print_materials();

    protected:
        Materials();
       ~Materials() {}

        static Materials* m_instance;

        void constructMaterials                   ();
        void constructMaterial_G4Water            ();
        void constructMaterial_G4Air              ();
        void constructMaterial_G4StainlessSteel   ();
        void constructMaterial_G4Vacuum           ();
        void constructMaterial_LXe                ();
        void constructMaterial_ANNIETankSteel     ();
        void constructMaterial_ANNIEMRDIron       ();
        void constructMaterial_ANNIEMRDScintilator();
        void constructMaterial_ANNIEWater_unDoped ();
        void constructMaterial_ANNIEWater_GdDoped ();
        void constructMaterial_aluminum           ();
        void constructMaterial_SNPH2              ();
        void constructMaterial_nonInteractive     ();
        void constructMaterial_veryInteractive    ();

        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };

        G4NistManager* m_nist{ G4NistManager::Instance() };

        G4Material* m_material_LXe                { nullptr };
        G4Material* m_material_ANNIETankSteel     { nullptr };
        G4Material* m_material_ANNIEMRDIron       { nullptr };
        G4Material* m_material_ANNIEMRDScintilator{ nullptr };
        G4Material* m_material_G4Water            { nullptr };
        G4Material* m_material_G4Air              { nullptr };
        G4Material* m_material_G4StainlessSteel   { nullptr };
        G4Material* m_material_G4Vacuum           { nullptr };
        G4Material* m_material_ANNIEWater_unDoped { nullptr };
        G4Material* m_material_ANNIEWater_GdDoped { nullptr };
        G4Material* m_material_aluminum           { nullptr };
        G4Material* m_material_SNPH2              { nullptr };
        G4Material* m_material_nonInteractive     { nullptr };
        G4Material* m_material_veryInteractive    { nullptr };

        G4Isotope* m_isotope_Gd152{ new G4Isotope( "Gd152", 64, 152, 152.0 * g / mole ) };
        G4Isotope* m_isotope_Gd154{ new G4Isotope( "Gd154", 64, 154, 154.0 * g / mole ) };
        G4Isotope* m_isotope_Gd155{ new G4Isotope( "Gd155", 64, 155, 155.0 * g / mole ) };
        G4Isotope* m_isotope_Gd156{ new G4Isotope( "Gd156", 64, 156, 156.0 * g / mole ) };
        G4Isotope* m_isotope_Gd157{ new G4Isotope( "Gd157", 64, 157, 157.0 * g / mole ) };
        G4Isotope* m_isotope_Gd158{ new G4Isotope( "Gd158", 64, 158, 158.0 * g / mole ) };
        G4Isotope* m_isotope_Gd160{ new G4Isotope( "Gd160", 64, 160, 160.0 * g / mole ) };

        G4Element* m_element_H { new G4Element( "Hydrogen"  , "H" , 1 , 1.01    * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_O { new G4Element( "Oxygen"    , "O" , 8 , 16.00   * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_Gd{ new G4Element( "Gadolinium", "Gd", 64, 157.25  * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_C { new G4Element( "Carbon"    , "C" , 6 , 12.01   * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_Fe{ new G4Element( "Iron"      , "Fe", 26, 55.85   * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_Cr{ new G4Element( "Chromium"  , "Cr", 24, 51.9961 * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_Ni{ new G4Element( "Nickel"    , "Ni", 28, 58.6934 * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_Mn{ new G4Element( "Manganese" , "Mn", 25, 54.938  * CLHEP::g / CLHEP::mole ) }; 
        G4Element* m_element_P { new G4Element( "Phosphore" , "P" , 15, 30.974  * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_Si{ new G4Element( "Silicon"   , "Si", 14, 28.09   * CLHEP::g / CLHEP::mole ) }; 
        G4Element* m_element_S { new G4Element( "Sulphur"   , "S" , 16, 32.065  * CLHEP::g / CLHEP::mole ) };
        G4Element* m_element_Al{ new G4Element( "Aluminum"  , "Al", 13, 26.98   * CLHEP::g / CLHEP::mole ) };  
        G4Element* m_element_Gd_complete { new G4Element( "Gadolinium", "Gd", 7 ) };

        G4MaterialPropertiesTable* m_materialPropertiesTable_LXe                { new G4MaterialPropertiesTable() };
        G4MaterialPropertiesTable* m_materialPropertiesTable_G4Water            { new G4MaterialPropertiesTable() };
        G4MaterialPropertiesTable* m_materialPropertiesTable_ANNIEWater         { new G4MaterialPropertiesTable() };
        G4MaterialPropertiesTable* m_materialPropertiesTable_ANNIEMRDScintilator{ new G4MaterialPropertiesTable() };
        G4MaterialPropertiesTable* m_materialPropertiesTable_SNPH2              { new G4MaterialPropertiesTable() };
        G4MaterialPropertiesTable* m_materialPropertiesTable_nonInteractive     { new G4MaterialPropertiesTable() };
        G4MaterialPropertiesTable* m_materialPropertiesTable_veryInteractive    { new G4MaterialPropertiesTable() };

        //////////////////////////////////////////////////////////////////
        //////////////////     MATERIAL PROPERTIES     ///////////////////
        //////////////////////////////////////////////////////////////////
        static const G4int m_size_mpt_water{ 60 };
        G4double m_ABWFF{ 1.30 };
        G4double m_RAYFF{ 0.75 };
        G4double m_MIEFF{ 0.0  };
        G4double m_water_mpt_energies[ m_size_mpt_water ]{ 1.56962e-09*CLHEP::GeV, 1.58974e-09*CLHEP::GeV, 1.61039e-09*CLHEP::GeV, 1.63157e-09*CLHEP::GeV, 
                                                           1.65333e-09*CLHEP::GeV, 1.67567e-09*CLHEP::GeV, 1.69863e-09*CLHEP::GeV, 1.72222e-09*CLHEP::GeV, 
                                                           1.74647e-09*CLHEP::GeV, 1.77142e-09*CLHEP::GeV, 1.79710e-09*CLHEP::GeV, 1.82352e-09*CLHEP::GeV, 
                                                           1.85074e-09*CLHEP::GeV, 1.87878e-09*CLHEP::GeV, 1.90769e-09*CLHEP::GeV, 1.93749e-09*CLHEP::GeV, 
                                                           1.96825e-09*CLHEP::GeV, 1.99999e-09*CLHEP::GeV, 2.03278e-09*CLHEP::GeV, 2.06666e-09*CLHEP::GeV,
                                                           2.10169e-09*CLHEP::GeV, 2.13793e-09*CLHEP::GeV, 2.17543e-09*CLHEP::GeV, 2.21428e-09*CLHEP::GeV, 
                                                           2.25454e-09*CLHEP::GeV, 2.29629e-09*CLHEP::GeV, 2.33962e-09*CLHEP::GeV, 2.38461e-09*CLHEP::GeV, 
                                                           2.43137e-09*CLHEP::GeV, 2.47999e-09*CLHEP::GeV, 2.53061e-09*CLHEP::GeV, 2.58333e-09*CLHEP::GeV, 
                                                           2.63829e-09*CLHEP::GeV, 2.69565e-09*CLHEP::GeV, 2.75555e-09*CLHEP::GeV, 2.81817e-09*CLHEP::GeV, 
                                                           2.88371e-09*CLHEP::GeV, 2.95237e-09*CLHEP::GeV, 3.02438e-09*CLHEP::GeV, 3.09999e-09*CLHEP::GeV,
                                                           3.17948e-09*CLHEP::GeV, 3.26315e-09*CLHEP::GeV, 3.35134e-09*CLHEP::GeV, 3.44444e-09*CLHEP::GeV, 
                                                           3.54285e-09*CLHEP::GeV, 3.64705e-09*CLHEP::GeV, 3.75757e-09*CLHEP::GeV, 3.87499e-09*CLHEP::GeV, 
                                                           3.99999e-09*CLHEP::GeV, 4.13332e-09*CLHEP::GeV, 4.27585e-09*CLHEP::GeV, 4.42856e-09*CLHEP::GeV, 
                                                           4.59258e-09*CLHEP::GeV, 4.76922e-09*CLHEP::GeV, 4.95999e-09*CLHEP::GeV, 5.16665e-09*CLHEP::GeV, 
                                                           5.39129e-09*CLHEP::GeV, 5.63635e-09*CLHEP::GeV, 5.90475e-09*CLHEP::GeV, 6.19998e-09*CLHEP::GeV };
        G4double m_water_mpt_rindices[ m_size_mpt_water ]{ 1.32885, 1.32906, 1.32927, 1.32948, 1.3297 , 1.32992, 1.33014,
                                                           1.33037, 1.3306 , 1.33084, 1.33109, 1.33134, 1.3316 , 1.33186, 1.33213,
                                                           1.33241, 1.3327 , 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
                                                           1.33498, 1.33536, 1.33576, 1.33617, 1.3366 , 1.33705, 1.33753, 1.33803,
                                                           1.33855, 1.33911, 1.3397 , 1.34033, 1.341  , 1.34172, 1.34248, 1.34331,
                                                           1.34419, 1.34515, 1.3462 , 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
                                                           1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
                                                           1.38362, 1.39074, 1.39956, 1.41075, 1.42535 };
        G4double m_water_mpt_absorption[ m_size_mpt_water ]{ 16.1419*CLHEP::cm*m_ABWFF,  18.278*CLHEP::cm*m_ABWFF, 21.0657*CLHEP::cm*m_ABWFF, 24.8568*CLHEP::cm*m_ABWFF, 30.3117*CLHEP::cm*m_ABWFF,
                                                             38.8341*CLHEP::cm*m_ABWFF, 54.0231*CLHEP::cm*m_ABWFF, 81.2306*CLHEP::cm*m_ABWFF, 120.909*CLHEP::cm*m_ABWFF, 160.238*CLHEP::cm*m_ABWFF,
                                                             193.771*CLHEP::cm*m_ABWFF, 215.017*CLHEP::cm*m_ABWFF, 227.747*CLHEP::cm*m_ABWFF,  243.85*CLHEP::cm*m_ABWFF, 294.036*CLHEP::cm*m_ABWFF,
                                                             321.647*CLHEP::cm*m_ABWFF,  342.81*CLHEP::cm*m_ABWFF, 362.827*CLHEP::cm*m_ABWFF, 378.041*CLHEP::cm*m_ABWFF, 449.378*CLHEP::cm*m_ABWFF,
                                                             739.434*CLHEP::cm*m_ABWFF, 1114.23*CLHEP::cm*m_ABWFF, 1435.56*CLHEP::cm*m_ABWFF, 1611.06*CLHEP::cm*m_ABWFF, 1764.18*CLHEP::cm*m_ABWFF,
                                                             2100.95*CLHEP::cm*m_ABWFF,  2292.9*CLHEP::cm*m_ABWFF, 2431.33*CLHEP::cm*m_ABWFF,  3053.6*CLHEP::cm*m_ABWFF, 4838.23*CLHEP::cm*m_ABWFF,
                                                             6539.65*CLHEP::cm*m_ABWFF, 7682.63*CLHEP::cm*m_ABWFF, 9137.28*CLHEP::cm*m_ABWFF, 12220.9*CLHEP::cm*m_ABWFF, 15270.7*CLHEP::cm*m_ABWFF,
                                                             19051.5*CLHEP::cm*m_ABWFF, 23671.3*CLHEP::cm*m_ABWFF, 29191.1*CLHEP::cm*m_ABWFF, 35567.9*CLHEP::cm*m_ABWFF,   42583*CLHEP::cm*m_ABWFF,
                                                             49779.6*CLHEP::cm*m_ABWFF, 56465.3*CLHEP::cm*m_ABWFF,   61830*CLHEP::cm*m_ABWFF, 65174.6*CLHEP::cm*m_ABWFF, 66143.7*CLHEP::cm*m_ABWFF,
                                                             64820*CLHEP::cm*m_ABWFF,   61635*CLHEP::cm*m_ABWFF, 57176.2*CLHEP::cm*m_ABWFF, 52012.1*CLHEP::cm*m_ABWFF, 46595.7*CLHEP::cm*m_ABWFF,
                                                             41242.1*CLHEP::cm*m_ABWFF, 36146.3*CLHEP::cm*m_ABWFF, 31415.4*CLHEP::cm*m_ABWFF, 27097.8*CLHEP::cm*m_ABWFF, 23205.7*CLHEP::cm*m_ABWFF,
                                                             19730.3*CLHEP::cm*m_ABWFF, 16651.6*CLHEP::cm*m_ABWFF, 13943.6*CLHEP::cm*m_ABWFF, 11578.1*CLHEP::cm*m_ABWFF, 9526.13*CLHEP::cm*m_ABWFF };
        G4double m_water_mpt_rayleigh[ m_size_mpt_water ]{ 386929*CLHEP::cm*m_RAYFF,  366249*CLHEP::cm*m_RAYFF,  346398*CLHEP::cm*m_RAYFF,  327355*CLHEP::cm*m_RAYFF,  309097*CLHEP::cm*m_RAYFF,
                                                            291603*CLHEP::cm*m_RAYFF,  274853*CLHEP::cm*m_RAYFF,  258825*CLHEP::cm*m_RAYFF,  243500*CLHEP::cm*m_RAYFF,  228856*CLHEP::cm*m_RAYFF,
                                                            214873*CLHEP::cm*m_RAYFF,  201533*CLHEP::cm*m_RAYFF,  188816*CLHEP::cm*m_RAYFF,  176702*CLHEP::cm*m_RAYFF,  165173*CLHEP::cm*m_RAYFF,
                                                            154210*CLHEP::cm*m_RAYFF,  143795*CLHEP::cm*m_RAYFF,  133910*CLHEP::cm*m_RAYFF,  124537*CLHEP::cm*m_RAYFF,  115659*CLHEP::cm*m_RAYFF,
                                                            107258*CLHEP::cm*m_RAYFF, 99318.2*CLHEP::cm*m_RAYFF, 91822.2*CLHEP::cm*m_RAYFF,   84754*CLHEP::cm*m_RAYFF, 78097.3*CLHEP::cm*m_RAYFF,
                                                            71836.5*CLHEP::cm*m_RAYFF,   65956*CLHEP::cm*m_RAYFF, 60440.6*CLHEP::cm*m_RAYFF, 55275.4*CLHEP::cm*m_RAYFF, 50445.6*CLHEP::cm*m_RAYFF,
                                                            45937*CLHEP::cm*m_RAYFF, 41735.2*CLHEP::cm*m_RAYFF, 37826.6*CLHEP::cm*m_RAYFF, 34197.6*CLHEP::cm*m_RAYFF, 30834.9*CLHEP::cm*m_RAYFF,
                                                            27725.4*CLHEP::cm*m_RAYFF, 24856.6*CLHEP::cm*m_RAYFF, 22215.9*CLHEP::cm*m_RAYFF, 19791.3*CLHEP::cm*m_RAYFF, 17570.9*CLHEP::cm*m_RAYFF,
                                                            15543*CLHEP::cm*m_RAYFF, 13696.6*CLHEP::cm*m_RAYFF, 12020.5*CLHEP::cm*m_RAYFF, 10504.1*CLHEP::cm*m_RAYFF, 9137.15*CLHEP::cm*m_RAYFF,
                                                            7909.45*CLHEP::cm*m_RAYFF,  6811.3*CLHEP::cm*m_RAYFF, 5833.25*CLHEP::cm*m_RAYFF,  4966.2*CLHEP::cm*m_RAYFF, 4201.36*CLHEP::cm*m_RAYFF,
                                                            3530.28*CLHEP::cm*m_RAYFF, 2944.84*CLHEP::cm*m_RAYFF, 2437.28*CLHEP::cm*m_RAYFF, 2000.18*CLHEP::cm*m_RAYFF,  1626.5*CLHEP::cm*m_RAYFF,
                                                            1309.55*CLHEP::cm*m_RAYFF, 1043.03*CLHEP::cm*m_RAYFF, 821.016*CLHEP::cm*m_RAYFF,  637.97*CLHEP::cm*m_RAYFF, 488.754*CLHEP::cm*m_RAYFF };
        G4double m_water_mpt_mie[ m_size_mpt_water ]{ 7790020*CLHEP::cm*m_MIEFF, 7403010*CLHEP::cm*m_MIEFF, 7030610*CLHEP::cm*m_MIEFF, 6672440*CLHEP::cm*m_MIEFF, 6328120*CLHEP::cm*m_MIEFF,
                                                      5997320*CLHEP::cm*m_MIEFF, 5679650*CLHEP::cm*m_MIEFF, 5374770*CLHEP::cm*m_MIEFF, 5082340*CLHEP::cm*m_MIEFF, 4802000*CLHEP::cm*m_MIEFF,
                                                      4533420*CLHEP::cm*m_MIEFF, 4276280*CLHEP::cm*m_MIEFF, 4030220*CLHEP::cm*m_MIEFF, 3794950*CLHEP::cm*m_MIEFF, 3570120*CLHEP::cm*m_MIEFF,
                                                      3355440*CLHEP::cm*m_MIEFF, 3150590*CLHEP::cm*m_MIEFF, 2955270*CLHEP::cm*m_MIEFF, 2769170*CLHEP::cm*m_MIEFF, 2592000*CLHEP::cm*m_MIEFF,
                                                      2423470*CLHEP::cm*m_MIEFF, 2263300*CLHEP::cm*m_MIEFF, 2111200*CLHEP::cm*m_MIEFF, 1966900*CLHEP::cm*m_MIEFF, 1830120*CLHEP::cm*m_MIEFF,
                                                      1700610*CLHEP::cm*m_MIEFF, 1578100*CLHEP::cm*m_MIEFF, 1462320*CLHEP::cm*m_MIEFF, 1353040*CLHEP::cm*m_MIEFF, 1250000*CLHEP::cm*m_MIEFF,
                                                      1152960*CLHEP::cm*m_MIEFF, 1061680*CLHEP::cm*m_MIEFF,  975936*CLHEP::cm*m_MIEFF,  895491*CLHEP::cm*m_MIEFF,  820125*CLHEP::cm*m_MIEFF,
                                                      749619*CLHEP::cm*m_MIEFF,  683760*CLHEP::cm*m_MIEFF,  622339*CLHEP::cm*m_MIEFF,  565152*CLHEP::cm*m_MIEFF,  512000*CLHEP::cm*m_MIEFF,
                                                      462688*CLHEP::cm*m_MIEFF,  417027*CLHEP::cm*m_MIEFF,  374832*CLHEP::cm*m_MIEFF,  335923*CLHEP::cm*m_MIEFF,  300125*CLHEP::cm*m_MIEFF,
                                                      267267*CLHEP::cm*m_MIEFF,  237184*CLHEP::cm*m_MIEFF,  209715*CLHEP::cm*m_MIEFF,  184704*CLHEP::cm*m_MIEFF,  162000*CLHEP::cm*m_MIEFF,
                                                      141456*CLHEP::cm*m_MIEFF,  122931*CLHEP::cm*m_MIEFF,  106288*CLHEP::cm*m_MIEFF, 91395.2*CLHEP::cm*m_MIEFF,   78125*CLHEP::cm*m_MIEFF, 
                                                      66355.2*CLHEP::cm*m_MIEFF, 55968.2*CLHEP::cm*m_MIEFF, 46851.2*CLHEP::cm*m_MIEFF, 38896.2*CLHEP::cm*m_MIEFF,   32000*CLHEP::cm*m_MIEFF };
        G4double m_water_basic_mpt_energies[ 4 ]{ 2.00*eV, 2.87*eV, 2.90*eV, 3.47*eV };
        G4double m_water_basic_mpt_rindices[ 4 ]{ 1.333, 1.333, 1.333, 1.333 };

        // From Geant4's LXe example
        vector< G4double > lxe_Energy = { 7.0 * eV, 7.07 * eV, 7.14 * eV };
        vector< G4double > lxe_SCINT = { 0.1, 1.0, 0.1 };
        vector< G4double > lxe_RIND  = { 1.59, 1.57, 1.54 };
        vector< G4double > lxe_ABSL  = { 35. * cm, 35. * cm, 35. * cm };

        static const int m_MRDScintilator_mpt_size{ 4 };
        G4double m_MRDScintilator_mpt_energies[ m_MRDScintilator_mpt_size ]{ 2.00*eV, 2.87*eV, 2.90*eV, 3.47*eV };
        G4double m_MRDScintilator_mpt_rindicies[ m_MRDScintilator_mpt_size ]{ 1.5, 1.5, 1.5, 1.5 };
        G4double m_MRDScintilator_mpt_absorption[ m_MRDScintilator_mpt_size ]{ 124.*cm, 124.*cm, 124.*cm, 124.*cm };
        G4double m_MRDScintilator_mpt_scintFast[ m_MRDScintilator_mpt_size ]{ 0.00, 0.00, 1.00, 1.00 };
        G4double m_MRDScintilator_mpt_scintYeild{ 10.0/keV };
        G4double m_MRDScintilator_mpt_resScale{ 1.0 };
        G4double m_MRDScintilator_mpt_fastTimeConst{ 10.0*ns };
        G4double m_MRDScintilator_mpt_birksConstant{ 0.126*mm/MeV };

        vector< G4double > SNPH2_AbsLength = { 420. * cm, 420. * cm, 420. * cm };
        vector< G4double > SNPH2_RIND      = { 1.98     , 1.98     , 1.98      };

        G4double inf{ 1.e25 };
        G4double zero{ 1.e-25 };
        G4double infs [m_size_mpt_water];
        G4double zeros[m_size_mpt_water];
};

#endif

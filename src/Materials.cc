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

#include "Materials.hh"

Materials* Materials::m_instance{ nullptr };

Materials::Materials() {
    constructMaterials();
}

Materials* Materials::get_instance() {
    if( !m_instance ) {
        m_instance = new Materials();
    }
    return m_instance;
}

void Materials::delete_instance() {
    if( m_instance ) {
        delete m_instance;
        m_instance = nullptr;
    }
}

void Materials::print_materials() {
    G4cout << "Attemting to use materials:" << G4endl
           << "  world_medium--------: " << m_constructionMessenger->get_world_material              () << G4endl
           << "  detector_wall-------: " << m_constructionMessenger->get_detector_wall_material      () << G4endl
           << "  calorimeter---------: " << m_constructionMessenger->get_calorimeter_material        () << G4endl
           << "  detector_medium-----: " << m_constructionMessenger->get_detector_medium_material    () << G4endl
        //    << "  lens----------------: " << m_constructionMessenger->get_lens_material               () << G4endl
           << "  photoSensor_surface-: " << m_constructionMessenger->get_photoSensor_surface_material() << G4endl;
}

void Materials::constructMaterials() {
    constructMaterial_G4Water            ();
    constructMaterial_G4Air              ();
    constructMaterial_G4StainlessSteel   ();
    constructMaterial_G4Vacuum           ();
    constructMaterial_LXe                ();
    constructMaterial_ANNIETankSteel     ();
    constructMaterial_ANNIEMRDIron       ();
    constructMaterial_ANNIEMRDScintilator();
    constructMaterial_ANNIEWater_unDoped ();
    constructMaterial_ANNIEWater_GdDoped ();
    constructMaterial_aluminum           ();
    constructMaterial_SNPH2              ();
    constructMaterial_nonInteractive     ();
    constructMaterial_veryInteractive    ();
}

void Materials::constructMaterial_ANNIEWater_unDoped() {
    m_material_ANNIEWater_unDoped = new G4Material( "ANNIEWater_unDoped", 1.00*CLHEP::g/CLHEP::cm3, 2 );
    m_material_ANNIEWater_unDoped->AddElement( m_element_H, 2 );
    m_material_ANNIEWater_unDoped->AddElement( m_element_O, 1 );

    if( !m_materialPropertiesTable_ANNIEWater->ConstPropertyExists( "RINDEX" ) ) 
        m_materialPropertiesTable_ANNIEWater->AddProperty( "RINDEX"   , m_water_mpt_energies, m_water_mpt_rindices  , m_size_mpt_water );
    if( !m_materialPropertiesTable_ANNIEWater->ConstPropertyExists( "ABSLENGTH" ) ) 
        m_materialPropertiesTable_ANNIEWater->AddProperty( "ABSLENGTH", m_water_mpt_energies, m_water_mpt_absorption, m_size_mpt_water );
    if( !m_materialPropertiesTable_ANNIEWater->ConstPropertyExists( "RAYLEIGH" ) ) 
        m_materialPropertiesTable_ANNIEWater->AddProperty( "RAYLEIGH" , m_water_mpt_energies, m_water_mpt_rayleigh  , m_size_mpt_water );
    m_material_ANNIEWater_unDoped->SetMaterialPropertiesTable( m_materialPropertiesTable_ANNIEWater );
}

void Materials::constructMaterial_G4Water() {
    m_material_G4Water = m_nist->FindOrBuildMaterial( "G4_WATER" );
    m_material_G4Water->SetName( "G4Water" );

    m_materialPropertiesTable_G4Water->AddProperty( "RINDEX", m_water_mpt_energies, m_water_mpt_rindices, 4 );
    m_material_G4Water->SetMaterialPropertiesTable( m_materialPropertiesTable_G4Water );
}

void Materials::constructMaterial_G4Air() {
    m_material_G4Air = m_nist->FindOrBuildMaterial( "G4_AIR" );
    m_material_G4Air->SetName( "G4Air" );
}

void Materials::constructMaterial_G4StainlessSteel() {
    m_material_G4Water = m_nist->FindOrBuildMaterial( "G4_STAINLESS-STEEL" );
    m_material_G4Water->SetName( "G4StainlessSteel" );
}

void Materials::constructMaterial_ANNIEWater_GdDoped() {
    m_element_Gd_complete->AddIsotope( m_isotope_Gd152,  0.2 * perCent );
    m_element_Gd_complete->AddIsotope( m_isotope_Gd154,  2.2 * perCent );
    m_element_Gd_complete->AddIsotope( m_isotope_Gd155, 14.9 * perCent );
    m_element_Gd_complete->AddIsotope( m_isotope_Gd156, 20.6 * perCent );
    m_element_Gd_complete->AddIsotope( m_isotope_Gd157, 15.7 * perCent );
    m_element_Gd_complete->AddIsotope( m_isotope_Gd158, 24.7 * perCent );
    m_element_Gd_complete->AddIsotope( m_isotope_Gd160, 21.7 * perCent );

    m_material_ANNIEWater_GdDoped = new G4Material( "ANNIEWater_GdDoped", 1.00 * CLHEP::g / CLHEP::cm3, 2 );
    m_material_ANNIEWater_GdDoped->AddMaterial( m_material_ANNIEWater_unDoped, 99.9 * CLHEP::perCent );
    m_material_ANNIEWater_GdDoped->AddElement ( m_element_Gd, 0.1  * CLHEP::perCent );
    
    if( !m_materialPropertiesTable_ANNIEWater->ConstPropertyExists( "RINDEX" ) ) 
        m_materialPropertiesTable_ANNIEWater->AddProperty( "RINDEX"   , m_water_mpt_energies, m_water_mpt_rindices  , m_size_mpt_water );
    if( !m_materialPropertiesTable_ANNIEWater->ConstPropertyExists( "ABSLENGTH" ) ) 
        m_materialPropertiesTable_ANNIEWater->AddProperty( "ABSLENGTH", m_water_mpt_energies, m_water_mpt_absorption, m_size_mpt_water );
    if( !m_materialPropertiesTable_ANNIEWater->ConstPropertyExists( "RAYLEIGH" ) ) 
        m_materialPropertiesTable_ANNIEWater->AddProperty( "RAYLEIGH" , m_water_mpt_energies, m_water_mpt_rayleigh  , m_size_mpt_water );
    m_material_ANNIEWater_GdDoped->SetMaterialPropertiesTable( m_materialPropertiesTable_ANNIEWater );
}

void Materials::constructMaterial_ANNIETankSteel() {
    m_material_ANNIETankSteel = new G4Material( "ANNIETankSteel", 7.81 * CLHEP::g / CLHEP::cm3, 8 ); // WCSim "StainlessSteel"
    m_material_ANNIETankSteel->AddElement( m_element_Fe, 70.44 * CLHEP::perCent );
    m_material_ANNIETankSteel->AddElement( m_element_Cr, 18    * CLHEP::perCent );
    m_material_ANNIETankSteel->AddElement( m_element_C , 0.08  * CLHEP::perCent );
    m_material_ANNIETankSteel->AddElement( m_element_Ni, 8     * CLHEP::perCent ); 
    m_material_ANNIETankSteel->AddElement( m_element_P , 0.45  * CLHEP::perCent );
    m_material_ANNIETankSteel->AddElement( m_element_Si, 1     * CLHEP::perCent );
    m_material_ANNIETankSteel->AddElement( m_element_Mn, 2     * CLHEP::perCent );
    m_material_ANNIETankSteel->AddElement( m_element_S , 0.03  * CLHEP::perCent );
}

void Materials::constructMaterial_aluminum() {
  m_material_aluminum = new G4Material( "aluminum", 2.7 * CLHEP::g / CLHEP::cm3, 1 );
  m_material_aluminum->AddElement( m_element_Al, 1 );
}

void Materials::constructMaterial_ANNIEMRDScintilator() {
    m_material_ANNIEMRDScintilator = new G4Material( "ANNIEMRDScintilator", 1.032 * CLHEP::g / CLHEP::cm3, 2 ); // WCSim "scinti"
    m_material_ANNIEMRDScintilator->AddElement( m_element_C, 8 );
    m_material_ANNIEMRDScintilator->AddElement( m_element_H, 8 );

    m_materialPropertiesTable_ANNIEMRDScintilator->AddProperty     ( "RINDEX"            , m_MRDScintilator_mpt_energies, m_MRDScintilator_mpt_rindicies , m_MRDScintilator_mpt_size );
    m_materialPropertiesTable_ANNIEMRDScintilator->AddProperty     ( "ABSLENGTH"         , m_MRDScintilator_mpt_energies, m_MRDScintilator_mpt_absorption, m_MRDScintilator_mpt_size );
    // m_materialPropertiesTable_ANNIEMRDScintilator->AddProperty     ( "FASTCOMPONENT"     , m_MRDScintilator_mpt_energies, m_MRDScintilator_mpt_scintFast , m_MRDScintilator_mpt_size );
    m_materialPropertiesTable_ANNIEMRDScintilator->AddConstProperty( "SCINTILLATIONYIELD", m_MRDScintilator_mpt_scintYeild    );
    m_materialPropertiesTable_ANNIEMRDScintilator->AddConstProperty( "RESOLUTIONSCALE"   , m_MRDScintilator_mpt_resScale      );
    // m_materialPropertiesTable_ANNIEMRDScintilator->AddConstProperty( "FASTTIMECONSTANT"  , m_MRDScintilator_mpt_fastTimeConst );
    m_material_ANNIEMRDScintilator->SetMaterialPropertiesTable( m_materialPropertiesTable_ANNIEMRDScintilator );
    m_material_ANNIEMRDScintilator->GetIonisation()->SetBirksConstant( m_MRDScintilator_mpt_birksConstant );
}

void Materials::constructMaterial_ANNIEMRDIron() {
    m_material_ANNIEMRDIron = new G4Material( "ANNIEMRDIron", 7.8 * CLHEP::g / CLHEP::cm3, 2 );
    m_material_ANNIEMRDIron->AddElement( m_element_C , 1.  * CLHEP::perCent );
    m_material_ANNIEMRDIron->AddElement( m_element_Fe, 99. * CLHEP::perCent );
}

void Materials::constructMaterial_LXe() {
    G4double a;  // atomic mass
    G4double z;  // atomic number
    G4double density;

    m_material_LXe = new G4Material( "LXe", z = 54., a = 131.29 * g / mole, density = 3.020 * g / cm3 );

    m_materialPropertiesTable_LXe->AddProperty( "SCINTILLATIONCOMPONENT1", lxe_Energy, lxe_SCINT );
    m_materialPropertiesTable_LXe->AddProperty( "SCINTILLATIONCOMPONENT2", lxe_Energy, lxe_SCINT );
    m_materialPropertiesTable_LXe->AddProperty( "RINDEX"                 , lxe_Energy, lxe_RIND  );
    m_materialPropertiesTable_LXe->AddProperty( "ABSLENGTH"              , lxe_Energy, lxe_ABSL  );
    m_materialPropertiesTable_LXe->AddConstProperty( "SCINTILLATIONYIELD"        , 12000. / MeV );
    m_materialPropertiesTable_LXe->AddConstProperty( "RESOLUTIONSCALE"           , 1.0          );
    m_materialPropertiesTable_LXe->AddConstProperty( "SCINTILLATIONTIMECONSTANT1", 20. * ns     );
    m_materialPropertiesTable_LXe->AddConstProperty( "SCINTILLATIONTIMECONSTANT2", 45. * ns     );
    m_materialPropertiesTable_LXe->AddConstProperty( "SCINTILLATIONYIELD1"       , 1.0          );
    m_materialPropertiesTable_LXe->AddConstProperty( "SCINTILLATIONYIELD2"       , 0.0          );

    m_material_LXe->SetMaterialPropertiesTable( m_materialPropertiesTable_LXe );
    m_material_LXe->GetIonisation()->SetBirksConstant( 0.126 * mm / MeV );
}

void Materials::constructMaterial_SNPH2() {
    m_material_SNPH2 = new G4Material( "SNPH2", 0.0000899 * CLHEP::g / CLHEP::cm3, 2 );
    m_material_SNPH2->AddElement( m_element_H, 2 );
    m_material_SNPH2->AddElement( m_element_C, 1 );

    m_materialPropertiesTable_SNPH2->AddProperty( "ABSLENGTH", lxe_Energy, SNPH2_AbsLength );
    m_materialPropertiesTable_SNPH2->AddProperty( "RINDEX"   , lxe_Energy, SNPH2_RIND      );
    m_material_SNPH2->SetMaterialPropertiesTable( m_materialPropertiesTable_SNPH2 );
}

void Materials::constructMaterial_G4Vacuum() {
    m_material_G4Vacuum = new G4Material( "G4Vacuum", 1.0, 1.01 * CLHEP::g / CLHEP::mole, 1.e-25 * CLHEP::g / CLHEP::cm3, kStateGas, 2.73 * CLHEP::kelvin, 3.e-18 * CLHEP::pascal );
}

void Materials::constructMaterial_nonInteractive() {
    m_material_nonInteractive = new G4Material( "nonInteractive", 1, 1, zero, kStateGas, inf, zero );

    std::fill_n( infs, m_size_mpt_water, inf );
    m_materialPropertiesTable_nonInteractive->AddProperty( "RINDEX"   , lxe_Energy          , lxe_RIND, 3                );
    m_materialPropertiesTable_nonInteractive->AddProperty( "ABSLENGTH", m_water_mpt_energies, infs    , m_size_mpt_water );
    m_materialPropertiesTable_nonInteractive->AddProperty( "RAYLEIGH" , m_water_mpt_energies, infs    , m_size_mpt_water );
    m_material_nonInteractive->SetMaterialPropertiesTable( m_materialPropertiesTable_nonInteractive );
}

void Materials::constructMaterial_veryInteractive() {
    m_material_veryInteractive = new G4Material( "veryInteractive", 1, 1, inf, kStateGas, zero, inf );

    std::fill_n( zeros, m_size_mpt_water, zero );
    m_materialPropertiesTable_veryInteractive->AddProperty( "RINDEX"   , lxe_Energy          , lxe_RIND, 3                );
    m_materialPropertiesTable_veryInteractive->AddProperty( "ABSLENGTH", m_water_mpt_energies, zeros   , m_size_mpt_water );
    m_materialPropertiesTable_veryInteractive->AddProperty( "RAYLEIGH" , m_water_mpt_energies, zeros   , m_size_mpt_water );
    m_material_veryInteractive->SetMaterialPropertiesTable( m_materialPropertiesTable_veryInteractive );
}
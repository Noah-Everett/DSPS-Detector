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

#include "Calorimeter.hh"

Calorimeter::Calorimeter( G4String t_name, G4String t_index, G4ThreeVector t_size ) {
    m_name = t_name + "_" + t_index;

    m_calorimeter = new GeometricObjectBox();

    if( t_size == G4ThreeVector() )
        t_size = G4ThreeVector( get_width(), get_height(), get_depth() );

    m_calorimeter->set_solid            ( new G4Box( m_name + "_G4Box", t_size.x()/2, t_size.y()/2, t_size.z()/2 ) );
    m_calorimeter->set_material         ( m_constructionMessenger->get_calorimeter_material                         () );
    m_calorimeter->set_visAttributes    ( m_constructionMessenger->get_calorimeter_visAttributes                    () );
    m_calorimeter->set_visibility       ( m_constructionMessenger->get_calorimeter_visibility                       () );
    m_calorimeter->make_logicalVolume   (                                                                              );
}

Calorimeter::~Calorimeter() {
    if( m_calorimeter ) delete m_calorimeter;
}

void Calorimeter::place( G4RotationMatrix* t_rotationMatrix, G4ThreeVector t_translationVector, G4LogicalVolume* t_parentLogicalVolume, G4bool t_isMany ) {
    m_position = t_translationVector;
    m_rotationMatrix = t_rotationMatrix;

    m_calorimeter->place( t_rotationMatrix, t_translationVector, t_parentLogicalVolume, t_isMany );
}

G4ThreeVector Calorimeter::get_size() {
    return G4ThreeVector( get_width(), get_height(), get_depth() );
}

G4double Calorimeter::get_width() {
    return ConstructionMessenger::get_instance()->get_calorimeter_size_width();
}

G4double Calorimeter::get_height() {
    return ConstructionMessenger::get_instance()->get_calorimeter_size_height();
}

G4double Calorimeter::get_depth() {
    return ConstructionMessenger::get_instance()->get_calorimeter_size_depth();
}

G4String Calorimeter::get_name() {
    return m_name;
}

G4LogicalVolume* Calorimeter::get_logicalVolume() {
    return m_calorimeter->get_logicalVolume();
}

void Calorimeter::set_sensitiveDetector( CalorimeterSensitiveDetector* t_calorimeterSensitiveDetector ) {
    m_calorimeterSensitiveDetector = t_calorimeterSensitiveDetector;
    m_calorimeter->set_sensitiveDetector( m_calorimeterSensitiveDetector );
}

CalorimeterSensitiveDetector* Calorimeter::get_sensitiveDetector() {
    return m_calorimeterSensitiveDetector;
}

G4ThreeVector Calorimeter::get_position() {
    return m_position;
}

G4RotationMatrix* Calorimeter::get_rotationMatrix() {
    return m_rotationMatrix;
}
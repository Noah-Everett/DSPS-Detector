//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "PhotoSensor.hh"

PhotoSensor::PhotoSensor( G4String t_name ) {
    m_name = t_name;

    m_surface->set_material( m_constructionMessenger->get_photoSensor_surface_material() );
    m_body   ->set_material( m_constructionMessenger->get_photoSensor_body_material   () );

    G4ThreeVector surface_size = m_constructionMessenger->get_photoSensor_surface_size();
    G4ThreeVector body_size    = m_constructionMessenger->get_photoSensor_body_size   ();
    m_surface->set_solid( new G4Box( t_name + "_surface", surface_size.x()/2, surface_size.y()/2, surface_size.z()/2 ) );
    m_body   ->set_solid( new G4Box( t_name + "_body"   , body_size   .x()/2, body_size   .y()/2, body_size   .z()/2 ) );
    G4cout << G4endl;

    m_surface->set_visAttributes( m_constructionMessenger->get_photoSensor_surface_visAttributes() );
    m_body   ->set_visAttributes( m_constructionMessenger->get_photoSensor_body_visAttributes   () );

    m_surface->make_logicalVolume();
    m_body   ->make_logicalVolume();
}

PhotoSensor::~PhotoSensor() {
    if( m_surface ) delete m_surface;
    if( m_body    ) delete m_body;
}

void PhotoSensor::place( G4RotationMatrix* t_rotationMatrix     , 
                         G4ThreeVector     t_translation        , 
                         G4LogicalVolume * t_motherLogicalVolume, 
                         G4bool            t_isMany              ) {
    G4double surface_z = m_surface->get_solid()->GetZHalfLength();
    G4double body_z    = m_body   ->get_solid()->GetZHalfLength();
    G4double total_z   = surface_z * 2 + body_z * 2;
    G4ThreeVector translation_surface( 0, 0,  2 * body_z + surface_z );
    G4ThreeVector translation_body   ( 0, 0,  body_z                 );
    translation_surface = *t_rotationMatrix * translation_surface;
    translation_body    = *t_rotationMatrix * translation_body;
    m_surface->place( t_rotationMatrix, t_translation - translation_surface, t_motherLogicalVolume, t_isMany );
    m_body   ->place( t_rotationMatrix, t_translation - translation_body   , t_motherLogicalVolume, t_isMany );
}

G4String PhotoSensor::get_name() {
    return m_name;
}

GeometricObjectBox* PhotoSensor::get_surface() {
    return m_surface;
}

GeometricObjectBox* PhotoSensor::get_body() {
    return m_body;
}

PhotoSensorSensitiveDetector* PhotoSensor::get_sensitiveDetector() {
    return m_sensitiveDetector;
}

void PhotoSensor::set_sensitiveDetector( PhotoSensorSensitiveDetector* t_sensitiveDetector ) {
    m_sensitiveDetector = t_sensitiveDetector;
    m_surface->set_sensitiveDetector( t_sensitiveDetector );
}

void PhotoSensor::set_name( const G4String& t_name ) {
    m_name = t_name;
}

// G4ThreeVector PhotoSensor::get_position( const char* t_relativePosition ) {
//     G4String relativePosition = G4String( t_relativePosition );
//     to_lower( relativePosition );
//     if( relativePosition == "front" || relativePosition == "f" )
//         return get_position_front();
//     else if( relativePosition == "back" || relativePosition == "b" )
//         return get_position_back();
//     else if( relativePosition == "center" || relativePosition == "c" )
//         return get_position_center();
//     else
//         G4Exception( "PhotoSensor::get_position()",
//                      "InvalidArgument"                               ,
//                      FatalErrorInArgument                            ,
//                      "Invalid relative position string. Valid positions are: "
//                      "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
//     return G4ThreeVector();
// }

// G4ThreeVector PhotoSensor::get_position_front() {
//     return m_position + G4ThreeVector( 0, 0,  2 * m_surface->get_solid()->GetZHalfLength() 
//                                             + 2 * m_body   ->get_solid()->GetZHalfLength() );
// }

// G4ThreeVector PhotoSensor::get_position_back() {
//     return m_position;
// }

// G4ThreeVector PhotoSensor::get_position_center() {
//     return m_position + G4ThreeVector( 0, 0,  m_surface->get_solid()->GetZHalfLength() 
//                                             + m_body   ->get_solid()->GetZHalfLength() );
// }

// G4ThreeVector PhotoSensor::get_position_front( G4RotationMatrix* t_rotationMatrix  , 
//                                                G4ThreeVector     t_position        , 
//                                                const char      * t_relativePosition ) {
//     G4String relativePosition = G4String( t_relativePosition );
//     to_lower( relativePosition );
//     if( relativePosition == "front" || relativePosition == "f" )
//         return t_position;
//     else if( relativePosition == "center" || relativePosition == "c" )
//         return t_position + G4ThreeVector( 0, 0,  m_surface->get_solid()->GetZHalfLength() 
//                                                 + m_body   ->get_solid()->GetZHalfLength() );
//     else if( relativePosition == "back" || relativePosition == "b" )
//         return t_position + G4ThreeVector( 0, 0,  2 * m_surface->get_solid()->GetZHalfLength() 
//                                                 + 2 * m_body   ->get_solid()->GetZHalfLength() );
//     else
//         G4Exception( "PhotoSensor::set_position()",
//                      "InvalidArgument"                               ,
//                      FatalErrorInArgument                            ,
//                      "Invalid relative position string. Valid positions are: "
//                      "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
//     return G4ThreeVector();
// }

// G4ThreeVector PhotoSensor::get_position_center( G4RotationMatrix* t_rotationMatrix  , 
//                                                 G4ThreeVector     t_position        , 
//                                                 const char      * t_relativePosition ) {
//     G4String relativePosition = G4String( t_relativePosition );
//     to_lower( relativePosition );
//     if( relativePosition == "front" || relativePosition == "f" )
//         return t_position - G4ThreeVector( 0, 0,  m_surface->get_solid()->GetZHalfLength() 
//                                                 + m_body   ->get_solid()->GetZHalfLength() );
//     else if( relativePosition == "center" || relativePosition == "c" )
//         return t_position;
//     else if( relativePosition == "back" || relativePosition == "b" )
//         return t_position + G4ThreeVector( 0, 0,  m_surface->get_solid()->GetZHalfLength() 
//                                                 + m_body   ->get_solid()->GetZHalfLength() );
//     else
//         G4Exception( "PhotoSensor::set_position()",
//                      "InvalidArgument"                               ,
//                      FatalErrorInArgument                            ,
//                      "Invalid relative position string. Valid positions are: "
//                      "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
//     return G4ThreeVector();
// }

// G4ThreeVector PhotoSensor::get_position_back( G4RotationMatrix* t_rotationMatrix  , 
//                                               G4ThreeVector     t_position        , 
//                                               const char      * t_relativePosition ) {
//     G4String relativePosition = G4String( t_relativePosition );
//     to_lower( relativePosition );
//     if( relativePosition == "front" || relativePosition == "f" )
//         return t_position - G4ThreeVector( 0, 0,  2 * m_surface->get_solid()->GetZHalfLength() 
//                                                 + 2 * m_body   ->get_solid()->GetZHalfLength() );
//     else if( relativePosition == "center" || relativePosition == "c" )
//         return t_position - G4ThreeVector( 0, 0,  m_surface->get_solid()->GetZHalfLength() 
//                                                 + m_body   ->get_solid()->GetZHalfLength() );
//     else if( relativePosition == "back" || relativePosition == "b" )
//         return t_position;
//     else
//         G4Exception( "PhotoSensor::set_position()",
//                      "InvalidArgument"                               ,
//                      FatalErrorInArgument                            ,
//                      "Invalid relative position string. Valid positions are: "
//                      "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
//     return G4ThreeVector();
// }

// G4ThreeVector PhotoSensor::get_position( const char      * t_relativePosition_want ,
//                                          G4RotationMatrix* t_rotationMatrix        ,
//                                          G4ThreeVector     t_position              ,
//                                          const char      * t_relativePosition_given ) {
//     G4String relativePosition_want  = G4String( t_relativePosition_want  );
//     to_lower( relativePosition_want  );
//     if( relativePosition_want == "front" || relativePosition == "f" )
//         return get_position_front( t_rotationMatrix, t_position, t_relativePosition_given );
//     else if( relativePosition_want == "center" || relativePosition == "c" )
//         return get_position_center( t_rotationMatrix, t_position, t_relativePosition_given );
//     else if( relativePosition_want == "back" || relativePosition == "b" )
//         return get_position_back( t_rotationMatrix, t_position, t_relativePosition_given );
//     else
//         G4Exception( "PhotoSensor::set_position()",
//                      "InvalidArgument"                               ,
//                      FatalErrorInArgument                            ,
//                      "Invalid relative position string. Valid positions are: "
//                      "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
//     return G4ThreeVector();
// }

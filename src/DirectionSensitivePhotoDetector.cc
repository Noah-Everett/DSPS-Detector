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

#include "DirectionSensitivePhotoDetector.hh"

DirectionSensitivePhotoDetector::DirectionSensitivePhotoDetector( G4String t_name ) {
    m_lensSystem  = new LensSystem ( t_name + "_lensSystem" , true );
    m_photoSensor = new PhotoSensor( t_name + "_photoSensor"       );
}

DirectionSensitivePhotoDetector::~DirectionSensitivePhotoDetector() {
    if( m_photoSensor ) delete m_photoSensor;
    if( m_lensSystem  ) delete m_lensSystem;
}

void DirectionSensitivePhotoDetector::place( G4RotationMatrix* t_rotationMatrix, G4ThreeVector t_translationVector, G4LogicalVolume* t_parentLogicalVolume, G4bool t_isMany ) {
    m_photoSensor->place( t_rotationMatrix, t_translationVector, t_parentLogicalVolume, t_isMany );
    m_lensSystem ->place( t_rotationMatrix, t_translationVector, t_parentLogicalVolume, t_isMany );
}

G4ThreeVector DirectionSensitivePhotoDetector::get_size() {
    return G4ThreeVector( get_width(), get_height(), get_depth() );
}

G4double DirectionSensitivePhotoDetector::get_width() {
    return ConstructionMessenger::get_instance()->get_photoSensor_surface_size_width();
}

G4double DirectionSensitivePhotoDetector::get_height() {
    return ConstructionMessenger::get_instance()->get_photoSensor_surface_size_height();
}

G4double DirectionSensitivePhotoDetector::get_depth() {
    return ConstructionMessenger::get_instance()->get_photoSensor_surface_size_depth()
         + ConstructionMessenger::get_instance()->get_photoSensor_body_size_depth();
}
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

DirectionSensitivePhotoDetector::DirectionSensitivePhotoDetector() {
    m_size = m_constructionMessenger->get_photoSensor_surface_size();
    m_size.setZ( m_constructionMessenger->get_photoSensor_surface_size().z() + m_constructionMessenger->get_photoSensor_body_size().z() );
}

DirectionSensitivePhotoDetector::~DirectionSensitivePhotoDetector() {
    if( m_photoSensor ) delete m_photoSensor;
    if( m_lensSystem  ) delete m_lensSystem;
    if( m_parser      ) delete m_parser;
}

void DirectionSensitivePhotoDetector::place( G4RotationMatrix* t_rotationMatrix, G4ThreeVector t_translationVector, G4LogicalVolume* t_parentLogicalVolume, G4bool t_isMany ) {
    m_photoSensor->place( t_rotationMatrix, t_translationVector, t_parentLogicalVolume, t_isMany );
    m_lensSystem ->place( t_rotationMatrix, t_translationVector, t_parentLogicalVolume, t_isMany );
}

G4ThreeVector DirectionSensitivePhotoDetector::get_size() const {
    return m_size;
}
G4double DirectionSensitivePhotoDetector::get_width() const {
    return m_size.x();
}
G4double DirectionSensitivePhotoDetector::get_height() const {
    return m_size.y();
}
G4double DirectionSensitivePhotoDetector::get_depth() const {
    return m_size.z();
}
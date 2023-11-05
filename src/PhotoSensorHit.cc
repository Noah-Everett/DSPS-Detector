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

#include "PhotoSensorHit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator< PhotoSensorHit >* PhotoSensorHitAllocator;

PhotoSensorHit::PhotoSensorHit() {
}

PhotoSensorHit::PhotoSensorHit( const G4ThreeVector    & t_photoSensor_position      ,
                                const G4String         & t_photoSensor_name          ,
                                      G4RotationMatrix*  t_photoSensor_rotationMatrix,
                                const G4ThreeVector    & t_hit_position              ,
                                const G4double         & t_hit_time                  ,
                                const G4double         & t_hit_energy                ,
                                const G4double         & t_particle_energy           ,
                                const G4ThreeVector    & t_hit_momentum              ,
                                const G4ThreeVector    & t_particle_momentum          ) {
    m_photoSensor_position       = t_photoSensor_position      ;
    m_photoSensor_rotationMatrix = t_photoSensor_rotationMatrix;
    m_photoSensor_name           = t_photoSensor_name          ;
    m_hit_position               = t_hit_position              ;
    m_hit_time                   = t_hit_time                  ;
    m_hit_energy                 = t_hit_energy                ;
    m_hit_momentum               = t_hit_momentum              ;
    m_particle_energy            = t_particle_energy           ;
    m_particle_momentum          = t_particle_momentum         ;
}

PhotoSensorHit::PhotoSensorHit( const PhotoSensorHit& t_hit ) {
    m_photoSensor_position       = t_hit.m_photoSensor_position      ;
    m_photoSensor_rotationMatrix = t_hit.m_photoSensor_rotationMatrix;
    m_photoSensor_name           = t_hit.m_photoSensor_name          ;
    m_hit_position               = t_hit.m_hit_position              ;
    m_hit_time                   = t_hit.m_hit_time                  ;
    m_hit_energy                 = t_hit.m_hit_energy                ;
    m_hit_momentum               = t_hit.m_hit_momentum              ;
    m_particle_energy            = t_hit.m_particle_energy           ;
    m_particle_momentum          = t_hit.m_particle_momentum         ;
}

void PhotoSensorHit::Draw() {
    auto visManager = G4VVisManager::GetConcreteInstance();
    if( !visManager ) 
        return;

    G4Circle circle( m_hit_position );
    circle.SetScreenSize( 2 );
    circle.SetFillStyle( G4Circle::filled );
    G4Colour colour( 1.,1.,0. );
    G4VisAttributes attribs( colour );
    circle.SetVisAttributes( attribs );
    visManager->Draw( circle );
}

void PhotoSensorHit::Print() {
    G4cout << *this << G4endl;
}

std::ostream& operator<<( std::ostream& t_os, const PhotoSensorHit& t_photoSensorHit ) {
    t_os << "[" << "photoSensor_position="       <<  t_photoSensorHit.m_photoSensor_position       << ", \n"
                << "photoSensor_name="           <<  t_photoSensorHit.m_photoSensor_name           << ", \n"
                << "photoSensor_rotationMatrix=" << *t_photoSensorHit.m_photoSensor_rotationMatrix << ", \n"
                << "hit_position="               <<  t_photoSensorHit.m_hit_position               << ", \n"
                << "hit_time="                   <<  t_photoSensorHit.m_hit_time                   << ", \n"
                << "hit_energy="                 <<  t_photoSensorHit.m_hit_energy                 << ", \n"
                << "hit_momentum="               <<  t_photoSensorHit.m_hit_momentum               << ", \n"
                << "particle_energy="            <<  t_photoSensorHit.m_particle_energy            << ", \n"
                << "particle_momentum="          <<  t_photoSensorHit.m_particle_momentum          << "]";

    return t_os;
}

std::ostream& operator<<( std::ostream& t_os, const PhotoSensorHit* t_photoSensorHit ) {
    t_os << *t_photoSensorHit;

    return t_os;
}

void PhotoSensorHit::set_hit_position( G4ThreeVector t_hit_position ) {
    m_hit_position = t_hit_position;
}

void PhotoSensorHit::set_photoSensor_position( G4ThreeVector t_photoSensor_position ) {
    m_photoSensor_position = t_photoSensor_position;
}

void PhotoSensorHit::set_photoSensor_rotationMatrix( G4RotationMatrix* t_photoSensor_rotationMatrix ) {
    m_photoSensor_rotationMatrix = t_photoSensor_rotationMatrix;
}

void PhotoSensorHit::set_photoSensor_name( G4String t_photoSensor_name ) {
    m_photoSensor_name = t_photoSensor_name;
}

void PhotoSensorHit::set_hit_time( G4double t_hit_time ) {
    m_hit_time = t_hit_time;
}   

void PhotoSensorHit::set_hit_energy( G4double t_hit_energy ) {
    m_hit_energy = t_hit_energy;
}

void PhotoSensorHit::set_particle_energy( G4double t_particle_energy ) {
    m_particle_energy = t_particle_energy;
}

void PhotoSensorHit::set_hit_momentum( G4ThreeVector t_hit_momentum ) {
    m_hit_momentum = t_hit_momentum;
}

void PhotoSensorHit::set_particle_momentum( G4ThreeVector t_particle_momentum ) {
    m_particle_momentum = t_particle_momentum;
}

G4ThreeVector PhotoSensorHit::get_hit_position() {
    return m_hit_position;
}

G4ThreeVector PhotoSensorHit::get_photoSensor_position() {
    return m_photoSensor_position;
}

G4RotationMatrix* PhotoSensorHit::get_photoSensor_rotationMatrix() {
    return m_photoSensor_rotationMatrix;
}

G4String PhotoSensorHit::get_photoSensor_name() {
    return m_photoSensor_name;
}

G4double PhotoSensorHit::get_hit_time() {
    return m_hit_time;
}

G4double PhotoSensorHit::get_hit_energy() {
    return m_hit_energy;
}

G4double PhotoSensorHit::get_particle_energy() {
    return m_particle_energy;
}

G4ThreeVector PhotoSensorHit::get_hit_momentum() {
    return m_hit_momentum;
}

G4ThreeVector PhotoSensorHit::get_particle_momentum() {
    return m_particle_momentum;
}
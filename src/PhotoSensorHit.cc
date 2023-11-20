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

#include "PhotoSensorHit.hh"

G4ThreadLocal G4Allocator< PhotoSensorHit >* PhotoSensorHitAllocator;

PhotoSensorHit::PhotoSensorHit() {
}

PhotoSensorHit::PhotoSensorHit( const G4ThreeVector    & t_photoSensor_position      ,
                                      G4RotationMatrix*  t_photoSensor_rotationMatrix,
                                const G4String         & t_photoSensor_name          ,
                                      G4int              t_photoSensor_ID            ,
                                const G4ThreeVector    & t_hit_position              ,
                                const G4double         & t_hit_time                  ,
                                const G4double         & t_hit_energy                ,
                                const G4double         & t_particle_energy           ,
                                const G4ThreeVector    & t_hit_momentum              ,
                                const G4ThreeVector    & t_particle_momentum          ) {
    m_photoSensor_position       = t_photoSensor_position      ;
    m_photoSensor_rotationMatrix = t_photoSensor_rotationMatrix;
    m_photoSensor_name           = t_photoSensor_name          ;
    m_photoSensor_ID             = t_photoSensor_ID            ;
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
    m_photoSensor_ID             = t_hit.m_photoSensor_ID            ;
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
                << "photoSensor_rotationMatrix=" << *t_photoSensorHit.m_photoSensor_rotationMatrix << ", \n"
                << "photoSensor_name="           <<  t_photoSensorHit.m_photoSensor_name           << ", \n"
                << "photoSensor_ID="             <<  t_photoSensorHit.m_photoSensor_ID             << ", \n"
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

void PhotoSensorHit::set_hit_position_absolute( G4ThreeVector t_hit_position ) {
    m_hit_position = t_hit_position;
}

void PhotoSensorHit::set_photoSensor_position( G4ThreeVector t_photoSensor_position ) {
    m_photoSensor_position = t_photoSensor_position;
}

void PhotoSensorHit::set_photoSensor_rotationMatrix( G4RotationMatrix* t_photoSensor_rotationMatrix ) {
    m_photoSensor_rotationMatrix = t_photoSensor_rotationMatrix;
}

void PhotoSensorHit::set_photoSensor_name( const G4String& t_photoSensor_name ) {
    m_photoSensor_name = t_photoSensor_name;
}

void PhotoSensorHit::set_photoSensor_ID( G4int t_photoSensor_ID ) {
    m_photoSensor_ID = t_photoSensor_ID;
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

void PhotoSensorHit::set_hit_process( const G4String& t_hit_process ) {
    m_hit_process = t_hit_process;
}

G4ThreeVector PhotoSensorHit::get_hit_position_absolute() {
    return m_hit_position;
}

G4ThreeVector PhotoSensorHit::get_hit_position_relative() {
    G4ThreeVector relative_position = m_hit_position - m_photoSensor_position;
    G4RotationMatrix inverse_rotation = m_photoSensor_rotationMatrix->inverse();
    G4ThreeVector rotated_relative_position = inverse_rotation * relative_position;
    if( abs( rotated_relative_position.z() ) >  5.000001 ||
        abs( rotated_relative_position.x() ) > 50.000001 ||
        abs( rotated_relative_position.y() ) > 50.000001   ) {
        G4cout << G4endl;
        G4cout << "photosensor = " << m_photoSensor_name << G4endl;
        G4cout << "photosensor position = " << m_photoSensor_position << G4endl;
        G4cout << "hit position = " << m_hit_position << G4endl;
        G4cout << "relative_position = " << relative_position << G4endl;
        G4cout << "rotated_relative_position = " << rotated_relative_position << G4endl;
        G4Exception( "PhotoSensorHit::get_hit_position_relative()", "Error", FatalException, "rotated_relative_position is out of range" );
    }
    return rotated_relative_position;
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

G4int PhotoSensorHit::get_photoSensor_ID() {
    return m_photoSensor_ID;
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

G4String PhotoSensorHit::get_hit_process() {
    return m_hit_process;
}
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

#include "MediumHit.hh"

G4ThreadLocal G4Allocator< MediumHit >* MediumHitAllocator;

MediumHit::MediumHit() {
}

MediumHit::MediumHit( const G4ThreeVector    & t_medium_position          ,
                            G4RotationMatrix*  t_medium_rotationMatrix    ,
                      const G4String         & t_medium_name              ,
                            G4int              t_medium_ID                ,
                      const G4ThreeVector    & t_hit_position_absolute    ,
                      const G4double         & t_hit_time                 ,
                      const G4double         & t_particle_energy          ,
                      const G4ThreeVector    & t_particle_momentum        ,
                      const G4ThreeVector    & t_particle_position_initial,
                            G4bool             t_particle_transmittance    ) {
    m_medium_position           = t_medium_position          ;
    m_medium_rotationMatrix     = t_medium_rotationMatrix    ;
    m_medium_name               = t_medium_name              ;
    m_medium_ID                 = t_medium_ID                ;
    m_hit_position_absolute     = t_hit_position_absolute    ;
    m_hit_time                  = t_hit_time                 ;
    m_particle_energy           = t_particle_energy          ;
    m_particle_momentum         = t_particle_momentum        ;
    m_particle_position_initial = t_particle_position_initial;
    m_particle_transmittance    = t_particle_transmittance   ;
}

MediumHit::MediumHit( const MediumHit& t_hit ) {
    m_medium_position           = t_hit.m_medium_position          ;
    m_medium_rotationMatrix     = t_hit.m_medium_rotationMatrix    ;
    m_medium_name               = t_hit.m_medium_name              ;
    m_medium_ID                 = t_hit.m_medium_ID                ;
    m_hit_position_absolute     = t_hit.m_hit_position_absolute    ;
    m_hit_time                  = t_hit.m_hit_time                 ;
    m_particle_energy           = t_hit.m_particle_energy          ;
    m_particle_momentum         = t_hit.m_particle_momentum        ;
    m_particle_position_initial = t_hit.m_particle_position_initial;
    m_particle_transmittance    = t_hit.m_particle_transmittance   ;
}

void MediumHit::Draw() {
    auto visManager = G4VVisManager::GetConcreteInstance();
    if( !visManager ) 
        return;

    G4Circle circle( m_hit_position_absolute );
    circle.SetScreenSize( 2 );
    circle.SetFillStyle( G4Circle::filled );
    G4Colour colour( 1.,1.,0. );
    G4VisAttributes attribs( colour );
    circle.SetVisAttributes( attribs );
    visManager->Draw( circle );
}

void MediumHit::Print() {
    G4cout << *this << G4endl;
}

std::ostream& operator<<( std::ostream& t_os, const MediumHit& t_mediumHit ) {
    t_os << "[" << "medium_position="       <<  t_mediumHit.m_medium_position              << ", \n"
                << "medium_rotationMatrix=" << *t_mediumHit.m_medium_rotationMatrix        << ", \n"
                << "medium_name="           <<  t_mediumHit.m_medium_name                  << ", \n"
                << "medium_ID="             <<  t_mediumHit.m_medium_ID                    << ", \n"
                << "hit_position_absolute=" <<  t_mediumHit.m_hit_position_absolute        << ", \n"
                << "hit_time="              <<  t_mediumHit.m_hit_time                     << ", \n"
                << "particle_energy="       <<  t_mediumHit.m_particle_energy              << ", \n"
                << "particle_momentum="     <<  t_mediumHit.m_particle_momentum            << ", \n"
                << "particle_position_initial=" << t_mediumHit.m_particle_position_initial << ", \n"
                << "particle_transmittance=" <<  t_mediumHit.m_particle_transmittance      << "]" << G4endl;

    return t_os;
}

std::ostream& operator<<( std::ostream& t_os, const MediumHit* t_mediumHit ) {
    t_os << *t_mediumHit;

    return t_os;
}

void MediumHit::set_hit_position_absolute( G4ThreeVector t_hit_position_absolute ) {
    m_hit_position_absolute = t_hit_position_absolute;
}

void MediumHit::set_medium_position( G4ThreeVector t_medium_position ) {
    m_medium_position = t_medium_position;
}

void MediumHit::set_medium_rotationMatrix( G4RotationMatrix* t_medium_rotationMatrix ) {
    m_medium_rotationMatrix = t_medium_rotationMatrix;
}

void MediumHit::set_medium_name( const G4String& t_medium_name ) {
    m_medium_name = t_medium_name;
}

void MediumHit::set_medium_ID( G4int t_medium_ID ) {
    m_medium_ID = t_medium_ID;
}

void MediumHit::set_hit_time( G4double t_hit_time ) {
    m_hit_time = t_hit_time;
}   

void MediumHit::set_particle_energy( G4double t_particle_energy ) {
    m_particle_energy = t_particle_energy;
}

void MediumHit::set_particle_momentum( G4ThreeVector t_particle_momentum ) {
    m_particle_momentum = t_particle_momentum;
}

void MediumHit::set_particle_position_initial( G4ThreeVector t_particle_position_initial ) {
    m_particle_position_initial = t_particle_position_initial;
}

void MediumHit::set_hit_process( const G4String& t_hit_process ) {
    m_hit_process = t_hit_process;
}

void MediumHit::set_particle_transmittance( G4bool t_particle_transmittance ) {
    m_particle_transmittance = t_particle_transmittance;
}

G4bool MediumHit::get_particle_transmittance() {
    return m_particle_transmittance;
}

G4ThreeVector MediumHit::get_hit_position_absolute() {
    return m_hit_position_absolute;
}

G4ThreeVector MediumHit::get_medium_position() {
    return m_medium_position;
}

G4RotationMatrix* MediumHit::get_medium_rotationMatrix() {
    return m_medium_rotationMatrix;
}

G4String MediumHit::get_medium_name() {
    return m_medium_name;
}

G4int MediumHit::get_medium_ID() {
    return m_medium_ID;
}

G4double MediumHit::get_hit_time() {
    return m_hit_time;
}

G4ThreeVector MediumHit::get_particle_position_initial() {
    return m_particle_position_initial;
}

G4String MediumHit::get_hit_process() {
    return m_hit_process;
}

G4double MediumHit::get_particle_energy() {
    return m_particle_energy;
}

G4ThreeVector MediumHit::get_particle_momentum() {
    return m_particle_momentum;
}
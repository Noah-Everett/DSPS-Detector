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

#include "LensHit.hh"

G4ThreadLocal G4Allocator< LensHit >* LensHitAllocator;

LensHit::LensHit() {
}

LensHit::LensHit( const G4ThreeVector    & t_lens_position      ,
                        G4RotationMatrix*  t_lens_rotationMatrix,
                  const G4String         & t_lens_name          ,
                        G4int              t_lens_ID            ,
                  const G4ThreeVector    & t_hit_position       ,
                  const G4double         & t_hit_time           ,
                  const G4double         & t_hit_energy         ,
                  const G4double         & t_particle_energy    ,
                  const G4ThreeVector    & t_hit_momentum       ,
                  const G4ThreeVector    & t_particle_momentum   ) {
    m_lens_position       = t_lens_position      ;
    m_lens_rotationMatrix = t_lens_rotationMatrix;
    m_lens_name           = t_lens_name          ;
    m_lens_ID             = t_lens_ID            ;
    m_hit_position        = t_hit_position       ;
    m_hit_time            = t_hit_time           ;
    m_hit_energy          = t_hit_energy         ;
    m_hit_momentum        = t_hit_momentum       ;
    m_particle_energy     = t_particle_energy    ;
    m_particle_momentum   = t_particle_momentum  ;
}

LensHit::LensHit( const LensHit& t_hit ) {
    m_lens_position       = t_hit.m_lens_position      ;
    m_lens_rotationMatrix = t_hit.m_lens_rotationMatrix;
    m_lens_name           = t_hit.m_lens_name          ;
    m_lens_ID             = t_hit.m_lens_ID            ;
    m_hit_position        = t_hit.m_hit_position       ;
    m_hit_time            = t_hit.m_hit_time           ;
    m_hit_energy          = t_hit.m_hit_energy         ;
    m_hit_momentum        = t_hit.m_hit_momentum       ;
    m_particle_energy     = t_hit.m_particle_energy    ;
    m_particle_momentum   = t_hit.m_particle_momentum  ;
}

void LensHit::Draw() {
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

void LensHit::Print() {
    G4cout << *this << G4endl;
}

std::ostream& operator<<( std::ostream& t_os, const LensHit& t_lensHit ) {
    t_os << "[" << "lens_position="       <<  t_lensHit.m_lens_position       << ", \n"
                << "lens_rotationMatrix=" << *t_lensHit.m_lens_rotationMatrix << ", \n"
                << "lens_name="           <<  t_lensHit.m_lens_name           << ", \n"
                << "lens_ID="             <<  t_lensHit.m_lens_ID             << ", \n"
                << "hit_position="        <<  t_lensHit.m_hit_position        << ", \n"
                << "hit_time="            <<  t_lensHit.m_hit_time            << ", \n"
                << "hit_energy="          <<  t_lensHit.m_hit_energy          << ", \n"
                << "hit_momentum="        <<  t_lensHit.m_hit_momentum        << ", \n"
                << "particle_energy="     <<  t_lensHit.m_particle_energy     << ", \n"
                << "particle_momentum="   <<  t_lensHit.m_particle_momentum   << "]";

    return t_os;
}

std::ostream& operator<<( std::ostream& t_os, const LensHit* t_lensHit ) {
    t_os << *t_lensHit;

    return t_os;
}

void LensHit::set_hit_position_absolute( G4ThreeVector t_hit_position ) {
    m_hit_position = t_hit_position;
}

void LensHit::set_lens_position( G4ThreeVector t_lens_position ) {
    m_lens_position = t_lens_position;
}

void LensHit::set_lens_rotationMatrix( G4RotationMatrix* t_lens_rotationMatrix ) {
    m_lens_rotationMatrix = t_lens_rotationMatrix;
}

void LensHit::set_lens_name( const G4String& t_lens_name ) {
    m_lens_name = t_lens_name;
}

void LensHit::set_lens_ID( G4int t_lens_ID ) {
    m_lens_ID = t_lens_ID;
}

void LensHit::set_hit_time( G4double t_hit_time ) {
    m_hit_time = t_hit_time;
}   

void LensHit::set_hit_energy( G4double t_hit_energy ) {
    m_hit_energy = t_hit_energy;
}

void LensHit::set_particle_energy( G4double t_particle_energy ) {
    m_particle_energy = t_particle_energy;
}

void LensHit::set_hit_momentum( G4ThreeVector t_hit_momentum ) {
    m_hit_momentum = t_hit_momentum;
}

void LensHit::set_particle_momentum( G4ThreeVector t_particle_momentum ) {
    m_particle_momentum = t_particle_momentum;
}

void LensHit::set_particle_position_initial( G4ThreeVector t_particle_position_initial ) {
    m_particle_position_initial = t_particle_position_initial;
}

void LensHit::set_hit_process( const G4String& t_hit_process ) {
    m_hit_process = t_hit_process;
}

void LensHit::set_particle_transmittance( G4bool t_particle_transmittance ) {
    m_particle_transmittance = t_particle_transmittance;
}

G4bool LensHit::get_particle_transmittance() {
    return m_particle_transmittance;
}

G4ThreeVector LensHit::get_hit_position_absolute() {
    return m_hit_position;
}

G4ThreeVector LensHit::get_hit_position_relative() {
    G4ThreeVector relative_position = m_hit_position - m_lens_position;
    G4RotationMatrix inverse_rotation = m_lens_rotationMatrix->inverse();
    G4ThreeVector rotated_relative_position = inverse_rotation * relative_position;

    // G4double epsilon = 1e-6;
    // if( abs( rotated_relative_position.z() ) > m_constructionMessenger->get_lens_size_depth ()     + epsilon ||
    //     abs( rotated_relative_position.x() ) > m_constructionMessenger->get_lens_size_height() / 2 + epsilon ||
    //     abs( rotated_relative_position.y() ) > m_constructionMessenger->get_lens_size_width () / 2 + epsilon   ) {
    //     G4cout << G4endl;
    //     G4cout << "lens = " << m_lens_name << G4endl;
    //     G4cout << "lens position = " << m_lens_position << G4endl;
    //     G4cout << "lens size = " << m_constructionMessenger->get_lens_size_depth() << " x " << m_constructionMessenger->get_lens_size_height() << " x " << m_constructionMessenger->get_lens_size_width() << G4endl;
    //     G4cout << "hit position = " << m_hit_position << G4endl;
    //     G4cout << "relative_position = " << relative_position << G4endl;
    //     G4cout << "rotated_relative_position = " << rotated_relative_position << G4endl;
    //     G4Exception( "lensHit::get_hit_position_relative()", "Error", FatalException, "rotated_relative_position is out of range" );
    // }
    return rotated_relative_position;
}

G4ThreeVector LensHit::get_lens_position() {
    return m_lens_position;
}

G4RotationMatrix* LensHit::get_lens_rotationMatrix() {
    return m_lens_rotationMatrix;
}

G4String LensHit::get_lens_name() {
    return m_lens_name;
}

G4int LensHit::get_lens_ID() {
    return m_lens_ID;
}

G4double LensHit::get_hit_time() {
    return m_hit_time;
}

G4double LensHit::get_hit_energy() {
    return m_hit_energy;
}

G4double LensHit::get_particle_energy() {
    return m_particle_energy;
}

G4ThreeVector LensHit::get_hit_momentum() {
    return m_hit_momentum;
}

G4ThreeVector LensHit::get_particle_momentum() {
    return m_particle_momentum;
}

G4ThreeVector LensHit::get_particle_position_initial() {
    return m_particle_position_initial;
}

G4String LensHit::get_hit_process() {
    return m_hit_process;
}
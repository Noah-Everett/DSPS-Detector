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

#include "CalorimeterHit.hh"

G4ThreadLocal G4Allocator< CalorimeterHit >* CalorimeterHitAllocator;

CalorimeterHit::CalorimeterHit() {
}

CalorimeterHit::CalorimeterHit( const G4ThreeVector    & t_calorimeter_position      ,
                                      G4RotationMatrix*  t_calorimeter_rotationMatrix,
                                const G4String         & t_calorimeter_name          ,
                                      G4int              t_calorimeter_ID            ,
                                const G4ThreeVector    & t_hit_position              ,
                                const G4double         & t_hit_time                  ,
                                const G4double         & t_hit_energy                ,
                                const G4double         & t_particle_energy           ,
                                const G4ThreeVector    & t_hit_momentum              ,
                                const G4ThreeVector    & t_particle_momentum          ) {
    m_calorimeter_position       = t_calorimeter_position      ;
    m_calorimeter_rotationMatrix = t_calorimeter_rotationMatrix;
    m_calorimeter_name           = t_calorimeter_name          ;
    m_calorimeter_ID             = t_calorimeter_ID            ;
    m_hit_position               = t_hit_position              ;
    m_hit_time                   = t_hit_time                  ;
    m_hit_energy                 = t_hit_energy                ;
    m_hit_momentum               = t_hit_momentum              ;
    m_particle_energy            = t_particle_energy           ;
    m_particle_momentum          = t_particle_momentum         ;
}

CalorimeterHit::CalorimeterHit( const CalorimeterHit& t_hit ) {
    m_calorimeter_position       = t_hit.m_calorimeter_position      ;
    m_calorimeter_rotationMatrix = t_hit.m_calorimeter_rotationMatrix;
    m_calorimeter_name           = t_hit.m_calorimeter_name          ;
    m_calorimeter_ID             = t_hit.m_calorimeter_ID            ;
    m_hit_position               = t_hit.m_hit_position              ;
    m_hit_time                   = t_hit.m_hit_time                  ;
    m_hit_energy                 = t_hit.m_hit_energy                ;
    m_hit_momentum               = t_hit.m_hit_momentum              ;
    m_particle_energy            = t_hit.m_particle_energy           ;
    m_particle_momentum          = t_hit.m_particle_momentum         ;
}

void CalorimeterHit::Draw() {
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

void CalorimeterHit::Print() {
    G4cout << *this << G4endl;
}

std::ostream& operator<<( std::ostream& t_os, const CalorimeterHit& t_calorimeterHit ) {
    t_os << "[" << "calorimeter_position="       <<  t_calorimeterHit.m_calorimeter_position       << ", \n"
                << "calorimeter_rotationMatrix=" << *t_calorimeterHit.m_calorimeter_rotationMatrix << ", \n"
                << "calorimeter_name="           <<  t_calorimeterHit.m_calorimeter_name           << ", \n"
                << "calorimeter_ID="             <<  t_calorimeterHit.m_calorimeter_ID             << ", \n"
                << "hit_position="               <<  t_calorimeterHit.m_hit_position               << ", \n"
                << "hit_time="                   <<  t_calorimeterHit.m_hit_time                   << ", \n"
                << "hit_energy="                 <<  t_calorimeterHit.m_hit_energy                 << ", \n"
                << "hit_momentum="               <<  t_calorimeterHit.m_hit_momentum               << ", \n"
                << "particle_energy="            <<  t_calorimeterHit.m_particle_energy            << ", \n"
                << "particle_momentum="          <<  t_calorimeterHit.m_particle_momentum          << "]";

    return t_os;
}

std::ostream& operator<<( std::ostream& t_os, const CalorimeterHit* t_calorimeterHit ) {
    t_os << *t_calorimeterHit;

    return t_os;
}

void CalorimeterHit::set_hit_position_absolute( G4ThreeVector t_hit_position ) {
    m_hit_position = t_hit_position;
}

void CalorimeterHit::set_calorimeter_position( G4ThreeVector t_calorimeter_position ) {
    m_calorimeter_position = t_calorimeter_position;
}

void CalorimeterHit::set_calorimeter_rotationMatrix( G4RotationMatrix* t_calorimeter_rotationMatrix ) {
    m_calorimeter_rotationMatrix = t_calorimeter_rotationMatrix;
}

void CalorimeterHit::set_calorimeter_name( const G4String& t_calorimeter_name ) {
    m_calorimeter_name = t_calorimeter_name;
}

void CalorimeterHit::set_calorimeter_ID( G4int t_calorimeter_ID ) {
    m_calorimeter_ID = t_calorimeter_ID;
}

void CalorimeterHit::set_hit_time( G4double t_hit_time ) {
    m_hit_time = t_hit_time;
}   

void CalorimeterHit::set_hit_energy( G4double t_hit_energy ) {
    m_hit_energy = t_hit_energy;
}

void CalorimeterHit::set_particle_energy( G4double t_particle_energy ) {
    m_particle_energy = t_particle_energy;
}

void CalorimeterHit::set_hit_momentum( G4ThreeVector t_hit_momentum ) {
    m_hit_momentum = t_hit_momentum;
}

void CalorimeterHit::set_particle_momentum( G4ThreeVector t_particle_momentum ) {
    m_particle_momentum = t_particle_momentum;
}

void CalorimeterHit::set_particle_position_initial( G4ThreeVector t_particle_position_initial ) {
    m_particle_position_initial = t_particle_position_initial;
}

void CalorimeterHit::set_hit_process( const G4String& t_hit_process ) {
    m_hit_process = t_hit_process;
}

G4ThreeVector CalorimeterHit::get_hit_position_absolute() {
    return m_hit_position;
}

G4ThreeVector CalorimeterHit::get_hit_position_relative() {
    G4ThreeVector relative_position = m_hit_position - m_calorimeter_position;
    G4RotationMatrix inverse_rotation = m_calorimeter_rotationMatrix->inverse();
    G4ThreeVector rotated_relative_position = inverse_rotation * relative_position;

    G4double epsilon = 1e-6;
    if( abs( rotated_relative_position.z() ) > m_constructionMessenger->get_calorimeter_size_depth ()     + epsilon ||
        abs( rotated_relative_position.x() ) > m_constructionMessenger->get_calorimeter_size_height() / 2 + epsilon ||
        abs( rotated_relative_position.y() ) > m_constructionMessenger->get_calorimeter_size_width () / 2 + epsilon   ) {
        G4cout << G4endl;
        G4cout << "calorimeter = " << m_calorimeter_name << G4endl;
        G4cout << "calorimeter position = " << m_calorimeter_position << G4endl;
        G4cout << "calorimeter size = " << m_constructionMessenger->get_calorimeter_size_depth() << " x " << m_constructionMessenger->get_calorimeter_size_height() << " x " << m_constructionMessenger->get_calorimeter_size_width() << G4endl;
        G4cout << "hit position = " << m_hit_position << G4endl;
        G4cout << "relative_position = " << relative_position << G4endl;
        G4cout << "rotated_relative_position = " << rotated_relative_position << G4endl;
        G4Exception( "CalorimeterHit::get_hit_position_relative()", "Error", FatalException, "rotated_relative_position is out of range" );
    }
    return rotated_relative_position;
}

G4ThreeVector CalorimeterHit::get_calorimeter_position() {
    return m_calorimeter_position;
}

G4RotationMatrix* CalorimeterHit::get_calorimeter_rotationMatrix() {
    return m_calorimeter_rotationMatrix;
}

G4String CalorimeterHit::get_calorimeter_name() {
    return m_calorimeter_name;
}

G4int CalorimeterHit::get_calorimeter_ID() {
    return m_calorimeter_ID;
}

G4double CalorimeterHit::get_hit_time() {
    return m_hit_time;
}

G4double CalorimeterHit::get_hit_energy() {
    return m_hit_energy;
}

G4double CalorimeterHit::get_particle_energy() {
    return m_particle_energy;
}

G4ThreeVector CalorimeterHit::get_hit_momentum() {
    return m_hit_momentum;
}

G4ThreeVector CalorimeterHit::get_particle_momentum() {
    return m_particle_momentum;
}

G4ThreeVector CalorimeterHit::get_particle_position_initial() {
    return m_particle_position_initial;
}

G4String CalorimeterHit::get_hit_process() {
    return m_hit_process;
}
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


#include "SteppingAction.hh"

SteppingAction::SteppingAction( RunAction* t_runAction ) :
    m_runAction( t_runAction ),
    m_outputManager( m_runAction->get_outputManager() ) {
}

SteppingAction::~SteppingAction() {
}

void SteppingAction::UserSteppingAction( const G4Step* t_step ) {
    if( !t_step->GetPostStepPoint()->GetPhysicalVolume()                ||
        abs( t_step->GetTrack()->GetPosition().x() ) > m_world_size.x() ||
        abs( t_step->GetTrack()->GetPosition().y() ) > m_world_size.y() ||
        abs( t_step->GetTrack()->GetPosition().z() ) > m_world_size.z()    ) {
        t_step->GetTrack()->SetTrackStatus( fKillTrackAndSecondaries );
        return;
    }

    if( abs( t_step->GetTrack()->GetDefinition()->GetPDGEncoding() ) == 0  || 
        abs( t_step->GetTrack()->GetDefinition()->GetPDGEncoding() ) == 22    ) {
        m_outputManager->fill_tuple_column_double( "photon_length"     , t_step->GetStepLength()                                               );
        m_outputManager->fill_tuple_column_string( "photon_process"    , t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
        m_outputManager->fill_tuple_column_double( "photon_time"       , t_step->GetPostStepPoint()->GetGlobalTime()                           );
        m_outputManager->fill_tuple_column_double( "photon_position_x" , t_step->GetPostStepPoint()->GetPosition().x()                         );
        m_outputManager->fill_tuple_column_double( "photon_position_y" , t_step->GetPostStepPoint()->GetPosition().y()                         );
        m_outputManager->fill_tuple_column_double( "photon_position_z" , t_step->GetPostStepPoint()->GetPosition().z()                         );
        m_outputManager->fill_tuple_column_double( "photon_direction_x", t_step->GetPostStepPoint()->GetMomentumDirection().x()                );
        m_outputManager->fill_tuple_column_double( "photon_direction_y", t_step->GetPostStepPoint()->GetMomentumDirection().y()                );
        m_outputManager->fill_tuple_column_double( "photon_direction_z", t_step->GetPostStepPoint()->GetMomentumDirection().z()                );
        m_outputManager->fill_tuple_column_double( "photon_energy"     , t_step->GetPostStepPoint()->GetKineticEnergy()                        );
        m_outputManager->fill_tuple_column_string( "photon_volume"     , t_step->GetPostStepPoint()->GetPhysicalVolume()->GetName()            );
        m_outputManager->fill_tuple_column       ( "photon" );
    // } else if( t_step->GetTrack()->GetParentID() == 0 ) {
    } else {
        m_outputManager->fill_tuple_column_double ( "primary_position_x" , t_step->GetPostStepPoint()->GetPosition().x()                         );
        m_outputManager->fill_tuple_column_double ( "primary_position_y" , t_step->GetPostStepPoint()->GetPosition().y()                         );
        m_outputManager->fill_tuple_column_double ( "primary_position_z" , t_step->GetPostStepPoint()->GetPosition().z()                         );
        m_outputManager->fill_tuple_column_double ( "primary_direction_x", t_step->GetPostStepPoint()->GetMomentumDirection().x()                );
        m_outputManager->fill_tuple_column_double ( "primary_direction_y", t_step->GetPostStepPoint()->GetMomentumDirection().y()                );
        m_outputManager->fill_tuple_column_double ( "primary_direction_z", t_step->GetPostStepPoint()->GetMomentumDirection().z()                );
        m_outputManager->fill_tuple_column_string ( "primary_process"    , t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
        m_outputManager->fill_tuple_column_double ( "primary_time"       , t_step->GetPostStepPoint()->GetGlobalTime()                           );
        m_outputManager->fill_tuple_column_double ( "primary_energy"     , t_step->GetPostStepPoint()->GetKineticEnergy()                        );
        m_outputManager->fill_tuple_column_string ( "primary_volume"     , t_step->GetPostStepPoint()->GetPhysicalVolume()->GetName()            );
        m_outputManager->fill_tuple_column_integer( "primary_pdg"        , t_step->GetTrack()->GetDefinition()->GetPDGEncoding()                 );
        m_outputManager->fill_tuple_column        ( "primary" );
    }
}
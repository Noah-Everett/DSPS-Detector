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

#include "LensSystem.hh"

LensSystem::LensSystem( G4String t_name, G4bool t_makeLenses ) {
    if( !t_makeLenses ) 
        return;

    for( G4int nLens{ 0 }; nLens < m_constructionMessenger->get_lens_amount(); nLens++ )
        add_lens( new Lens( t_name, nLens ) );
}

LensSystem::~LensSystem() {
    // G4cout << "HERE3" << G4endl;
    for( Lens* lens : m_lenses )
        if( lens ) 
            delete lens;
    // G4cout << "HERE4" << G4endl;
}

void LensSystem::add_lens( Lens* t_lens ) {
    m_lenses.push_back( t_lens );
}

void  LensSystem::place( G4RotationMatrix* t_rotationMatrix     , 
                         G4ThreeVector     t_translationVector  , 
                         G4LogicalVolume * t_motherLogicalVolume, 
                         G4bool            t_isMany              ) {
    for( G4int nLens{ 0 }; nLens < m_lenses.size(); nLens++ ) {
        G4ThreeVector position( 0, 0, m_constructionMessenger->get_lens_position( nLens ) );
        position = *t_rotationMatrix * position;
        m_lenses[ nLens ]->place( t_rotationMatrix, position + t_translationVector, t_motherLogicalVolume, t_isMany );
    }
}

vector< Lens* > LensSystem::get_lenses() const { 
    return m_lenses; 
}

Lens* LensSystem::get_lens( G4int t_index ) const { 
    return m_lenses.at( t_index ); 
}

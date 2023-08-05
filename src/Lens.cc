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

#include "Lens.hh"

Lens::Lens( G4int t_nLens ) {
    G4cout << "HERE a" << G4endl;
    G4cout << t_nLens << G4endl;
    G4cout << m_constructionMessenger << G4endl;
    m_lensSurface_1 = new LensSurface( m_constructionMessenger->get_lens_surface_1_radius_x( t_nLens ),
                                       m_constructionMessenger->get_lens_surface_1_radius_y( t_nLens ),
                                       m_constructionMessenger->get_lens_surface_1_yLimits ( t_nLens ),
                                       m_constructionMessenger->get_lens_surface_1_yLimits ( t_nLens ),
                                       m_constructionMessenger->get_lens_material          ( t_nLens ),
                                       m_constructionMessenger->get_lens_visAttributes     ( t_nLens ) );
    G4cout << "HERE b" << G4endl;
    m_lensSurface_2 = new LensSurface( m_constructionMessenger->get_lens_surface_2_radius_x( t_nLens ),
                                       m_constructionMessenger->get_lens_surface_2_radius_y( t_nLens ),
                                       m_constructionMessenger->get_lens_surface_2_yLimits ( t_nLens ),
                                       m_constructionMessenger->get_lens_surface_2_yLimits ( t_nLens ),
                                       m_constructionMessenger->get_lens_material          ( t_nLens ),
                                       m_constructionMessenger->get_lens_visAttributes     ( t_nLens ) );
    G4cout << "HERE c" << G4endl;
    m_middleTube->set_solid( new G4EllipticalTube( "lensMiddle"                                                   ,
                                                   m_constructionMessenger->get_lens_surface_2_yLimits ( t_nLens ),
                                                   m_constructionMessenger->get_lens_surface_2_yLimits ( t_nLens ),
                                             0.5 * m_constructionMessenger->get_lens_distance          ( t_nLens ) ) ); 
    G4cout << "HERE d" << G4endl;
    m_middleTube->set_material( m_constructionMessenger->get_lens_material( t_nLens ) );
    G4cout << "HERE e" << G4endl;
    m_middleTube->set_visAttributes( m_constructionMessenger->get_lens_visAttributes( t_nLens ) );
    G4cout << "HERE f" << G4endl;
    m_middleTube->make_logicalVolume();
    G4cout << "HERE g" << G4endl;
}

Lens::~Lens() {
    if( m_lensSurface_1 ) delete m_lensSurface_1;
    if( m_lensSurface_2 ) delete m_lensSurface_2;
    if( m_middleTube    ) delete m_middleTube   ;
}

ostream& operator<<( ostream& t_os, Lens* t_lens )
{
    t_os << *t_lens;
    return t_os;
}

ostream& operator<<( ostream& t_os, const Lens& t_lens )
{
    t_os << "[" << t_lens.get_lensSurface_1                 () << ", "
                << t_lens.get_lensSurface_2                 () << ", "
                << t_lens.get_distanceBetweenSurfaces       () << ", "
                << t_lens.get_distanceOfLensCenterFromOrigin() 
         << "]";
    return t_os;
}

void Lens::place( G4RotationMatrix * t_rotationMatrix     , 
                  G4ThreeVector      t_translation        ,
                  G4LogicalVolume  * t_motherLogicalVolume,
                  G4bool             t_isMany              ) {
    m_lensSurface_1->place( t_rotationMatrix, t_translation, t_motherLogicalVolume, t_isMany );
    m_lensSurface_2->place( t_rotationMatrix, t_translation, t_motherLogicalVolume, t_isMany );
    m_middleTube   ->place( t_rotationMatrix, t_translation, t_motherLogicalVolume, t_isMany );
}
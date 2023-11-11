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

#ifndef LensSystem_h
#define LensSystem_h

#include "G4PVPlacement.hh"

#include "GeometricObject.hh"
#include "Lens.hh"
#include "ConstructionMessenger.hh"

#include <vector>

using std::vector;

class LensSystem
{
    public:
        LensSystem( const G4String&, G4bool = true );
       ~LensSystem();

        void add_lens( Lens* );

        void place( G4RotationMatrix*, G4ThreeVector , G4LogicalVolume*, G4bool = false );

        vector< Lens* >  get_lenses(       ) const;
        Lens           * get_lens  ( G4int ) const;
        G4String         get_name  (       ) const;

        void set_name( const G4String& );

    protected:
        vector< Lens* > m_lenses;
        G4String        m_name;
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
};

#endif

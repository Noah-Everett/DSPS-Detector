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

#ifndef OutputManager_h
#define OutputManager_h

#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"

#include "OutputMessenger.hh"
#include "ConstructionMessenger.hh"

#include <map>

using std::map;
using std::pair;

class OutputManager
{
    public:
        OutputManager();
       ~OutputManager();
        // static OutputManager* get_instance   ( G4bool = false );
        // static void           delete_instance(                );

        G4int                add_histogram_1D        ( const G4String&, const G4String&,       
                                                             G4int    ,       G4double , G4double  );
        G4int                add_histogram_2D        ( const G4String&, const G4String&,       
                                                             G4int    ,       G4double , G4double ,       
                                                             G4int    ,       G4double , G4double  );
        G4int                add_tuple_initialize    ( const G4String&, const G4String&            );
        void                 add_tuple_finalize      (                                             );
        pair< G4int, G4int > add_tuple_column_Integer( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_double ( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_3vector( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_string ( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_boolean( const G4String&,       G4int                );
        
        G4int                get_histogram_1D_ID( const G4String& );
        G4int                get_histogram_2D_ID( const G4String& );
        G4int                get_tuple_ID       ( const G4String& );
        pair< G4int, G4int > get_tuple_column_ID( const G4String& );

        void reset();

    private:
        //          OutputManager();
        // virtual ~OutputManager();

        OutputMessenger      * m_outputMessenger      { OutputMessenger      ::get_instance() };
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
        G4AnalysisManager    * m_analysisManager      { nullptr                               };

        map< G4String, G4int                > m_histogram_1D_IDs;
        map< G4String, G4int                > m_histogram_2D_IDs;
        map< G4String, G4int                > m_tuple_IDs       ;
        map< G4String, pair< G4int, G4int > > m_tuple_column_IDs;

    protected:
        // static OutputManager* m_instance;
};

#endif
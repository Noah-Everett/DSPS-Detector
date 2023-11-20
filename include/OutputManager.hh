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

#ifndef OutputManager_hh
#define OutputManager_hh

#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"

#include "OutputMessenger.hh"
#include "ConstructionMessenger.hh"

#include <map>
#include <vector>

using std::map;
using std::vector;
using std::pair;
using std::make_pair;

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
        pair< G4int, G4int > add_tuple_column_integer( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_double ( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_3vector( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_string ( const G4String&,       G4int                );
        pair< G4int, G4int > add_tuple_column_boolean( const G4String&,       G4int                );
        
        G4int                get_histogram_1D_ID( const G4String                      & );
        G4int                get_histogram_2D_ID( const G4String                      & );
        G4int                get_tuple_ID       ( const G4String                      & );
        G4int                get_tuple_ID       ( const vector< G4String             >& );
        G4int                get_tuple_ID       ( const vector< pair< G4int, G4int > >& );
        G4int                get_tuple_ID       ( const vector< G4int                >& );
        pair< G4int, G4int > get_tuple_column_ID( const G4String                      & );

        G4bool fill_histogram_1D        (       G4int                ,       G4double      , G4double           );
        G4bool fill_histogram_1D        ( const G4String            &,       G4double      , G4double           );
        G4bool fill_histogram_2D        (       G4int                ,       G4double      , G4double, G4double );
        G4bool fill_histogram_2D        ( const G4String            &,       G4double      , G4double, G4double );
        G4bool fill_tuple_column_integer(       pair< G4int, G4int > ,       G4int                              );
        G4bool fill_tuple_column_integer( const G4String            &,       G4int                              );
        G4bool fill_tuple_column_double (       pair< G4int, G4int > ,       G4double                           );
        G4bool fill_tuple_column_double ( const G4String            &,       G4double                           );
        G4bool fill_tuple_column_3vector(       pair< G4int, G4int > , const G4ThreeVector&                     );
        G4bool fill_tuple_column_3vector( const G4String            &, const G4ThreeVector&                     );
        G4bool fill_tuple_column_string (       pair< G4int, G4int > , const G4String     &                     );
        G4bool fill_tuple_column_string ( const G4String            &, const G4String     &                     );
        G4bool fill_tuple_column_boolean(       pair< G4int, G4int > ,       G4bool                             );
        G4bool fill_tuple_column_boolean( const G4String            &,       G4bool                             );
        G4bool fill_tuple_column        (       G4int                                                           );
        G4bool fill_tuple_column        ( const G4String            &                                           );

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
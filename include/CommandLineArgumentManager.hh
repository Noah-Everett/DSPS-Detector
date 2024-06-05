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

#ifndef CommandLineArgumentManager_hh
#define CommandLineArgumentManager_hh

#include "G4String.hh"

#include <vector>

using std::vector;
using std::stoi;

class CommandLineArgumentManager
{
    public:
        CommandLineArgumentManager( int, char**, const vector<G4String>& );
       ~CommandLineArgumentManager();

        G4bool findArgument       ( const G4String& ) const;
        G4bool findArgument_string( const G4String& ) const;
        G4bool findArgument_int   ( const G4String& ) const;

        G4String getArgument_string( const G4String& ) const;
        G4int    getArgument_int   ( const G4String& ) const;

    protected:
        int                m_argc        ;
        char**             m_argv        ;
        vector< G4String > m_arguments   ;
        vector< G4int >    m_argLocations;

        G4int  findArgumentLocation     ( const G4String& ) const;
        G4int  findArgumentLocation_argv( const G4String& ) const;
        G4bool saveArgumentLocation     ( const G4String& );
};

#endif
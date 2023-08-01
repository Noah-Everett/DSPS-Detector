#ifndef Maps_h
#define Maps_h

#include "G4String.hh"

#include <map>

const std::map< G4String, int > map_process  = { { "Cerenkov"            , 0 },
                                                 { "eBrem"               , 1 },
                                                 { "muMinusCaptureAtRest", 2 },
                                                 { "neutronInelastic"    , 3 },
                                                 { "nCapture"            , 4 } };
const std::map< G4String, int > map_particle = { { "mu"                  , 0 },
                                                 { "e"                   , 1 },
                                                 { "generic"             , 2 },
                                                 { "static"              , 3 },
                                                 { "nucleon"             , 4 } };

#endif

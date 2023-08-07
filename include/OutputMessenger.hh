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

#ifndef OutputMessenger_h
#define OutputMessenger_h

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"

class OutputMessenger : public G4UImessenger
{
    public:
        static OutputMessenger* get_instance   ();
        static void             delete_instance();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4bool   get_photoSensor_hits_save    () const;
        G4String get_photoSensor_hits_fileName() const;
        G4bool   get_GDML_save                () const;
        G4String get_GDML_fileName            () const;

        void set_photoSensor_hits_save    ( G4bool   );
        void set_photoSensor_hits_fileName( G4String );
        void set_GDML_save                ( G4bool   );
        void set_GDML_fileName            ( G4String );

    protected:
                 OutputMessenger();
        virtual ~OutputMessenger();
        
        G4UIcmdWithABool  * m_command_photoSensor_hits_save    { nullptr }; G4bool   m_variable_photoSensor_hits_save    { false };
        G4UIcmdWithAString* m_command_photoSensor_hits_fileName{ nullptr }; G4String m_variable_photoSensor_hits_fileName{ "" };
        G4UIcmdWithABool  * m_command_GDML_save                { nullptr }; G4bool   m_variable_GDML_save                { false };
        G4UIcmdWithAString* m_command_GDML_fileName            { nullptr }; G4String m_variable_GDML_fileName            { "" };
    
    private:
        static OutputMessenger* m_instance;
};

#endif
# G4-DSPD-Detector-Sim

This project is a simulation of a detector system using the Geant4 software toolkit. It is designed to simulate the interaction of particles with a detector, and provides a framework for analyzing the results of these interactions.

### Features

- Simulation of particle interactions with a detector
- Output of simulation results for analysis
- Visualization of the detector and particle paths

### Getting Started

To run the simulation, you need to have the Geant4 software toolkit installed on your system. Once you have Geant4 installed, you can run the simulation using the provided macro files.

For example, to initialize the simulation, you can use the `event.mac` macro:
```
/run/initialize

/control/verbose 0 #2
/run/verbose 0 #2
/event/verbose 0
/tracking/verbose 0 #1
#/process/verbose 1

/gun/particle e-
/gun/energy 1200 keV
/gun/position 0 0 0
/gun/direction 1 0 0
/run/beamOn 1
```

This macro sets up the initial conditions for the simulation, including the particle type, energy, position, and direction.

### Detector Configuration

The detector configuration can be adjusted using the parameters in the `parameters_detector.mac` macro:
```
/geometry/calorimeter/size                       10.0 2.00 40.0 cm
/geometry/calorimeter/material                   aluminum
/geometry/calorimeter/visibility                 true
/geometry/calorimeter/color                      red
/geometry/calorimeter/alpha                      0.3
/geometry/calorimeter/forceSolid                 true

/geometry/photoSensor/surface/size               10.0 10.0 0.50 cm
/geometry/photoSensor/surface/material           ANNIEWater_unDoped
/geometry/photoSensor/surface/visibility         true
/geometry/photoSensor/surface/color              blue
/geometry/photoSensor/surface/alpha              1
/geometry/photoSensor/surface/forceSolid         true

/geometry/photoSensor/body/size                  10.0 10.0 2.00 cm
/geometry/photoSensor/body/material              ANNIEWater_unDoped
/geometry/photoSensor/body/visibility            true
/geometry/photoSensor/body/color                 blue
/geometry/photoSensor/body/alpha                 0.5
/geometry/photoSensor/body/forceSolid            true
```

This includes parameters such as the size and material of the calorimeter and photo sensor.

### Output

The simulation results are outputted for analysis. The output parameters can be adjusted using the `parameters_output.mac` macro:
```
/GDML/save                                            false
/GDML/fileName                                        geometry.gdml

/output/fileName                                      output.root

/output/photoSensor/hits/position/binned/save         true
/output/photoSensor/hits/position/binned/nBinsPerSide 100
/output/photoSensor/hits/position/save                true
/output/photoSensor/hits/time/save                    true

/output/primary/position/save                         true
/output/primary/emission/photon/save                  true
/output/primary/emission/electron/save                true
/output/primary/process/save                          true
/output/primary/time/save                             true

/output/photon/length/save                            true
/output/photon/process/save                           true
/output/photon/time/save                              true
```

### License

This project is licensed under the terms of the Geant4 Software License. Please see the license in the file LICENSE and URL above for the full disclaimer and the limitation of liability.
```
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
```

### Contributing

Contributions are welcome. Please make sure to update tests as appropriate.

### Disclaimer

This code implementation is the result of the scientific and technical work of the GEANT4 collaboration. By using, copying, modifying or distributing the software (or any work based on the software) you agree to acknowledge its use in resulting scientific publications, and indicate your acceptance of all terms of the Geant4 Software license.
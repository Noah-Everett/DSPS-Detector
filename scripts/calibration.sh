#!/bin/bash

NUM_POSITIONS=10

for ((i=1; i<=$NUM_POSITIONS; i++))
do
    # Generate calibration.mac file
    cat << EOF > calibration.mac
/run/initialize
/control/verbose 0
/run/verbose 0
/event/verbose 0
/tracking/verbose 0
#/process/verbose 1
/gun/particle e-
/gun/energy 3 MeV
/gun/position $i $i $i
/gun/direction 1 0 0
/run/beamOn 1
EOF

    # Run the application with calibration.mac
    ./application -m calibration.mac
done


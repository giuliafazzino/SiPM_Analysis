#! /usr/bin/env bash

move_to_and_scan()
{
    chip=$1
    sensor=$2
    
    date
    echo " --- /au/measure/move_to_sensor.sh ${chip} ${sensor} "
    time -p /au/measure/move_to_sensor.sh ${chip} ${sensor}
    
    date
    echo " --- /au/measure/scan.sh ${chip} ${sensor} "
    time -p /au/measure/scan.sh ${chip} ${sensor}
}

move_to_and_scan 1 A2
move_to_and_scan 1 A1

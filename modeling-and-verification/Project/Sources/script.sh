#!/bin/bash

# source ../Confmodelsim/config 

## clean
if [ -d ../Libs/LIB_CONTROLLER ] 
then /bin/rm -rf ../Libs/LIB_CONTROLLER 
fi

## creation of the lib
vlib ../Libs/LIB_CONTROLLER

## compilations
vcom -work LIB_CONTROLLER Counter.vhd
vcom -cover bfs -work LIB_CONTROLLER Controller.vhd
vcom -work LIB_CONTROLLER Oven.vhd

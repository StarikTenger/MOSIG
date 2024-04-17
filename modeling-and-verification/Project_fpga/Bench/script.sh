#!/bin/bash

## clean
if [ -d ../Libs/LIB_CONTROLLER_BENCH ] 
then /bin/rm -rf ../Libs/LIB_CONTROLLER_BENCH
fi

## creation of the lib
vlib ../Libs/LIB_CONTROLLER_BENCH

## compilation
vcom -work LIB_CONTROLLER_BENCH test_oven.vhd


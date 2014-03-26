#!/bin/bash

# configuration
. ./config_load.sh

# clean AppShared
cd $SOURCE_DIRECTORY/TSDevices_Apps/Shared
./qmake_tpro.sh clean
cd $WORKING_DIR

# clean ProductionTools
cd $SOURCE_DIRECTORY/TSDevices_Apps/ProductionTools
./build_device.sh clean
cd $WORKING_DIR

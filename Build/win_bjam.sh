#!/bin/bash


# configuration
. ./config_load.sh
DEST_DIRECTORY=$DEST_DIRECTORY_TPRO_WIN


. ./config.env

WORKING_DIR=C:\Data\TBS\git\tbs-devices\
SOURCE_DIRECTORY=$WORKING_DIR"/.."
PLATFORM=""
BJAM_PARAM="toolset=gcc"
ACTION=""
TYPE="release"



PLATFORM=win

echo $SOURCE_DIRECTORY
cd $SOURCE_DIRECTORY
echo t $TYPE d device=$PLATFORM p $BJAM_PARAM a $ACTION
bjam $TYPE device=$PLATFORM $BJAM_PARAM $ACTION
cd $WORKING_DIR
 
#!/bin/bash

if [ ! -f ./config.env ]; then
	echo "Please create config.env with content (example):"
	echo "DEST_DIRECTORY_TPRO_LIN=\"<path>\""
	echo "DEST_DIRECTORY_TPRO_TPRO=\"<path>\""

	echo "DEST_DIRECTORY_TCOMTE_LIN=\"<path>\""
	echo "DEST_DIRECTORY_TCOMTE_TPRO=\"<path>\""

	echo "DEST_DIRECTORY_TCOMST_LIN=\"<path>\""
	echo "DEST_DIRECTORY_TCOMST_TPRO=\"<path>\""	
	exit 1
fi

WORKING_DIR=`pwd`
SOURCE_DIRECTORY=$WORKING_DIR"/.."
. ./config.env



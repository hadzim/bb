#!/bin/bash

if [ $# -ne 1 ]; then
        echo "usage: $0 <destination_dir>"
		exit 0
fi

DEST_DIRECTORY="$1"

if [ ! -z $DEST_DIRECTORY ]; then
	rm -rf $DEST_DIRECTORY/Plugins
	rm -rf $DEST_DIRECTORY/lib
	rm -rf $DEST_DIRECTORY/data/remoteControl
fi



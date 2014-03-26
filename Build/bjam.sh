#!/bin/sh

# Wrapper for output colorization
# TN, 01/2014

FILTER_CMD="cat"

if [ -e /usr/bin/ccze ]; then
	FILTER_CMD="/usr/bin/ccze -A"
fi

./bjam_core.sh $@ | $FILTER_CMD

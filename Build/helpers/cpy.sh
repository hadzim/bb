#!/bin/sh

echo "\033[38;5;148m#########################################"
echo "### COPY TO $1"
echo "#########################################\033[39m"

scp /home/honza/workspace/BeagleBone/bb/BeagleBone* root@$1:/home/root/bin/


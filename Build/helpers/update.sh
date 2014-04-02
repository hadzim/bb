#!/bin/bash

IP_ADDRESS=""
EXCLUDE_AP_START=""

if [ $# -eq 0 ]; then
	echo "usage: $0 <ip_address>"
	exit 0
fi

IP_ADDRESS="$1"

echo "updating: $IP_ADDRESS"

./ssh_systemctl.sh $IP_ADDRESS stop --all && ./shared_scp.sh $IP_ADDRESS && ./ssh_systemctl.sh $IP_ADDRESS start --all


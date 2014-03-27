#!/bin/bash

IP_ADDRESS=""
EXCLUDE_AP_START=""

if [ $# -eq 0 ]; then
	echo "usage: $0 <ip_address> [-aps]"
	exit 0
fi

./ssh_systemctl.sh $IP_ADDRESS stop --all && ./scp_shared.sh $IP_ADDRESS && ./ssh_systemctl.sh $IP_ADDRESS start --all


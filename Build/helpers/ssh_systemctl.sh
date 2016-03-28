
APPONLY=0
REMOTE_CONTROL=0
FB=0
UPDATER=0
HWIO=0
ALL=0
ALL_EXCLUDE_APP=0
IP_ADDRESS=""
CMD="status"

if [ $# -eq 0 ]; then
	echo "usage: $0 <ip_address> <command>"
	exit 0
fi

# By keeping options in alphabetical order, it's easy to add more.
while :
do
	case "$1" in
		*.*.*.*)
			IP_ADDRESS="$1"
			shift
		;;

		start | stop | status | restart | reload)
			CMD="$1"
			shift
		;;
					
		--all)
			ALL=1
			shift
		;;

		-aps)
			ALL_EXCLUDE_APP=1
			shift
		;;		

		
		--) # End of all options
			shift
			break;
		;;

		*)  # No more options
			break
		;;
	esac
done

function systemctl_service {
	REMOTE_CMD="systemctl $CMD $2"
	echo "" 
	echo " == $REMOTE_CMD ==" 
	echo "" 
	ssh root@$1 $REMOTE_CMD
	echo "=============================================================================" 
}

systemctl_service $IP_ADDRESS bb-mosquitto.service
#systemctl_service $IP_ADDRESS bb-collector.service
#systemctl_service $IP_ADDRESS bb-httpserver.service
#systemctl_service $IP_ADDRESS bb-remoteclient.service
#systemctl_service $IP_ADDRESS bb-status.service
#systemctl_service $IP_ADDRESS bb-actions.service
#systemctl_service $IP_ADDRESS bb-www.service
#systemctl_service $IP_ADDRESS bb-serialcollector.service

systemctl_service $IP_ADDRESS bb-new-www.service
systemctl_service $IP_ADDRESS bb-screen.service
systemctl_service $IP_ADDRESS bb-time.service
systemctl_service $IP_ADDRESS bb-rules.service
systemctl_service $IP_ADDRESS bb-history.service

systemctl_service $IP_ADDRESS bb-master.service
systemctl_service $IP_ADDRESS bb-motion.service
systemctl_service $IP_ADDRESS bb-camera.service
systemctl_service $IP_ADDRESS bb-network.service
systemctl_service $IP_ADDRESS bb-hw.service
systemctl_service $IP_ADDRESS bb-relay.service
systemctl_service $IP_ADDRESS bb-forecast.service
systemctl_service $IP_ADDRESS bb-temperature.service



exit 0




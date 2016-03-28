if [ $# -eq 0 ]; then
	echo "usage: $0 <ip_address>"
	exit 0
fi

echo "copying env structure"
scp -r services/*.service root@$1:/lib/systemd/system/


echo "making directories"

ssh root@$1 mkdir /home/tbs/
ssh root@$1 mkdir /home/tbs/bin/
ssh root@$1 mkdir /home/tbs/lib/
ssh root@$1 mkdir /home/tbs/www/

echo "reload daemon"

ssh root@$1 systemctl daemon-reload 

echo "enable services"

ssh root@$1 systemctl enable tbs-dbus-session.service
ssh root@$1 systemctl enable bb.mosquitto.service
ssh root@$1 systemctl enable bb-collector.service
ssh root@$1 systemctl enable bb-httpsender.service
ssh root@$1 systemctl enable bb-forecast.service
ssh root@$1 systemctl enable bb-temperature.service
ssh root@$1 systemctl enable bb-remoteclient.service
ssh root@$1 systemctl enable bb-status.service
ssh root@$1 systemctl enable bb-motion.service
ssh root@$1 systemctl enable bb-camera.service
ssh root@$1 systemctl enable bb-network.service
ssh root@$1 systemctl enable bb-actions.service
ssh root@$1 systemctl enable bb-www.service
ssh root@$1 systemctl enable bb-serialcollector.service
ssh root@$1 systemctl enable bb-screen.service
ssh root@$1 systemctl enable bb-time.service
ssh root@$1 systemctl enable bb-hw.service
ssh root@$1 systemctl enable bb-relay.service
ssh root@$1 systemctl enable bb-master.service
ssh root@$1 systemctl enable bb-rules.service


#cd /etc/systemd/system/
#ln -s /lib/systemd/system/bb-server.service bb-server.service
#ln -s /lib/systemd/system/bb-websender.service bb-websender.service
#ln -s /lib/systemd/system/bb-boardsensors.service bb-boardsensors.service

#systemctl daemon-reload

#systemctl enable bb-server.service
#systemctl enable bb-websender.service
#systemctl enable bb-boardsensors.service

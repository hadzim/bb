cd /etc/systemd/system/
ln -s /lib/systemd/system/bb-server.service bb-server.service
ln -s /lib/systemd/system/bb-websender.service bb-websender.service
ln -s /lib/systemd/system/bb-boardsensors.service bb-boardsensors.service

systemctl daemon-reload

systemctl enable bb-server.service
systemctl enable bb-websender.service
systemctl enable bb-boardsensors.service

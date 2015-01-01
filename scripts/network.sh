#!/bin/bash
ip link set dev eth0 up
ip addr add 192.168.0.99/24 broadcast 192.168.0.255 dev eth0

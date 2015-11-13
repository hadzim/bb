cmake_minimum_required(VERSION 3.0)
project(BB.Mosquitto)

install(FILES mosquitto.conf DESTINATION Default COMPONENT Default)

servicefile()
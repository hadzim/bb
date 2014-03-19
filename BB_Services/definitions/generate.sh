#!/bin/bash
RELATIVE_PATH="TSeries/Services"
INCLUDE_PATH="../include/TSeries/Services"
SRC_PATH="../src"
DOC_PATH="../doc"
JS_PATH="../js"

./../../TBS_Services/TBS.Services.Generator.lin/TBS_Services.Generator --def=hwio.xml --name=HWIO --src=$SRC_PATH/ --include=$INCLUDE_PATH/ --relative=$RELATIVE_PATH/ --generate=dbus
./../../TBS_Services/TBS.Services.Generator.lin/TBS_Services.Generator --def=tsdevices.xml --name=TSDevices --src=$SRC_PATH/ --include=$INCLUDE_PATH/ --relative=$RELATIVE_PATH/ --generate=dbus
# ./../../TBS_Services/TBS.Services.Generator.lin/TBS_Services.Generator --def=camera.xml --name=Camera --src=$SRC_PATH/ --include=$INCLUDE_PATH/ --relative=$RELATIVE_PATH/  --javascript=$JS_PATH/ --generate=json
./../../TBS_Services/TBS.Services.Generator.lin/TBS_Services.Generator --def=device-control.xml --name=DeviceControl --src=$SRC_PATH/ --include=$INCLUDE_PATH/ --doc=$DOC_PATH/ --javascript=$JS_PATH/ --relative=$RELATIVE_PATH/ --generate=json,doc
./../../TBS_Services/TBS.Services.Generator.lin/TBS_Services.Generator --def=enrollment.xml --name=Enrollment --src=$SRC_PATH/ --include=$INCLUDE_PATH/ --doc=$DOC_PATH/ --javascript=$JS_PATH/ --relative=$RELATIVE_PATH/ --generate=jsonp


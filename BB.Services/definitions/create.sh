#!/bin/bash
RELATIVE_PATH="BB/Services/"
INCLUDE_PATH="../include/BB/Services/"
SRC_PATH="../src"
DOC_PATH="../doc"
JS_PATH="../js"


./../../TBS.Services/TBS.Services.Generator.lin/TBS.Services.Generator --def=data.xml --name=Data --src=$SRC_PATH/ --include=$INCLUDE_PATH/ --relative=$RELATIVE_PATH/ --generate=dbus
./../../TBS.Services/TBS.Services.Generator.lin/TBS.Services.Generator --def=webui.xml --name=WebUI --src=$SRC_PATH/ --include=$INCLUDE_PATH/ --relative=$RELATIVE_PATH/ --javascript=$JS_PATH/ --generate=json


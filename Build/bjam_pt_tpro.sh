# configuration
. ./config_load.sh
DEST_DIRECTORY=$DEST_DIRECTORY_TPRO_TPRO

# make AppShared
#cd $SOURCE_DIRECTORY/TSDevices_Apps/Shared
#echo building: `pwd`
#if [ "$ACTION" == "clean" ]; then
#	./qmake_tpro.sh clean
#else
#	./qmake_tpro.sh $BJAM_PARAM
#fi
#cd $WORKING_DIR

# make ProductionTools
cd $SOURCE_DIRECTORY/TSDevices_Apps/ProductionTools
echo building: `pwd`
if [ "$ACTION" == "clean" ]; then
	./build_device.sh clean
else
	./build_device.sh $BJAM_PARAM
	cp ./device/debug/ProductionTools $DEST_DIRECTORY/ProductionTools
	cp -r ./data/productionTools $DEST_DIRECTORY/data/
	cp -r ./images/productionTools $DEST_DIRECTORY/data/images/
fi
cd $WORKING_DIR


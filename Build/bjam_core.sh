#!/bin/bash


# configuration
. ./config_load.sh
. ./etc/term_colors.sh
. ./etc/common_functions.sh
DEST_DIRECTORY=$DEST_DIRECTORY_TPRO

tasks_total=0
tasks_cumulative_retvals=0

ACTION="all"
BJAM_PARAM="-j2"
PLATFORM="tpro"
TYPE="release"
CWD=`pwd`

# By keeping options in alphabetical order, it's easy to add more.
while :
do
	case "$1" in
		-j*)
			BJAM_PARAM="$1"
			shift
		;;

		tpro | linpc | lindevice | lin | win)
			PLATFORM="$1"
			shift
		;;

		clean | -a)
			ACTION="$1"
			shift
		;;

		face)
			ACTION="all face"
			shift
		;;

		dbustest)
			ACTION="all dbustest"
			shift
		;;

		debug | release)
			TYPE="$1"
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

#clean destination
./clean_dest.sh $DEST_DIRECTORY

# bjam shared libraries
echo -e "${txtbld}Starting the tb2 build${txtrst}"
echo -e "${txtbld}==============================${txtrst}"
cd $SOURCE_DIRECTORY

echo "tb2 $TYPE device=$PLATFORM $BJAM_PARAM $ACTION"


tb2 $TYPE device=$PLATFORM $BJAM_PARAM $ACTION
retval=$?
echo -en "${txtbld}Result of tb2 build:${txtrst} "
print_result ${retval}
echo
((tasks_total++))
tasks_cumulative_retvals=$(( tasks_cumulative_retvals + $retval ))
cd $CWD

#copy data related files
cp -r $SOURCE_DIRECTORY/www $DEST_DIRECTORY/

echo -e "${txtbld}Build summary${txtrst}"
echo -e "${txtbld}=============${txtrst}"
echo -e "${txtrst}Top level tasks executed: ${bldcyn}${tasks_total}${txtrst}"
echo -e "${txtrst}Cumulative retval:        ${bldcyn}${tasks_cumulative_retvals}${txtrst}"
echo -en "${txtrst}Overal status:            "
print_result ${tasks_cumulative_retvals}
echo

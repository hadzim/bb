#! /bin/bash

#     /\     /\    
#    {  `---'  }   DO I HAVE YOUR ATTENTION, HUMAN?
#    {  O   O  }   I WANT YOU TO READ NOTES IN build.sh
#    ~~>  V  <~~   BEFORE YOU MODIFY THIS SCRIPT!
#     \  \|/  /
#      `-----'____
#      /     \    \_
#     {       }\  )_\_   _
#     |  \_/  |/ /  \_\_/ )
#      \__/  /(_/     \__/
#        (__/

# Internal build script for setting up variant directory. You should not use
# it directly, run build.sh instead.

# abort on first failure
set -e

# abort when reading null variable
set -u

TBS_GIT_WS="$1"
shift
TBS_VARIANT_ROOT="$1"
shift
TBS_TARGET_TOOLCHAIN="$1"
shift

TBS_VARIANT_BUILD="${TBS_VARIANT_ROOT}/bin" # location for built binaries
TBS_VARIANT_INSTALL="${TBS_VARIANT_ROOT}/install" # installed binaries
TBS_VARIANT_PACKAGE="${TBS_VARIANT_ROOT}/package" # package creation

TBS_BUILD_ENV="${TBS_VARIANT_ROOT}/build.env"

case "${TBS_TARGET_TOOLCHAIN}" in
	"tpro2000"|"tpro1000")
		TBS_TARGET_DEVICE="tpro" 
		;;
	*)
		TBS_TARGET_DEVICE="$TBS_TARGET_TOOLCHAIN"
	
esac

case "${TBS_TARGET_TOOLCHAIN}" in
	"colibri")
		TBS_ENV_FILE="/usr/local/bbx-colibri2/environment-setup-armv7at2hf-vfp-neon-angstrom-linux-gnueabi"
		TBS_OPENCV_DIR="/usr/local/bbx-colibri2/sysroots/armv7at2hf-vfp-neon-angstrom-linux-gnueabi/usr/share/OpenCV"
		;;
	"tpro1000")
		TBS_ENV_FILE="/usr/local/tbs-toolchain-gcc-4.6-i686/environment-setup-armv7a-angstrom-linux-gnueabi"
		TBS_OPENCV_DIR="/usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/share/opencv"
		;;
	"tpro2000")
		TBS_ENV_FILE="/usr/local/oecore-i686/environment-setup-cortexa8-vfp-neon-oe-linux-gnueabi"
		TBS_OPENCV_DIR="/usr/local/oecore-i686/sysroots/cortexa8-vfp-neon-oe-linux-gnueabi/usr/share/OpenCV"
		;;
	"linpc" | "lindevice")
		TBS_ENV_FILE="/usr/local/tbs-toolchain-target-i586-gcc-4.6-i686/environment-setup-i586-angstrom-linux" 
		TBS_OPENCV_DIR="/usr/local/tbs-toolchain-target-i586-gcc-4.6-i686/sysroots/i586-angstrom-linux/usr/share/opencv"
		;;
	"native")
		TBS_ENV_FILE="" 
		TBS_OPENCV_DIR="/usr/share/opencv"
		;;
esac

TBS_TOOLCHAIN_FILE="${TBS_GIT_WS}/cmake/${TBS_TARGET_TOOLCHAIN}.cmake"
if [ ! -f "${TBS_TOOLCHAIN_FILE}" ]; then
	TBS_TOOLCHAIN_FILE=""
fi

if [ -f "${TBS_BUILD_ENV}" ]; then
	echo "${TBS_BUILD_ENV} already exists, delete the directory before running prepare"
	exit -1
fi

TBS_BUILD_TOOL="make"
TBS_BUILD_TYPE="Release"

while :
do
	case "${1:-}" in
		"ninja" | "ninja-eclipse" | "make" | "make-eclipse")
			TBS_BUILD_TOOL="$1"
			shift
			;;
		"Release" | "Debug")
			TBS_BUILD_TYPE="$1"
			shift
			;;

		"")
			break
			;;
		*)
			echo "prepare error: unknown parameter $1"
			exit -1
			;;
	esac
done

# setup directories
mkdir -p "${TBS_VARIANT_ROOT}"

#setup default values
TBS_BUILD_ENV_CONTENT="# TBS Build Environment file

# __________ GENERATOR INPUT __________ 
# TBS_GIT_WS="$TBS_GIT_WS"
# TBS_VARIANT_ROOT="$TBS_VARIANT_ROOT"
# TBS_TARGET_TOOLCHAIN="$TBS_TARGET_TOOLCHAIN"

# __________ DIRECTORY STRUCTURE __________ 
TBS_VARIANT_BUILD=\"${TBS_VARIANT_ROOT}/bin\"
TBS_VARIANT_INSTALL=\"${TBS_VARIANT_ROOT}/install\"
TBS_VARIANT_PACKAGE=\"${TBS_VARIANT_ROOT}/package\"

# __________ CMAKE PARAMETERS __________ 
TBS_INSTALL_PREFIX=\"${TBS_VARIANT_INSTALL}\"
TBS_TOOLCHAIN_FILE=\"${TBS_TOOLCHAIN_FILE}\"
TBS_ENV_FILE=\"${TBS_ENV_FILE}\"
TBS_OPENCV_DIR=\"${TBS_OPENCV_DIR}\"
TBS_SOURCE_DIR=\"${TBS_GIT_WS}\"

# ___________ CONFIGURABLE CMAKE PARAMETERS __________
TBS_BUILD_TOOL=\"${TBS_BUILD_TOOL}\" # make, ninja, make-eclipse, ninja-eclipse
TBS_BUILD_TYPE=\"${TBS_BUILD_TYPE}\"
TBS_ADITIONAL_CMAKE_PARAMS=\"\" 

# ____________ CONFIGURABLE MAKE PARAMETERS __________
# or ninja parameters, of course
TBS_ADITIONAL_MAKE_PARAMS=\"\"

# ____________ OTHER PARAMETERS ________________
TBS_TARGET_TOOLCHAIN=\"$TBS_TARGET_TOOLCHAIN\"
TBS_TARGET_DEVICE=\"$TBS_TARGET_DEVICE\"

"

echo "${TBS_BUILD_ENV_CONTENT}" > "${TBS_BUILD_ENV}"



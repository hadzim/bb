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

# abort on first failure
set -e

# abort when reading null variable
set -u

TBS_VARIANT_ROOT="$1"
shift
TBS_BUILD_ENV="$1"
shift

source "${TBS_BUILD_ENV}"

CMAKE_PARAMS=()

if [ -n "${TBS_TOOLCHAIN_FILE:-}" ]; then
	CMAKE_PARAMS+=("-DCMAKE_TOOLCHAIN_FILE=${TBS_TOOLCHAIN_FILE}")
else
	echo "no toolchain file specified"
fi
		
if [ -n "${TBS_TARGET_DEVICE:-}" ]; then
	CMAKE_PARAMS+=("-DTargetDevice=${TBS_TARGET_DEVICE}")
else
	echo "no target device specified"
fi

if [ -n "${TBS_INSTALL_PREFIX:-}" ]; then
	CMAKE_PARAMS+=("-DCMAKE_INSTALL_PREFIX=${TBS_INSTALL_PREFIX}")
else
	echo "no install prefix specified"
fi

if [ -n "${TBS_OPENCV_DIR:-}" ]; then
	CMAKE_PARAMS+=("-DOpenCV_DIR=${TBS_OPENCV_DIR}")
else
	echo "no opencv dir specified"
fi

if [ -n "${TBS_BUILD_TYPE:-}" ]; then
	CMAKE_PARAMS+=("-DCMAKE_BUILD_TYPE=${TBS_BUILD_TYPE}")
else
	echo "no build type specified"
fi

case "${TBS_BUILD_TOOL:-}" in
	"make")
		CMAKE_PARAMS+=("-GUnix Makefiles")
		;;
	"make-eclipse")
		CMAKE_PARAMS+=("-GEclipse CDT4 - Unix Makefiles")
		;;
	"ninja")
		CMAKE_PARAMS+=("-GNinja")
		;;
	"ninja-eclipse")
		CMAKE_PARAMS+=("-GEclipse CDT4 - Ninja")
		;;
	"")
		echo "no build tool specified"
		;;
	*)
		echo "unsupported build tool: ${TBS_BUILD_TOOL}"
		exit -1
		;;
esac

if [ ! -d "${TBS_VARIANT_BUILD}" ]; then
	mkdir -p "${TBS_VARIANT_BUILD}"
fi
cd ${TBS_VARIANT_BUILD}

if [ -n "${TBS_ENV_FILE:-}" ]; then
	echo "using env file: ${TBS_ENV_FILE}" 
set +u
	source "${TBS_ENV_FILE}"
set -u
else
	echo "no env file specified"
fi

set -x
cmake "${CMAKE_PARAMS[@]}" ${TBS_ADITIONAL_CMAKE_PARAMS} "${TBS_SOURCE_DIR}"
set +x


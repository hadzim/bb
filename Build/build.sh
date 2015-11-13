#! /bin/bash
	
#     /\     /\    
#    {  `---'  }   DO I HAVE YOUR ATTENTION, HUMAN?
#    {  O   O  }   I WANT YOU TO READ THESE NOTES
#    ~~>  V  <~~   BEFORE YOU MODIFY THIS SCRIPT!
#     \  \|/  /
#      `-----'____
#      /     \    \_
#     {       }\  )_\_   _
#     |  \_/  |/ /  \_\_/ )
#      \__/  /(_/     \__/
#        (__/

# SOME NOTES ABOUT SCRIPT WRITING:
#
# Keep consistent versioning!
# Version 1.2.3:
# 1: major number, never change.
# 2: feature number, change when adding/changing features.
# 3: bugfix number, change when fixing bugs, etc
# The rule of thumb is that when you need to change command reference, change
# feature number. Otherwise change bugfix number.
# 
# Keep it simple. Do we really need position independent arguments, save them
# to environment and then execute, or we could just execute arguments as they
# are parsed?
#
# Prepend variable names with TBS_ to avoid collisions.
#
# Use 'set -e' in every script to make it abort on first error.
# Use 'set -u' to abort when reading null variable; use ${VAR:-} to return
# empty when null
#
# If possible, try to pass variables between scripts by parameters, not by
# environment.
#
# While brevity is good when writing code, don't be shy to write long 
# commit message (not shortlog) or wiki page ;)


#__________________SETUP_BASIC_ENVIRONMENT_AND_FIXED_VARIABLES__________________

# abort on first failure
set -e

# abort on reading null variable 
set -u

TBS_VERSION="1.4.0" # MAJOR.FEATURE.BUGFIX

# check if not a version run
if [ "${1:-}" = "--version" ] || [ "${1:-}" = "-v" ]; then
	echo "TBS Build ${TBS_VERSION}"
	exit 0
fi

# check if not a help run
if [ "${1:-}" = "--help" ] || [ "${1:-}" = "-h" ]; then
	echo "TBS Build system ${TBS_VERSION}"
	echo "This is just a quick reference."
	echo "For detailed reference (including action params) see the wiki:"
	echo "https://dulik.local.tbs-biometrics.cz/wiki/index.php?title=Build.sh"
	echo ""
	echo "Usage:"
	echo "Build/build.sh action           (non-variant actions)"
	echo "Build/build.sh variant actions  (variant-related actions)"
	echo ""
	echo "Non-variant actions reference:"
	echo "  --version -v      show version info"
	echo "  --help -h         display this help"
	echo "  --dump-config     creates initial/updates global config"
	echo "  --cfg-edit        open global config in desired editor"
	echo "  --cfg-set-var     set variable in global config (requires sed)"
	echo "  --list-variants   list all build variants"
	echo ""
	echo "Variant-related actions reference:"
	echo "  --prepare         setup build variant"
	echo "  --cfg-list        list all variant configs"
	echo "  --cfg-setup       create new variant config and open in editor"
	echo "  --cfg-set         choose variant config"
	echo "  --cfg-edit        open variant config in editor"
	echo "  --cfg-set-var     set variable in variant config (requires sed)"
	echo "  --clear           delete everything but variant config"
	echo "  --cmake           run cmake"
	echo "  --make            run selected build tool"
	echo "  --install         install"
	echo "  --build -b        run cmake make install"
	echo "  --update          update selected device"
	echo "  --pkg             create packages"
	echo "  --pkg-load        load package to device"
	exit 0
fi


# git workspace is retrieved from this script location
TBS_GIT_WS="$(cd "$(dirname $0)"; cd ..; pwd)"
TBS_SCRIPT_DIR="${TBS_GIT_WS}/Build" # location of this script


#_________________________CONFIGURATION_FILE___________________________________

# default buld root inside git repository
TBS_BUILD_ROOT="${TBS_GIT_WS}/CMakeBuildRoot"

# default notification command is echo
TBS_NOTIFICATION_COMMAND=""

# settings editor
TBS_TEXT_EDITOR="vim"

TBS_CONFIG_FILE="${TBS_SCRIPT_DIR}/build.env"
if [ -e "${TBS_CONFIG_FILE}" ]; then
	echo "loading config file ${TBS_CONFIG_FILE}"
	source "${TBS_CONFIG_FILE}"
fi

# now save the configuration if required

if [ "${1:-}" = "--dump-config" ]; then
			shift
			echo "# configuration file for build.sh" > "${TBS_CONFIG_FILE}"
			echo "" >> "${TBS_CONFIG_FILE}"
			echo "TBS_BUILD_ROOT=\"${TBS_BUILD_ROOT}\"" >> "${TBS_CONFIG_FILE}"
			echo "TBS_NOTIFICATION_COMMAND=\"${TBS_NOTIFICATION_COMMAND}\"" >> "${TBS_CONFIG_FILE}"
			echo "TBS_TEXT_EDITOR=\"${TBS_TEXT_EDITOR}\"" >> "${TBS_CONFIG_FILE}"

			if [ -z "${1:-}" ]; then
				exit 0
			fi
fi

# edit configuration file

if [ "${1:-}" = "--edit-config" ] || [ "${1:-}" = "--cfg-edit" ]; then {
	shift
	${TBS_TEXT_EDITOR} "${TBS_CONFIG_FILE}"
	exit 0
} fi

# set variable in configuration file
if [ "${1:-}" = "--cfg-set-var" ]; then {
	shift

	TBS_VAR_NAME="${1:-}"
	if [ -z "${TBS_VAR_NAME}" ]; then
		echo "missing variable name"
		exit 1
	fi
	shift

	TBS_VAR_VALUE="${1:-}"
	shift

	sed -i "s#^${TBS_VAR_NAME}=.*#${TBS_VAR_NAME}=\"${TBS_VAR_VALUE}\"#" ${TBS_CONFIG_FILE}
} fi

# get device firmware version
if [ "${1:-}" = "--fw-ver" ]; then {
	shift
	
	TBS_TARGET="${1:-}"
	if [ -z "${TBS_TARGET}" ]; then
		echo "missing target device"
		exit 1
	fi
	shift

	case "${1:-}" in 
		-*|"") 
			TBS_DESIRED_VERSION=""
			;;
		*) 
			TBS_DESIRED_VERSION=${1}
			shift
			;;
	esac

	TBS_RESULT=`curl -sS -k -uuser:4TbsPartners "https://${TBS_TARGET}/home"`
	if [ -z "${TBS_RESULT}" ]; then
		echo "failed to obtain device config mainpage"
		exit 1
	fi
	TBS_RESULT=`echo ${TBS_RESULT} | grep "Firmware Version"`
	if [ -z "${TBS_RESULT}" ]; then
		echo "failed to find firmware version"
		exit 1
	fi
	TBS_RESULT=`echo ${TBS_RESULT} | sed 's#.*<dt>Firmware Version:</dt><dd>\([^<]*\)</dd>.*#\1#'`
	if [ -z "${TBS_RESULT}" ]; then
		echo "failed to parse firmware version"
		exit 1
	fi
	
	TBS_FW_VERSION=$TBS_RESULT

	if [ -z "${TBS_DESIRED_VERSION}" ]; then
		echo "${TBS_FW_VERSION}"
	elif [ "${TBS_DESIRED_VERSION}" != "${TBS_FW_VERSION}" ]; then
		echo "device fw version '${TBS_FW_VERSION}' does not match desired version '${TBS_DESIRED_VERSION}'"
		exit 1
	fi
} fi

	

# print available variants

if [ "${1:-}" = "--list-variants" ]; then {
	shift
	ls "${TBS_BUILD_ROOT}"
	exit 0
} fi

#____________________BUILD_END_NOTIFICATION____________________________________
# This runs the notification command when the scirpt exits (not necessarily
# successfully).

TBS_PARENT_COMMAND=""

set +e 
# may fail on some platforms, but we don't want to kill the whole build
TBS_PARENT_COMMAND="$(ps -o comm= $PPID)"
set -e

case "$TBS_PARENT_COMMAND" in
	build.*.sh | build.sh)
		# parent process will notify, not this one
	;;

	"") 
		# no notification when we don't know parent process
	;;

	*)
		# notify if possible
		if [ -n "${TBS_NOTIFICATION_COMMAND:-}" ]; then
			function tbs_notify {
				exec ${TBS_NOTIFICATION_COMMAND} "TBS Build Finished"
			}

			trap tbs_notify EXIT
		fi
	;;
esac
	
#_______________________________VARIANT_CHECKING_______________________________

TBS_VARIANT="${1:-}"
case "${TBS_VARIANT}" in
	"")
		exit 0
		;;
	-*)
		echo "error: expected variant, got command (${TBS_VARIANT})"
		exit -1
		;;
esac
shift


TBS_VARIANT_ROOT="${TBS_BUILD_ROOT}/${TBS_VARIANT}" 


#_____________________PREPARE_ACTION_DEFINITION_________________________________

# Prepare build (generate build.env for given variant)
# Because it needs a "database" of some default cmake properties I isolated it
# to a different script to have the mess there and not here. 
# This action must be the only one on the command line. This is because
# you can specify aditional parameters as 'ninja' or 'Debug' to modify generated
# file. You can specify these parameters in the generated file as well, but
# it is faster directly from the command line. See the prepare
# script itself for more info.
#
# Example usage:
# Build/build.sh tpro-debug --prepare tpro ninja Debug
#
if [ "${1:-}" = "--prepare" ]; then {
	shift

	TBS_TARGET_TOOLCHAIN="${1:-}" 
	if [ -z "${TBS_TARGET_TOOLCHAIN}" ]; then
		echo "error: target toolchain not specified"
		exit -1
	fi
	shift

	"${TBS_SCRIPT_DIR}/build.prepare.sh" "${TBS_GIT_WS}" "${TBS_VARIANT_ROOT}" "${TBS_TARGET_TOOLCHAIN}" $@ 

	exit 0 # this action must always be single
} fi

# set default config
TBS_VARIANT_CONFIG="${TBS_VARIANT_ROOT}/build.env"

# list configs
if [ "${1:-}" = "--cfg-list" ]; then 
	shift

	ls "${TBS_VARIANT_ROOT}/" | grep "^build"
fi


# create new config from the default one
if [ "${1:-}" = "--cfg-setup" ]; then
	shift
	TBS_CFG_NAME="${1:-}" 
	if [ -z "${TBS_CFG_NAME}" ]; then
		echo "error: config name not specified"
		exit -1
	fi
	shift

	cp "${TBS_VARIANT_ROOT}/build.env" "${TBS_VARIANT_ROOT}/build.${TBS_CFG_NAME}.env"
	"${TBS_TEXT_EDITOR}" "${TBS_VARIANT_ROOT}/build.${TBS_CFG_NAME}.env"
fi

# choose another config
if [ "${1:-}" = "--cfg-set" ]; then 
	shift

	TBS_CFG_NAME="${1:-}" 
	if [ -z "${TBS_CFG_NAME}" ]; then
		echo "error: config name not specified"
		exit -1
	fi
	shift

	TBS_VARIANT_CONFIG="${TBS_VARIANT_ROOT}/build.${TBS_CFG_NAME}.env"

	if [ ! -f "${TBS_VARIANT_CONFIG}" ]; then
		echo "error: specified cfg file ${TBS_VARIANT_CONFIG} does not exist"
		exit -1
	fi
fi

# choose another config - direct filename
if [ "${1:-}" = "--cfg-setfile" ]; then 
	shift

	TBS_VARIANT_CONFIG="${1:-}" 
	if [ -z "${TBS_VARIANT_CONFIG}" ]; then
		echo "error: config file not specified"
		exit -1
	fi
	shift

	if [ ! -f "${TBS_VARIANT_CONFIG}" ]; then
		echo "error: specified cfg file ${TBS_VARIANT_CONFIG} does not exist"
		exit -1
	fi
fi


# Conveniently open build variant config
if [ "${1:-}" = "--edit-config" ] || [ "${1:-}" = "--cfg-edit" ]; then {
	shift

	"${TBS_TEXT_EDITOR}" "${TBS_VARIANT_CONFIG}"
} fi

# Set one config variable
if [ "${1:-}" = "--cfg-set-var" ]; then {
	shift

	TBS_VAR_NAME="${1:-}"
	if [ -z "${TBS_VAR_NAME}" ]; then
		echo "missing variable name"
		exit 1
	fi
	shift

	TBS_VAR_VALUE="${1:-}"
	shift

	sed -i "s#^${TBS_VAR_NAME}=.*#${TBS_VAR_NAME}=\"${TBS_VAR_VALUE}\"#" ${TBS_VARIANT_CONFIG}
} fi

# Since we got here it is certainly not the prepare action and the
# configuration must exist - so let's load it
source "${TBS_VARIANT_CONFIG}"

#___________________________BUILD_ACTIONS______________________________________

# Deletes variant directories (not the configuration file itself, this you 
# have to delete manually when needed)
if [ "${1:-}" = "--clear" ]; then
	shift
	rm -rf "${TBS_VARIANT_BUILD}" "${TBS_VARIANT_INSTALL}" "${TBS_VARIANT_PACKAGE}"
fi

# Run cmake with parameters given in configuration
if [ "${1:-}" = "--cmake" ]; then {
	shift
	"${TBS_SCRIPT_DIR}/build.cmake.sh" "${TBS_VARIANT_ROOT}" "${TBS_VARIANT_CONFIG}"
} fi

case "${1:-}" in "--cmake-gui")
	shift
	cmake-gui "${TBS_VARIANT_BUILD}"
	;;
esac

# Run build tool specified in configuration
if [ "${1:-}" = "--make" ] || [ "${1:-}" = "-m" ]; then {
	shift
	case "${1:-}" in 
		-*|"") 
			TBS_MAKE_PARAMS="all"
			;;
		*) 
			TBS_MAKE_PARAMS=${1}
			shift
			;;
	esac
	case "${TBS_BUILD_TOOL}" in
		"make" | "make-eclipse")
			# no quotes to allow multiple params:
			make -C "${TBS_VARIANT_BUILD}" ${TBS_MAKE_PARAMS} ${TBS_ADITIONAL_MAKE_PARAMS}
			;;
		"ninja" | "ninja-eclipse")
			# no quotes to allow multiple params:
			ninja -C "${TBS_VARIANT_BUILD}" ${TBS_MAKE_PARAMS} ${TBS_ADITIONAL_MAKE_PARAMS}
			;;
	esac
} fi

# Run a lower level install that does not depend on "all" and can choose
# a component from command line.
# TODO: decide how to choose component
if [ "${1:-}" = "--install" ]; then {
	shift
	cmake -P "${TBS_VARIANT_BUILD}/cmake_install.cmake"
} fi

#________________________POST_BUILD_ACTIONS____________________________________

# "Macro" for a typical build sequence
if [ "${1:-}" = "--build" ] || [ "${1:-}" = "-b" ]; then {
	shift

	"${TBS_SCRIPT_DIR}/build.sh" "${TBS_VARIANT}" --cfg-setfile ${TBS_VARIANT_CONFIG} --cmake --make --install
} fi


# Build is done now, what are next steps in a typical workflow?

# Update device with given IP
if [ "${1:-}" = "--update" ] || [ "${1:-}" = "-u" ]; then {
	shift

	TBS_UPDATE_PARAMS="${1:-}"
	if [ -z "${TBS_UPDATE_PARAMS}" ]; then {
		echo "error: no update params specified (at least the IP is required)"
		exit -1
	} fi
	shift

        cd "${TBS_VARIANT_INSTALL}/Default"
	# no quotes to allow multiple params:
        ./update.sh ${TBS_UPDATE_PARAMS} 
        cd -
} fi

# Create a package
if [ "${1:-}" = "--pkg" ]; then {
	shift

	case "${1:-}" in 
		-*|"") 
			TBS_PKG_COPY_PARAMS=""
			;;
		"help")
			echo usage: --pkg '"tpro|tcomte|... [other copy params]"' '["pkgname [other genpkg params]"]'
			echo "more help at https://dulik.local.tbs-biometrics.cz/wiki/index.php/Build_scripts_%26_Firmware_Packages"
			echo "exitting now..."
			exit 0
			;;
		*)
			TBS_PKG_COPY_PARAMS="${1:-}"
			shift
	esac

	case "${1:-}" in 
		-*|"") 
			TBS_PKG_GEN_PARAMS=""
			;;
		*) 
			TBS_PKG_GEN_PARAMS=${1}
			shift
			;;
	esac

	#echo "TBS_PKG_COPY_PARAMS: $TBS_PKG_COPY_PARAMS"
	#echo "TBS_PKG_GEN_PARAMS: $TBS_PKG_GEN_PARAMS"

	
	"${TBS_SCRIPT_DIR}/build.pkg.sh" "${TBS_GIT_WS}" "${TBS_VARIANT_ROOT}" "${TBS_PKG_COPY_PARAMS}" "${TBS_PKG_GEN_PARAMS}"

} fi

if [ "${1:-}" = "--pkg-load" ]; then {
	shift
	
	case "${1:-}" in 
		--*|"") 
			echo "error: package name not specified"
			exit -1
			;;
		*) 
			TBS_PKG_LOAD_NAME=${1}
			shift
			;;
	esac

	case "${1:-}" in 
		--*|"") 
			echo "error: package load target not specified"
			exit -1
			;;
		*) 
			TBS_PKG_LOAD_TARGET=${1}
			shift
			;;
	esac

	"${TBS_SCRIPT_DIR}/build.pkg-load.sh" "${TBS_VARIANT_ROOT}/packages/${TBS_PKG_LOAD_NAME}" "${TBS_PKG_LOAD_TARGET}" "${TBS_VARIANT_ROOT}/tmp"
} fi

if [ "${1:-}" = "--pkg-copy" ]; then {
	shift

	case "${1:-}" in 
		--*|"") 
			echo "error: package name not specified"
			exit -1
			;;
		*) 
			TBS_PKG_COPY_NAME=${1}
			shift
			;;
	esac

	case "${1:-}" in 
		--*|"") 
			echo "error: package destination not specified"
			exit -1
			;;
		*) 
			TBS_PKG_DST=${1}
			shift
			;;
	esac

	if [ -f "${TBS_VARIANT_ROOT}/packages/${TBS_PKG_COPY_NAME}" ]; then
		cp "${TBS_VARIANT_ROOT}/packages/${TBS_PKG_COPY_NAME}" "${TBS_PKG_DST}"
	elif [ -f "${TBS_VARIANT_ROOT}/packages/${TBS_PKG_COPY_NAME}.tbs" ]; then
		cp "${TBS_VARIANT_ROOT}/packages/${TBS_PKG_COPY_NAME}.tbs" "${TBS_PKG_DST}"
	else
		echo "cannot find package ${TBS_VARIANT_ROOT}/packages/${TBS_PKG_COPY_NAME}" 
		exit 1
	fi
} fi


#__________________________SYNTAX_CHECK________________________________________

if [ ! -z "${1:-}" ]; then {
	echo "error: command ${1} not processed"
	exit -1
} fi


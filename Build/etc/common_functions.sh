#!/bin/bash
# Some common functions (maybe)
# TN 10/2013

# XXX It's expected that term_colors.sh is loaded

print_result() {
	if [ $1 -eq 0 ]; then
		echo -en "${bldgrn}OK${txtrst}"
	else
		echo -en "${bldred}Failed${txtrst}"
	fi
}

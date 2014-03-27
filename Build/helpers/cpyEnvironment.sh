#!/bin/sh

echo "\033[38;5;148m#########################################"
echo "### COPY TO $1"
echo "#########################################\033[39m"

scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libPoco* root@$1:/usr/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libstdc++* root@$1:/usr/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libpcre.so* root@$1:/usr/lib/




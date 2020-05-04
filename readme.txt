Readme:
- build: make all
- insert module: sudo insmod Pendrive.ko
- remove existing driver: sudo modprobe -rf usb_storage
- check registered block devices: sudo fdisk -l 
- mount the pendrive after plugin:  sudo mount -t vfat /dev/Pendrive /media/external -o uid=1000,gid=1000,utf8,dmask=027,fmask=137
- go to pendrive directory: cd /media/external
- create new directory: mkdir filename
- delete a file: rm filename
- edit a file with echo: echo "text words" > filename
- unmount the pendrive:sudo umount /dev/Pendrive


INFO: task kworker/0:2:72 blocked for more than 120 seconds.
"echo 0 > /proc/sys/kernel/hung_task_timeout_secs" disables this message.

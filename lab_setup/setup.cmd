@ECHO OFF
ECHO Using SSD (S:) for VMs

MD S:\vbox
"C:\Program Files\Oracle\VirtualBox\VBoxManage.exe" setproperty machinefolder "S:\vbox"

ECHO Configuring Realtek (CISCO) adapter...

netsh int set int "CISCO" admin=enabled
netsh int ipv4 set addr "CISCO" static addr=192.168.1.10/24
netsh int ipv4 set dns "CISCO" static 192.168.1.11 validate=no

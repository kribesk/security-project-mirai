@ECHO OFF
ECHO Deleting VM and vagrant data...

RMDIR /S /Q S:\vbox
RMDIR /S /Q C:\Users\Cisco\.vagrant.d
RMDIR /S /Q C:\Users\Cisco\.VirtualBox

ECHO Reseting network adapter...

netsh int ipv4 set addr "CISCO" source=dhcp
netsh int ipv4 set dns "CISCO" source=dhcp
netsh int set int "CISCO" admin=disabled

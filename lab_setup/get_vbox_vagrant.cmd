@ECHO OFF
ECHO Downloading Vagrant and VirtualBox...
ECHO This requires wget binary

wget https://releases.hashicorp.com/vagrant/1.9.2/vagrant_1.9.2.msi --no-check-certificate
wget http://download.virtualbox.org/virtualbox/5.1.18/VirtualBox-5.1.18-114002-Win.exe

vagrant_1.9.2.msi
VirtualBox-5.1.18-114002-Win.exe

@ECHO OFF
ECHO Patching vagrant...
ECHO This requires patch binary

patch -R C:/HashiCorp/Vagrant/embedded/gems/gems/vagrant-1.9.2/lib/vagrant/util/platform.rb platform.patch

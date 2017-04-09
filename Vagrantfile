# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

    config.vm.define "mirai" do |mirai|
      mirai.vm.box = "ubuntu/xenial64"
      mirai.vm.network "public_network", bridge: "Realtek PCI GBE Family Controller", ip: "192.168.1.11"
      mirai.vm.provision "shell", path: "configs/provision.sh"
      mirai.vm.hostname = "mirai"

      mirai.vm.provider "virtualbox" do |vb|
        vb.name = "mirai"
        vb.memory = "2048"
        vb.cpus = 2
        vb.customize ["modifyvm", :id, "--uartmode1", "disconnected"]
      end
    end

    (1..10).each do |i|
      config.vm.define "target_#{i}" do |target|
        target.vm.network "public_network", bridge: "Realtek PCI GBE Family Controller", ip: "192.168.1.#{100+i}"
        target.vm.box = "olbat/tiny-core-micro"
        config.vm.box_check_update = false
        target.ssh.shell = "sh"
        target.vm.synced_folder './', '/vagrant', disabled: true
        target.vm.hostname = "target-#{i}"
        target.vm.provision "shell", path: "configs/provision_target.sh"
        
        target.vm.provider "virtualbox" do |vb|
          vb.name = "target_#{i}"
          vb.memory = "256"
          vb.cpus = 1
        end
      end
    end

end

# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
    config.vm.box = "ubuntu/xenial64"

#      config.vm.provider :digital_ocean do |provider, override|
#        provider.ssh_key_name = 'kribesk'
#        override.ssh.private_key_path = '~/.ssh/id_rsa'
#        override.vm.box = 'digital_ocean'
#        override.vm.box_url = "https://github.com/devopsgroup-io/vagrant-digitalocean/raw/master/box/digital_ocean.box"
#        provider.token = '10801a453d3e202a7484b00bb24d9853ce17bcad25f7f12c08436dcd3ec2be82'
#        provider.image = 'ubuntu-16-10-x64'
#        provider.region = 'fra1'
#        provider.size = '512mb'
#      end

    config.vm.provider "virtualbox" do |vb|
      vb.name = "mirai"
      vb.memory = "2048"
      vb.cpus = 2
      vb.customize ["modifyvm", :id, "--uartmode1", "disconnected"]
    end

    config.vm.network "public_network", bridge: "Realtek PCI GBE Family Controller", ip: "192.168.1.11"
    config.vm.provision "shell", path: "configs/provision.sh"

end

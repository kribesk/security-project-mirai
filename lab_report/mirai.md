# Mirai botnet

## Introduction

Mirai (未来) is malware designed for building large scale botnet of IoT devices. The bot and related programs was created by Anna-senpai, firstly discovered
and researched by MalwareMustDie in the end of August 2016. In reply to their blog post, one month later, Anna-sepai published sources and manual on how 
to build and run botnet, while pointing out mistakes in analysis and humiliating unixfreaxjp, author of post.

Mirai bot searches network for devices with telnet/ssh port open and bruteforces several most common and default passwords. Because users of IoT devices 
often do not much care about their security, large portion of them is vulnerable to attack. And because of large quantity of IoT devices, Mirai botnet
can grow to hundreds of thousands of hosts.

After Mirai infects host, it updates passwords to make it secure against other bots. Mirai does not install itself to target device but only stays in RAM.
This behavior was an obstacle for researches, because the only way of getting binary was dump from memory.

In attack mode Mirai has several builtin strategies: 

  0. UDP flood `ATK_VEC_UDP`
  1. Valve Source Engine query flood `ATK_VEC_VSE`
  2. DNS water torture `ATK_VEC_DNS`
  3. SYN flood `ATK_VEC_SYN`
  4. ACK flood `ATK_VEC_ACK`
  5. ACK flood to bypass mitigation devices `ATK_VEC_STOMP`
  6. GRE IP flood `ATK_VEC_GREIP`
  7. GRE Ethernet flood `ATK_VEC_GREIP`
  8. Proxy knockback connection `ATK_VEC_PROXY`
  9. Plain UDP flood optimized for speed `ATK_VEC_UDP_PLAIN`
  10. HTTP layer 7 flood `ATK_VEC_HTTP`


## Project structure

Project originaly consists of several binaries and snippets of helper code (See [Anna-sepai's original @post]). I have done small reorganizing of structure
and build scripts. Also I have added my own reports server in C so it is possible to embedded it into loader.

So the structure in brief is:

  - `/mirai/bot`: the main part, code of mirai bot written in C and cross-compiled for different possible archs used in IoT devices.
  - `/mirai/cnc`: CNC server, written in Go, used for counting size of botnet and initiating attacks, supports different users with quotes on botnet usage,
    which allows selling botnet as a service. Users are stored in MySQL database.
  - `/mirai/loader`: uploads server, receives information on compromised hosts form stdout, uses it to open telnet connections to hosts, login and get access to
    busybox, determine arch, find usable method to upload bot (tftp, wget or telnet cat with dropper) and initiate download.
  - `/mirai/dlr`: dropper that is used if target host does not have tftp or wget. This is a minimal wget-similar binary that can be quickly uploaded via telnet cat.
    cross-compiled for the same platforms as mirai. Loader searches for this binaries in `$PWD/bins` directory.
  - `/mirai/reports`: reports server that listens on 48101 TCP port for messages from bots with connection information.
  - `/mirai/tools/enc.c`: generated encrypted strings for bot that makes reverse-engeneering harder.
  - `/mirai/tools/nogdb.c`: breaks binaries in a way they are not debuggable with gdb.
  - `/mirai/tools/scanListen.go`: original reports server written in go.
  - `/mirai/tools/single_load.c`: simplified loader for testing/debuging.

## Lab environment and vagrant

To observe mirai in action, lab environment should be used. All preparation is described with Vagrant configuration and provision shell script.

Required software (Windows versions):

  * __Vagrant__ can be downloaded from [here](https://releases.hashicorp.com/vagrant/1.9.2/vagrant_1.9.2.msi)
  * __VirtualBox__ can be downloaded from [here](http://download.virtualbox.org/virtualbox/5.1.18/VirtualBox-5.1.18-114002-Win.exe)

This is the recommended configuration. Other host platforms (Linux, macOS) and virtualization providers (KVM, VMware) or cloud can be used.
This setup is described in `/lab_setup/get_vbox_vagrant.cmd` (can be executed if `wget.exe` is in path).

__Note:__ With the latest version of Vagrant and VirtualBox for Windows there is a bug (newer vbox provides long path suffix itself), this can be fixed by
`/lab_setup/platform.patch` (`/lab_setup/platform.cmd` applies this patch if `patch.exe` is in path).

MB316 configuration is described in `/lab_setup/setup.cmd` script, that can be easily adapted for other setups: 

  - using SSD for VMs can make everything work faster
  - VM uses bridge adapter, so real hardware can also be used. Script preconfigures adapter to be used as bridge. The same adapter should be set in
    Vagrantfile, but VBox uses physical name, while netsh uses logical name. 

__Note:__ Before starting vm, check that cable is connected to chosen adapter, otherwise vagrant can not bridge to it.

Vagrant describes main virtual machine used to build all the binaries and run all servers. Also it describes $N$ example virtual targets, that are used to test
mirai without any IoT hardware.

All provisioning is done with shell script:

  - install required packages
  - download crosscompilers and set paths
  - setup and initialize SQL database
  - setup dnsmasq for tftp and dns
  - build everything

Convenience script `/build.cmd` is used to build mirai server vm, `/build_vms.cmd` starts three virtual targets, `/run.cmd` starts loader and cnc servers in already
built mirai server vm, `/cnc.bat` connects to cnc server via telnet (`telnet.exe` should be in path). List of initial target hosts is placed into `/configs/hosts.txt`.

By default the following IP addresses are used (network `192.168.1.0/24`, dns server on mirai server VM):

------------------------------------------------------------------------------------ 
Host                         IP
---------------------------- ------------------------------------------------------- 
 Windows host                 `192.168.1.10`

 Mirai server vm              `192.168.1.11`

 Virtual target 1             `192.168.1.101`

 Virtual target 2             `192.168.1.102`

 ...                          ...
------------------------------------------------------------------------------------ 



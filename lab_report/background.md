# Overview of cyberthreats

## Brute force attack

Brute force is a general method of attacking against password protected resource: encrypted data or remote service. Brute force attack
means iterating over set of all possible passwords that can be used. Brute force on its own is very primitive method but still used in
some cases. For different cases there are many brute force optimizations.

The most common optimization is dictionary attack. Instead of combinatorical iteration over all values, list of "words" is used. This
attack dramatically reduced number of iterations, but if password is not in the dictionary, it does not work. For simple and dictionary 
brute force the order of iterations also matters: the more expected password is, the higher position it should have.

In one situation using "right order" of iteration can even reduce worst case time. Consider PIN-like code lock that does not have enter
key. In this case inputted keys can overlap and thus reduce number of symbols inputted. So using the correct input sequence can make brute
force much faster. Solution for this problem is de Bruijn sequence, that allows inputting $k^n + n$ symbols instead of $k^n * n$, where $k$
is size of alphabet and $n$ is length of passwords.

## Intrusion attacks

Classic type of cyber attack is attack targeting host, user's desktop or server. This type can be
typically split into individual phases [@wiki]:

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

## DoS attacks

Another type of attacks is Denial of Service attacks. They target hosts, services and networks. But their aim is not to get access to 
remote system but to flood it and exceed its resources, so that it becomes unavailable for users or long delays make it unusable [@ddos].

DDoS attacks are distributed DoS attacks: multiple hosts are used by adversary to send larger number of packets. Nowadays most of
DoS attacks are distributed. Typically attacker does not own machines used for DDoS but instead has remote access to them (for example
from intrusion attacks). Group of compromised hosts used for DDoS is called botnet. 

Emerging IoT made possible DDoS attacks with very large number of attacking devices. Most of IoT devices
lack computing power for better security, users seldom update firmware and technology is still not well developed. This allows hackers
to search for this devices and perform brute force intrusion attacks against telnet or ssh services and include them into botnet.

Common types of dos/ddos attacks [@ddos-defend]:

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

## Application layer attacks

This group of attacks target not a host or service, but application. This means firewalls, DPI tools or anitviruses can do
nothing with it. Incorrect or insecure implementations of backend and frontend cause different kinds of vulnerabilities.

The most common attacks against applications are [@web]:

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

## Mobile

Modern mobile phones run pretty same software as desktops, that means a lot of desktop oriented attacks are applicable. In addition to
those, there exist attacks related to different wireless communication medias and protocols.

Common attacks on mobile devices [@mobile]:

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

## Zero day attacks

Zero day attack is any kind of attack that uses recently published vulnerabilities and relies on usage of not up-to-date software [@zero].



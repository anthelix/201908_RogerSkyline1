Note pour roger sky line
———————————————————————————

A chaque ouverture, faire sudo apt-get update

———————————————————————————
Install VN 8 000 000 000 000
VDI
Partition 
	manuel
	sc3…
	Partition = 4.2
Hostname: Debian
Password root : pwdroot
User: polux : 2019

mise a jour:
su -c "apt update && apt upgrade" root
su -c "apt install sudo" root”
Su -c “sudo adduser castor sudo” root

Ajouter polux au shudders

Installer 
	net-tools
	htop
	ipreoute
	curl
pour installer systemctl pour sshd
Sudo apt install policykit-1
sudo apt-get install ufw
sudo apt install fail2ban
 
	


			su-	apt-get update
					upgrade
				apt-get install sudo
				exit
 

Tester les commanded du shell: https://explainshell.com/explain

			Su -c “apt-get upgrade” root

usermod -aG sudo USER
	modify a user account
	-a, --append
    	Add the user to the supplementary group(s). Use only with the -G option.
	-G, --groups GROUP1[,GROUP2,...[,GROUPN]]]. A list of supplementary groups which the user is also a member of. Each group is separated from the next by a comma, with no intervening whitespace. The groups are subject to the same restrictions as the group given with the -g option.

Ip de la machine : ip address: 
	ip : 10.11.200.53/16
	brd : 10.11.255.255


ajouterUtilisateurs pas dans les sudoers
su -c "sudo adduser castor" root
Redemarrrer

—————————————————————————————————————————————

ip a show enp0s3
ip a —> avant chaque session


Verifier le fichier INTERFACES
Sudo nano /etc/network/interfaces

Verifier la config
Sous le réseau de 42, car branche sur une machine fournissant une adresse automatique via  DHCP 
	allow-hotplug enp0s3
	iface eth0 inet dhcp
Une fois mon réseau créer:
	iface enp0s3 inet static
		address 10.1f.x.y —> 10.11.200.53/30
		netmask 30
		gateway 10.1f.254.254 -> 10.11.254.254
Apres les modifications 
Faire pour relancer 
	sudo reboot

———————————————————————
Pour verifier ssh installe cote serveur 
	apt-cache policy openssh-server	
Ou	ls -la /etc/ssh/sshd-config
Si actif
	systemctl status sshd

Verifier les ports de connection
	sudo netstat -tnplv | grep ssh
	ss -lntp | grep (+numero du port)
		-l : port a l’écoute seulement
		-n : port de facon numeriq
		-t : port Tcp seulemt
		-p : liste de sprocessud derive du port

sur le serveur, Faire une copie du fichier sshd_config
	sudo cp /etc/ssh/sshd_config /etc/ssh/sshd_config.bck
Editer le fichier ssh-config pour changer le port
	sudo vi /etc/ssh/sshd_config
	#Port 22 -> Port 1024
Redemarrer ssh
	service se trouve dans le PATH de root mais pas d un user (/usr/sbin/service) donc sudo
	sudo service sshd restart
	systemctl restart sshd
 
	
Generer les cles
Chez client
	- ssh-keygen -t rsa(/Users/schatagn/.ssh/id_rsa)poluxkeys
	- ssh-copy-id -i ~/.ssh/id_rsa.pub -p 1024 “polux@10.11.200.53”
			-p pour changer du port 22 
			-i pour le chemin des clefs


The sshd (OpenSSH Daemon) is the daemon program for ssh. Server side ssh configuration is defined in /etc/ssh/sshd_config file on Linux operating system. The ssh is the client program for sshd daemon. You need to use DenyUsers option to block access to root user on Linux. Another option to block root user access is to set PermitRootLogin to no in sshd_config file


in /etc/ssh/sshd_config
PermitRootLogin no
DenyUsers root
PasswordAuthentication no

Pour teste si efficace : ssh root@10.11.200.xxxx

———————————————————————————


sudo apt-get install ufw
Sudo uff status
#pour lister les services: 
less /etc/services
Les ports a l’ecoute 
sudo netstat -tnplv

sudo ufw enable
!!!!!!Sudo uff status verbose
sudo ufw allow 1024/tcp
sudo ufw allow http/tcp (port 80/tcp)
sudo ufw allonow 443. Authorizer https 
sudo ufw deny 22
sudo ufw reload

—————————————————
DOS, Denis de service : Ping flooding, Attaque  Ping  of Death, SYN  flood, UDP Flood
Fail2ban: fichier de configuration global /etc/fail2ban/fail2ban.conf, non modifie
Fail2ban va régulièrement lire les logs de connexion ou d’accès de votre machine et va, en fonction de règles prédéfinies (Regex),  et d’un nombre de tentatives de connexions définies à l’avance, bannir  pour un temps donné, les IP impliquées dans votre pare-feux iptables et, si vous le souhaitez, vous envoyer un mail.
Le programme fonctionne donc en modules « prisons » appelés « jail » pour chaque service que vous souhaitez surveiller tel que ssh ou apache etc…

—> configuration de base se fait dans le fichier /etc/fail2ban/jail.local, a créer
jail.local appelle les filtres de filter.d

sudo apt install fail2ban
Sudo reboot
sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local
sudo nano /etc/fail2ban/jail.local

!!!!!!!sudo systemctl restart fail2ban

sudo fail2ban-client status

bantime = Il définit en secondes combien de temps doit être bannie l’ip : 3600 = 1h
findtime = Il définit en seconde jusqu’ou dans le temps doit remonter fail2ban lors de sa lecture ou relecture des logs. 3600 = 1h
–maxretry = le nombre d’essai infructueux autorisés avant banissement

Decommenter : [sshd] et [DEFAULT]

bantime=600
findtime=600
maxretry=3

[sshd]
enabled = true
port    = 1337
???  Filter = sshd
logpath = %(sshd_log)s
backend = %(sshd_backend)s
bantime = 600
findtime = 600
maxretry = 3

—————————————————————————————————————————————————————
portsentry est un programme de détection et de blocage de "scan de ports" (généralement programme qui scanne votre machine à le recherche de ports ouverts, en général dans le but de préparer une attaque). 
Si vous choisissez le mode atcp et audp dans /etc/defaults/portsentry, inutile de préciser les ports; Portsentry va vérifier les ports utilisés et automatiquement "lier" les ports disponibles. C'est l'options la plus efficace ("a" signifie avancé). Avec cette options, portsentry établit une liste des ports d'écoute, TCP et UDP, et bloque l'hôte se connectant sur ​​ces ports, sauf s il est présent dans le fichier portsentry.ignore.

sudo apt install portsentry
Sudo reboot
Dans le fichier /etc/default/portsentry, écrire 
TCP_MODE="atcp"
UDP_MODE="audp"
sudo nano /etc/portsentry/portsentry.conf
##################
# Ignore Options #
##################


BLOCK_UDP="1"
BLOCK_TCP="1"

----______________________________________________________________________________
Lister les services:
	systemctl list-unit-files 

sudo systemctl disable console-setup.service
sudo systemctl disable keyboard-setup.service
sudo systemctl disable apt-daily.timer
sudo systemctl disable apt-daily-upgrade.timer
sudo systemctl disable syslog.service
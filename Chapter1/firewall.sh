#!/bin/sh
iptables -F
iptables -X
iptables -t nat -F
iptables -t nat -X
iptables -t mangle -F
iptables -t mangle -X
iptables -t raw -F
iptables -t raw -X
iptables -t security -F
iptables -t security -X
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT ACCEPT
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A INPUT -m state --state INVALI -j DROP
iptables -A INPUT -i lo -j ACCEPT
#iptables -A INPUT -p icmp -j ACCEPT #注意在shell脚本中’#’符号是注释，icmp是禁ping
iptables -A INPUT -p tcp -m multiport --dports 80,443  -j ACCEPT
iptables -A INPUT -s 192.168.18.0/24 -j ACCEPT

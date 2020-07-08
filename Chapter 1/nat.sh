#!/bin/sh
echo "1" > /proc/sys/net/ipv4/ip_forward  #作为网关使用时需要打开转发
#永久生效的话，需要修改sysctl.conf：net.ipv4.ip_forward = 1执行sysctl -p马上生效
iptables -F
iptables -t nat -F
iptables -X
iptables -Z
iptables -t nat -X
iptables -t nat -Z
iptables -t mangle -F
iptables -t mangle -X
iptables -t mangle -Z
iptables -t nat -A POSTROUTING -s 192.168.0.0/16 -j SNAT --to-source 1.1.1.1 
#至少两个网卡这里eth1是外网假设地址为1.1.1.1
#eth0是内网连通192.168.18.0/24网段
#如果使用拔号方式就使用 MASQUERADE这种方式比较消耗内存
#iptables -t nat -A POSTROUTING -s 192.168.0.0/16 -o ppp0 -j MASQUERADE
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT ACCEPT
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A INPUT -m state --state INVALI -j DROP
iptables -A INPUT -s 192.168.0.0/16 -j ACCEPT
iptables -A INPUT -i lo -j ACCEPT
iptables -A FORWARD -i eth1 -o eth0 -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A FORWARD -i eth1 -o eth0 -m state --state INVALI -j DROP
iptables -A FORWARD -s 192.168.18.0/24 -j ACCEPT #允许内网地址nat方式连网

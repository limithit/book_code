#!/bin/bash
#ѭ�����254��ip����ִ���ٶ�,һ���Ӻ��Զ�ɾ����ӵĹ���
for ((i=1; i<=254; i++))
 do
redis-cli TTL_DROP_INSERT 192.168.17.$i 60 
done  
redis-cli DROP_INSERT 192.168.18.5 
redis-cli DROP_DELETE 192.168.18.5 
redis-cli ACCEPT_INSERT 192.168.18.5
redis-cli ACCEPT_DELETE 192.168.18.5

#!/bin/bash
#循序插入254个ip测试执行速度,一分钟后自动删除添加的规则
for ((i=1; i<=254; i++))
 do
redis-cli TTL_DROP_INSERT 192.168.17.$i 60 
done  
redis-cli DROP_INSERT 192.168.18.5 
redis-cli DROP_DELETE 192.168.18.5 
redis-cli ACCEPT_INSERT 192.168.18.5
redis-cli ACCEPT_DELETE 192.168.18.5

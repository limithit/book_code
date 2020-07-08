#!/bin/bash             
#放到crond里一分钟执行一次
#*/1 * * * * root /bin/bash /root/oscheck.sh >> /dev/null 2>&1                                                                                  
DAT="`date +%Y%m%d`"                                                                                                                                                                                 
HOUR="`date +%H`"                                                                                                              
DIR="/home/oslog/host_${DAT}/${HOUR}"                                                                                                                                                                                       
DELAY=60                                                                                                                        
COUNT=60                                                                                                                           
#如果目录不存在就新建目录用于存放记录                                                                                    
if ! test -d ${DIR}                                                                                                             
then                                                                                                                                
mkdir -p ${DIR}                                                                                                               
fi                                                                                                                         
# 全局检查                                                                                                                                                                                                                         
top -b -d ${DELAY} -n ${COUNT} > ${DIR}/top_${DAT}.log 2>&1 &                                                       
# cpu 检查
sar -u ${DELAY} ${COUNT} > ${DIR}/cpu_${DAT}.log 2>&1 &                                                                                                                      
# memory 检查
vmstat ${DELAY} ${COUNT} > ${DIR}/vmstat_${DAT}.log 2>&1 &                                                           
# I/O 检查
iostat ${DELAY} ${COUNT} > ${DIR}/iostat_${DAT}.log 2>&1 &                                                                 
# network 检查
sar -n DEV ${DELAY} ${COUNT} > ${DIR}/net_${DAT}.log 2>&1 & 
#删除大于1天的记录                                                                                                                   
find /home/oslog/ -mtime +1  -exec rm -rf {} \; 

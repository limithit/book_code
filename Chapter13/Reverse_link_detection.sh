#!/bin/sh
#code by Gandalf 
#############白名单 ###########
if [ ! -f whitelist ]
then
echo "127.0.0.1" >  whitelist
fi
#########创建日志#######
if [ ! -f Reverse_link_detection.log ]
then
	touch Reverse_link_detection.log
fi
while :
do 
      ##过滤listen的端口并查找ESTABLISHED状态的连接，最后找到他们IP记录到日志中###
	netstat -tupln|grep -i listen|awk '{print $4}'|cut -d : -f 2|awk '{print $1}' >listen
	netstat -tapln| grep -i  established|awk '{if (NR >1){print $4}}'|sort -rn|uniq|awk '{print $1}'|cut -d : -f 2|sort -rn |uniq >ESTABLISHED
	cat ESTABLISHED | while read port 
do
        datetime=`date '+%Y-%m-%d %H:%M:%S'`
	cat listen | grep $port >> /dev/null 2>&1
	ret=$?
	if [ $ret -eq 1  ]
	then
		message=$(netstat -tapln|grep $port)
		address=$(netstat -tapln|grep $port|awk '{print $5}'|cut -d : -f 1)
		grep  $address whitelist >> /dev/null 2>&1
		sucess=$?
		if [ $sucess -eq 0 ]
		then
			continue
		elif [ $sucess -eq 1 ]
		then
		         if [ "$address" != "" ]
                        then
			echo $datetime attack from $address {$message} >>  Reverse_link_detection.log
          #用户可以选择是否邮件通知
		#	echo "$address" |mail -s "{$message}" **@139.com 
		        fi
		fi
	fi
done
sleep 3 
done

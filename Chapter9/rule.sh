#!/bin/sh
set -x               #�򿪵���
if [ $# -ne 1 ]         #����������1ʱ�˳�
then
echo  "Usage: ./autorule.sh /paths/logs/path.log"
exit 1
fi
curl -XPUT 'http://localhost:9200/nxapi/'               #�������Ȼ��ɾ��֮ǰ�ļ�¼
curl -XDELETE 'http://localhost:9200/nxapi/' -d '{
"query" : { 
	"match_all" : {}
}
}'
log_file="/tmp/generate.log"
exec 1>> "${log_file}"
exec 2>> "${log_file}"
naxsi_log=/usr/local/nginx/logs/${1}_error.log
nxtool.py -c nxapi.json  --files=$naxsi_log
sleep 1
nxtool.py -c nxapi.json -x --colors |grep '###' | awk '{print $2}' > url
servers=$(nxtool.py -c nxapi.json -x  --colors  |grep Host |awk '{print $2}')
 cat url |while read line
     do
nxtool.py -c nxapi.json --colors  -s $servers -f --filter 'uri '$line'' --slack |grep BasicRule >> ruletmp
   done
if [ -f ruletmp ]
then 
sed  -i 's/URL:/URL:\//g' ruletmp  #ȥ���ظ��Ĺ���
cat ruletmp|sort|uniq >>/usr/local/nginx/conf/conf.d/${1}.rules && rm -rf ruletmp 
fi
/usr/local/nginx/auto/learning_switch.sh naxsi ${1}        #������һ���ű��ر�ѧϰģʽ
return 0

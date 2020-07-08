#!/bin/bash
set -x 
if [ $# -ne 2 ]
then
 echo "Usage: naxsi/learning servername"
exit 1
fi
if [ "${1}" = "learning" ]       #�����ݵ��ַ�Ϊlearningʱ���ѧϰģʽ������ر�ѧϰģʽ
then
sed -i 's/naxsi/learning/' /usr/local/nginx/conf/conf.d/${2}.conf
else 
sed -i 's/learning/naxsi/' /usr/local/nginx/conf/conf.d/${2}.conf
fi
/usr/local/nginx/sbin/nginx -s reload

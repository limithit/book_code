#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>
int main(int argc, char **argv) {
    unsigned int j;                   /* ����redis����*/
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";    /*��ȡ���ӵ�ַ*/
    int port = (argc > 2) ? atoi(argv[2]) : 6379;   /* ��ȡredis�˿�*/
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds   /*���ӳ�ʱʱ��*/
c = redisConnectWithTimeout(hostname, port, timeout);
/* �������ʧ�����˳�����ӡ������ʾ*/
    if (c == NULL || c->err) {
        if (c) {
printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
printf("Connection error: can't allocate redis context\n");
        }
exit(1);
}
/*ִ��drop.insertָ���ͬiptables -I INPUT -S 192.168.18.3 -j DROP */
   reply = redisCommand(c,"drop_insert 192.168.18.3");
printf("%d\n", reply->integer);
freeReplyObject(reply);  /*�ͷ�reply���*/
/*ִ��accept.insertָ���ͬiptables -I INPUT -S 192.168.18.3 -j ACCEPT */
 reply = redisCommand(c,"accept_insert 192.168.18.4");
printf("%d\n", reply->integer);
freeReplyObject(reply);
/*ִ��drop.deleteָ���ͬiptables -D INPUT -S 192.168.18.3 -j DROP */
   reply = redisCommand(c,"drop_delete 192.168.18.3");
printf("%d\n", reply->integer);
    freeReplyObject(reply);
/*ִ��accept.deleteָ���ͬiptables -D INPUT -S 192.168.18.3 -j ACCEPT */
 reply = redisCommand(c,"accept_delete 192.168.18.5");
printf("%d\n", reply->integer);
    freeReplyObject(reply);
    redisFree(c);
    return 0;
}

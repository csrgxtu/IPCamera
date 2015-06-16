/**************************************************************************
 * 	FileName:		Cpu.c
 *	Description:	
 *	Copyright(C):	2012-2014 HHDigital Inc.
 *	Version:		V 1.0
 *	Author:			ChenYG
 *	Created:		2012-03-23
 *
 *					
 **************************************************************************/
 /*
日志:
修改内容:
1、修改main()函数的返回值和入口参数。
2、增加公司和程序版本打印标识。
3、修改打印CPU占用率的输出格式为 *******  CPU 15.8 %	只精确到0.1%,右对齐方式
修改: 	龙剑
日期:	2012年3月24日
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define		CPUFILE		"/proc/stat"
#define 	PROCFS 		"/proc"

char 		*VERSION			="V2.0.0.1 ";
char 		*BUILD_TIME_INF 	="Version: ";

#define 	Debug			(0)
#define 	Test			(1)
#define 	Release			(2)

char 		*VERSION_TYPE[3]	 	= {"Debug", "Test", "Release"};


#define CheekData(nInVal, nMin, nMax) ((((nInVal) > (nMin) )?1:0)?(((nInVal) < (nMax))?1:0):0)

int  main(int argc, char** argv)
{
	char	str[256];
	char	strname[128];					/* 字符串名 */
	/*
	user: 	从系统启动开始累计到当前时刻，用户态的CPU时间（单位：jiffies） ，不包含 nice值为负进程。1jiffies=0.01秒
 	nice:	从系统启动开始累计到当前时刻，nice值为负的进程所占用的CPU时间（单位：jiffies）
	sys:	从系统启动开始累计到当前时刻
	idle:	从系统启动开始累计到当前时刻，除硬盘IO等待时间以外其它等待时间（单位：jiffies）
	*/
	int		user, nice, sys, idle;

	
	int		user_last, nice_last, sys_last, idle_last;
	FILE	*pfile;

	int		Total_1, Total_2;
	double	Rate[128];
	double	dRate;
	double  myrate = 0;
	double	ftmp;

	char cBuf[128];

	printf("--------HIVT.Org--------\n");
	printf("----------------------------------------------\n");
	printf("      CPU [%s(%s %s)]\n", VERSION, __DATE__, VERSION_TYPE[Debug]);
	printf("----------------------------------------------\n");

	
		
	pfile = fopen(CPUFILE, "rb");
	if(pfile == NULL)
	{
		printf("Can't open %s \n", CPUFILE);
		return;
	}
	fgets(str, 255, pfile);
	fclose(pfile);

	sscanf(str,"%s %d %d %d %d",strname, &user, &nice, &sys, &idle);
	user_last = user;
	nice_last = nice;
	sys_last = sys;
	idle_last = idle;
	int i = 0;
	while(1)
	{
		for(i = 0; i < 1; i++)
		{
			usleep(500*1000);
			pfile = fopen(CPUFILE, "rb");
			if (pfile == NULL)
			{
				usleep(500*1000);
				continue;
			}
			fgets(str, 255, pfile);
			fclose(pfile);
			pfile = NULL;

			sscanf(str, "%s %d %d %d %d", strname, &user, &nice, &sys, &idle);
			
			//printf("user= %d, nice= %d, sys= %d, idle= %d \n", user,nice, sys, idle);
			
			Total_1 = user_last + nice_last+sys_last + idle_last; 
			Total_2 = user + nice + sys + idle; 
			
			ftmp = Total_2 - Total_1;

			Rate[i] = ( user + sys) - (user_last + sys_last);
		
			Rate[i] = (Rate[i] / ftmp) * 100.00;

			user_last = user;
			nice_last = nice;
			sys_last  = sys;
			idle_last = idle;
		}
		
		myrate = 0;
		
		for(i = 0; i < 1; i++)
		{
			myrate += Rate[i];		
		}		
		printf("*******  CPU %5.1lf %%\n", myrate);

	}
	printf("cpu program exit ......\n");
	return 0;
}
/*
/proc/stat中的第一行； 
每个CPU的例用率 
/proc/stat中第二行从cpu0开始,如果只有一个CPU，那就只有cpu0 
信息获取方法： 
注意cpux：后面有4列数字，依次是user, nice, sys, idle的数值，不用去关心他们是什么意思。 
对这个文件需要读取两遍，我们用total表示累计总资源，那么user+sys就是我们想要知道的累计CPU占用的资源。每个变量后面的数字表示它是第几次读文件得到的，用如下方法可算出CPU占用率： 
Total_1=user_1+nice_1+sys_1+idle_1 
Total_2=user_2+nice_2+sys_2+idle_2 
Rate={[(user_2+sys_2)-(user_1+sys_1)]/（total_2-total_1）}*100 

HISI 3512  输出例子
cpu  49036 0 64744 758565 0 1741 7889 0
cpu0 49036 0 64744 758565 0 1741 7889 0
intr 2806322 0 0 0 0 881975 0 0 0 0 0 0 0 1431 14154 0 149790 440037 438511 0 0
358 0 0 0 0 220026 440028 0 220012 0 0 0
ctxt 3229030
btime 1332502541
processes 17187
procs_running 1
procs_blocked 0

HISI 3516 输出例子
cpu  14930 0 56064 884039 1 0 16338 0 0 0
cpu0 14930 0 56064 884039 1 0 16338 0 0 0
intr 3228245 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 971620 0 0 0 0 0 0 0 0 0 0 2687 0 0 0 485131 727688 242563 0 0 0 0 0 0 0 313405 0 0 0 0 0 0 485123 16 1 11 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
ctxt 6645089
btime 1332501502
processes 122528
procs_running 1
procs_blocked 0
softirq 2756284 0 971372 3235 311076 0 0 556999 0 0 913602
*/

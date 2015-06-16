
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define		CPUFILE		"/proc/stat"
#define 	PROCFS 		"/proc"
#define		VERSION		"V2.0.0 - Release"

#define		NCPUSTATES	1


static long cp_time[NCPUSTATES];
static long cp_old[NCPUSTATES];
static long cp_diff[NCPUSTATES];
static int	cpu_states[NCPUSTATES];


#if 0
void main()
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

	int	Total_1, Total_2;
	double	Rate[128];
	double	dRate;
	double  myrate = 0;
	double	ftmp;

	char cBuf[128];

	printf("HHDigital Org\n");
	printf("--------%s--------\n", VERSION);
		
	pfile = fopen(CPUFILE, "rb");
	if(pfile == NULL)
	{
		printf("Can't open %s \n", CPUFILE);
		return;
	}
	fgets(str,255,pfile);
	fclose(pfile);
	
	sscanf(str,"%s %d %d %d %d",strname, &user, &nice, &sys, &idle);
	user_last = user;
	nice_last = nice;
	sys_last = sys;
	idle_last = idle;
	
	int count = 0;
	int i = 0;
	
	while(1)
	{
		for(i = 0; i < 1; i++)
		{
			usleep(500*1000);
			
			pfile = fopen(CPUFILE, "rb");
			fgets(str,255,pfile);
			fclose(pfile);
			
			sscanf(str,"%s %d %d %d %d",strname, &user, &nice, &sys, &idle);
			
			//printf("user= %d, nice= %d, sys= %d, idle= %d \n", user,nice, sys, idle);
			
			Total_1 = user_last + nice_last+sys_last + idle_last; 
			Total_2 = user + nice + sys + idle; 
			
			ftmp = Total_2 - Total_1;

			printf("ftmp= %05.1lf\n", ftmp);
			//printf("user + sys= %d\n", user + sys);
			//printf("user_last + sys_last= %d\n", user_last + sys_last);
			//printf("total= %d\n", user + sys - user_last - sys_last);

			//sprintf(cBuf, "%d\n", user + sys - user_last - sys_last);
			//printf("%s", cBuf);

			Rate[i] = ( user + sys) - (user_last + sys_last);
			
			printf("Rate[%d]= %lf\n", i, Rate[i]);
			//sprintf(cBuf, "%f\n", Rate[i]);
			//printf("%s", cBuf);

			
			Rate[i] = (Rate[i] / ftmp) * 100.00;
#if 0
			if (((Rate[i] - ftmp) - 0.0001) > 0)
			{			
				printf("R/F= %f\n", Rate[i] / ftmp);
				Rate[i] = (Rate[i] / ftmp) * 100.00;
				printf("Rate[%d](ftmp)= %f\n", i, Rate[i]);
			}
			else
				Rate[i]= 0.000000;
#endif			
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
		
		printf("*******  CPU %05.1lf %% \n", myrate);
		
	}
}
#endif
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

long percentages(int cnt, int *out, register long *new, register long *old, long *diffs);
int machine_init(struct statics * statics);
void get_system_info(struct system_info *info);        
void read_one_proc_stat(pid_t pid, struct top_proc *proc);
char *format_next_process(struct top_proc *p);

int main()
{
	 machine_init();
	 struct system_info info;
	 get_system_info(&info);
	 struct top_proc proc;
	 
	 
	 for(;;)
	 {
		 get_system_info(&info);
		 
		 read_one_proc_stat( (pid_t)7443, &proc);
	
		 struct top_proc *p = &proc;
	
		printf("%s\n",format_next_process(p));
	 }
}


int machine_init()
{
    /* make sure the proc filesystem is mounted */
	if (statfs(PROCFS, &sb) < 0 || sb.f_type != PROC_SUPER_MAGIC)
	{
	    printf("proc filesystem not mounted on " PROCFS "\n");
	    return -1;
	}
    return 0;
}

void get_system_info(static long *cp_time)
{
    char buffer[4096+1];
    int fd, len;
    char *p;
    int i;

    /* get the cpu time info */
    {
	fd = open("stat", O_RDONLY);
	len = read(fd, buffer, sizeof(buffer)-1);
	close(fd);
	buffer[len] = '\0';

	p = skip_token(buffer);			/* "cpu" */
	cp_time[0] = strtoul(p, &p, 0);
	
	cp_time[1] = strtoul(p, &p, 0);
	cp_time[2] = strtoul(p, &p, 0);
	cp_time[3] = strtoul(p, &p, 0);

	/* convert cp_time counts to percentages */
	percentages(4, cpu_states, cp_time, cp_old, cp_diff);
    }
}

/*
 *  percentages(cnt, out, new, old, diffs) - calculate percentage change
 *	between array "old" and "new", putting the percentages i "out".
 *	"cnt" is size of each array and "diffs" is used for scratch space.
 *	The array "old" is updated on each call.
 *	The routine assumes modulo arithmetic.  This function is especially
 *	useful on BSD mchines for calculating cpu state percentages.
 */

long percentages(int cnt, int *out, register long *new, register long *old, long *diffs)
{
    register int i;
    register long change;
    register long total_change;
    register long *dp;
    long half_total;

    /* initialization */
    total_change = 0;
    dp = diffs;

    /* calculate changes for each state and the overall change */
    for (i = 0; i < cnt; i++)
    {
	if ((change = *new - *old) < 0)
	{
	    /* this only happens when the counter wraps */
	    change = (int)
		((unsigned long)*new-(unsigned long)*old);
	}
	total_change += (*dp++ = change);
	*old++ = *new++;
    }

    /* avoid divide by zero potential */
    if (total_change == 0)
    {
	total_change = 1;
    }

    /* calculate percentages based on overall change, rounding up */
    half_total = total_change / 2l;
    for (i = 0; i < cnt; i++)
    {
	*out++ = (int)((*diffs++ * 1000 + half_total) / total_change);
    }

    /* return the total in case the caller wants to use it */
    return(total_change);
}




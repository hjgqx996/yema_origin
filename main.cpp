extern "C" 
{	
#include "4tbox_define.h"
#include "general.h"
#include "ql_oe.h"
#include "ql_eint.h"
#include "sql.h"
#include "mcu.h"
#include "ql_gpio.h"
#include "audio.h"
#include "http.h"
#include "gps.h"
#include "HU.h"

}

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h>
#include <assert.h>

#include "network.h"

#include "ctrl_air.pb.h"
#include "shellUpdate.h"
#define FIFO_NAME "/tmp/my_pipo"

pthread_t sql_thread_t,network_thread_t,plat_net_thread_t,gps_thread_t,audio_thread_t,HU_thread_t;

pthread_mutex_t tcp_mutex;
pthread_mutex_t sql_mutex;
pthread_mutex_t log_mutex;
pthread_mutex_t AT_mutex;
extern param_t param;
extern bool real_wakeup;
extern int msgid_net_to_mcu;
int at_fd = -1;

int wakelock_fd;
void mutex_init()
{
    if(pthread_mutex_init(&tcp_mutex,NULL) != 0 )  
    {  
        Log(__FUNCTION__,"Init metux error.");  
    } 
    if(pthread_mutex_init(&sql_mutex,NULL) != 0 )  
    {  
        Log(__FUNCTION__,"Init metux error.");  
    } 
    if(pthread_mutex_init(&log_mutex,NULL) != 0 )  
    {  
        Log(__FUNCTION__,"Init metux error.");  
    } 
    if(pthread_mutex_init(&AT_mutex,NULL) != 0 )  
    {  
        Log(__FUNCTION__,"Init metux error.");  
    } 
}

static int onWakeup(char* wakeup_src, void* reserved)
{
    Log(__FUNCTION__," ####### Module Wakeup !!! ####### \n");
	if(wakeup_src != NULL)
	{
		Log(__FUNCTION__,"wake up src = %s\n",wakeup_src);
	}
    return 0;
}

void gpio_init()
{
	my_gpio_eint_init();

	Ql_GPIO_Init(GPIO_GPS_PWR, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
	Ql_GPIO_SetLevel(GPIO_GPS_PWR, PINLEVEL_HIGH);

	Ql_GPIO_Init(GPIO_MCU_PWR, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
	Ql_GPIO_SetLevel(GPIO_MCU_PWR, PINLEVEL_HIGH);

//	Ql_GPIO_Init(GPIO_MUTE, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
//	Ql_GPIO_SetLevel(GPIO_MUTE, PINLEVEL_LOW);

//	Ql_GPIO_Init(GPIO_CODEC_PWR, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
//	Ql_GPIO_SetLevel(GPIO_CODEC_PWR, PINLEVEL_HIGH);

//	Ql_GPIO_Init(GPIO_BIG_VOICE, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
//	Ql_GPIO_SetLevel(GPIO_BIG_VOICE, PINLEVEL_LOW);

	wakelock_fd = Ql_SLP_WakeLock_Create("quec_wakelock_sample", sizeof("quec_wakelock_sample"));
	Ql_SLP_WakeLock_Lock(wakelock_fd);
	Ql_Autosleep_Enable(1);

}

void tts_callback(ql_tts_client_event_e_type evt,char *infostr)
{
	Log(__FUNCTION__,"tts:%d,%s",evt,infostr);
}

void init_AT()
{
    at_fd = open(_AT_PORT, O_RDWR | O_NONBLOCK | O_NOCTTY);
	Log(__FUNCTION__,"< open at port(\"%s\"), resule:%d >\n", _AT_PORT, at_fd);
//Fix try to connect IPV6 issue lead to SIM card be locked
//	Ql_SendAT((char*)"AT+CGDCONT=1,\"IP\",\"3gnet\"", (char*)"OK", 1000);
//	Ql_SendAT((char*)"AT+CGDCONT=2,\"IP\",\"3GWAP\"", (char*)"OK", 1000);
//	Ql_SendAT((char*)"AT+CGDCONT=3,\"IP\",\"WONET\"", (char*)"OK", 1000);

	//set hot plug
//	Ql_SendAT("AT+QSIMDET=1,0", "OK", 1000);
	//set usb net share	
	
	Ql_SendAT((char*)"at+qcfg=\"usbnet\",1",(char*)"OK", 10000);
	if(param.usbnet){
		system("iptables -D FORWARD -m physdev --physdev-in ecm0 -j DROP");
	}else{
		system("iptables -A FORWARD -m physdev --physdev-in ecm0 -j DROP");
	}
}

void queue_init()
{
	int msgid = msgget((key_t)MSG_ID_NET_TO_MCU,IPC_CREAT|0666);
	
	if(msgid == -1)
	{
		Log(__FUNCTION__,"message queue create failed\n");
	}

	msgid_net_to_mcu = msgget((key_t)MSG_ID_NET_TO_MCU,IPC_EXCL);
	if(msgid_net_to_mcu == -1)
	{
		Log(__FUNCTION__,"msgget failed\n");
	}

}

void childProcessFunc()
{
	system(" killall -9 mcm_ril_service mcm_data_srv MCM_MOBILEAP_ConnectionManager ");
	Log(__FUNCTION__,"child process init\n");
	if (access(FIFO_NAME, F_OK) == -1)	
	{  
		int res;
		printf("input child func \n");
		res = mkfifo(FIFO_NAME, 0777);	
		if (res != 0)  
		{  
			printf( "Could not create fifo %s\n", FIFO_NAME);
			exit(0);
		}  
	}  
	int pipe_fd;
	pipe_fd = open(FIFO_NAME, O_WRONLY);
	char str[10] = {0};
	sprintf(str,"%d\n",getpid());
	write(pipe_fd, str, strlen(str));

	//updateShell();
	init_get_device_check_param();
	gpio_init();
	mutex_init();
	queue_init();
	int ret;

	int ok = 0;  
    if((ok=mkdir("/media/sdcard/log",0755)) != 0)  
    {  
        perror("mkdir");  
    }  
	system("find /media/sdcard/ -mtime +2 -name \'*.log\' -exec rm {} \\;");

	Log(__FUNCTION__,"tbox version: %s %s %s",APP_VERSION,__DATE__,__TIME__);
	get_param();
	init_AT();
      
	pthread_t uart_thread_t,module_init_thread_t;
	ret = pthread_create(&module_init_thread_t,NULL,module_init_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"gps thread create failed!\n");
	}
	ret = pthread_create(&gps_thread_t,NULL,gps_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"gps thread create failed!\n");
	}
	ret = pthread_create(&sql_thread_t,NULL,sql_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"sql thread create failed!\n"); 
	}
	ret = pthread_create(&uart_thread_t,NULL,uart_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"Uart thread create failed!\n"); 
	}
	ret = pthread_create(&network_thread_t,NULL,TCP_Program,(void*)param.url);
	if(ret != 0)
	{
		Log(__FUNCTION__,"Network thread create failed!\n");
	}
	
	ret = pthread_create(&HU_thread_t,NULL,HU_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"HU thread create failed!\n");
	}
	while(1)
	{
		sleep(20); //20S  send heart to father thread
		write(pipe_fd, str, strlen(str));
		Log(__FUNCTION__,"child send heart\n");
	}

}

void forkChildProcess(int sigo)
{
    int status = 0;
	int pid;
    // 等待子进程中断或终止，释放子进程资源
    // 否则死掉的子进程会变成僵尸进程
    pid = wait(&status);

    // 等待3秒钟重新启动子进程
    sleep(3);

	
    pid = fork();
    if (pid == 0)
    {
        printf("Fork new child process\n");
        childProcessFunc();
    }
}

int initWatchDog()
{
    int pid = fork();
    if (pid)
    {
        // 父进程一直监视子进程的运行状态
        while (1)
        {
        assert(signal(SIGCHLD, forkChildProcess) != SIG_ERR); //get child out sigal and initial

		int pipe_fd;
		int len,timeout=0;
		char childPid[10] = {0};
		char buf[10] = {10};
		while(access(FIFO_NAME, F_OK) == -1) sleep(1);
		pipe_fd = open(FIFO_NAME, O_RDONLY);
		while(1)
		{
			int nfds;
			fd_set readfds;
			struct timeval tv;
			tv.tv_sec = 20;
			tv.tv_usec = 0;
			FD_ZERO(&readfds);
			FD_SET(pipe_fd, &readfds);
			nfds = select(pipe_fd+1, &readfds, NULL, NULL, &tv);
			if(nfds > 0 && FD_ISSET(pipe_fd,&readfds)){
				memset(buf,0,sizeof(buf));
				len = read(pipe_fd, buf, 10);
				if(len > 0)
				{
					timeout = 0;
					if(strcmp(childPid,buf)){
						memcpy(childPid,buf,10);
					}
					Log(__FUNCTION__,"get child heart\n");
				}else{
					sleep(1);	
				}
			}else{
				timeout++;
				if(timeout > 3){ // 60s recv heart will kill child
					int pid = atoi(childPid);
					kill(pid,SIGABRT);
					timeout = 0;
					
					Log(__FUNCTION__,"kill child\n");
				}
			}

		}

			
		// 捕获子进程结束信号
		// 父进程挂起，当有信号来时被唤醒
		pause();
        }
    }
    else if (pid < 0)
    {
        return 0;
    }

    return 1;
}
int main()
{
    printf("Main pid: %d\n", getpid());
    // 初始化看门狗进程
    int ret = initWatchDog();
    if (!ret)
    {
        printf("Init watch dog failed\n");
        return 1;
    }
    printf("Init watch dog success...\n");
    // 运行子进程代码
    childProcessFunc(); 

    return 0;
}


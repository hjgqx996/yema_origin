#include "ctrl_air.pb.h"

//#include "tbox_abstract.h"

extern "C" 
{
#include     <stdio.h>     
#include     <stdlib.h>     
#include     <unistd.h>     
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     "string.h"
#include     <fcntl.h>      
#include     <termios.h>    
#include     <errno.h>  
#include 	 <string.h>  
#include 	 <sys/un.h>  
#include 	 <sys/ipc.h>
#include 	 <stdbool.h> 
#include	 <pthread.h>
#include 	 <sys/ioctl.h>
#include 	 <sys/time.h>
#include 	 <errno.h>
#include 	 <sys/msg.h>
#include	 "4tbox_define.h"
#include 	 "sql.h"
#include 	 "network.h"
#include 	 "gps.h"

#include 	 <unistd.h>
#include 	 "ql_gpio.h"
#include 	 "ql_sleep_wakeup.h"
#include 	 "ql_oe.h"
#include 	 "ql_eint.h"
#include 	 "general.h"
}

#include "gpExtendQueue.h"


pthread_mutex_t mcu_send_mutex;
static int mcu_fd;
extern pthread_t sql_thread_t;
extern pthread_t network_thread_t;
extern pthread_t gps_thread_t;
static int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
        B38400, B19200, B9600, B4800, B2400, B1200, B300, };
static int name_arr[] = {115200, 38400,  19200,  9600,  4800,  2400,  1200,  300, 38400,  
        19200,  9600, 4800, 2400, 1200,  300, };

extern bool sql_thread_cancel;
extern bool network_thread_cancel;
extern bool gps_thread_cancel;
extern bool connect_ok;
extern car_data_t car_data;
static bool alert_start = false;
bool mcu_ready = false;
static int msgid_from_net;
static pthread_t sleep_thread_t;
bool real_wakeup = true;
int voice_num = 0;
extern int wakelock_fd;
extern unsigned char mcu_version[5];
extern unsigned char hardware_version[32];

extern param_t param;
extern int period;
extern bool mcu_upgrade;
extern int sock_cli;
extern int msgid_net_to_mcu;
extern upgrade_param_t upgrade_param;
extern tbox_extend_t tbox_extend;
extern int gpsRecvFirstDataStatus;

int get_remote_status_num = 0;

unsigned char air_state[20] = {0};
unsigned char door_state[4] = {0};
unsigned char tire_preture[8] = {0};
unsigned char oil_mass[2]={0};

unsigned char EOL[32] = "eol";
unsigned char pipe_Skey[32] = "pipe_Skey";

gl_data_t gl_data;

bool recv_unfinish = false;
gpExtendQueue *extendQueue;


/*int check_log_size()
{
	DIR *d;
	struct dirent *de;
	struct stat buf;
	int exists;
	int total_size;

	d = opendir("./log");
	if (d == NULL) {
	perror("prsize");
	exit(1);
	}

	total_size = 0;

	for (de = readdir(d); de != NULL; de = readdir(d)) {
	exists = stat(de->d_name, &buf);
	if (exists < 0) {
	  fprintf(stderr, "Couldn't stat %s\n", de->d_name);
	} else {
	  total_size += buf.st_size;
	}
	}
	closedir(d);
	Log(__FUNCTION__,"%d\n", total_size);
	return total_size;
}*/

/*********************************************************************/
int OpenDev(char *Dev)
{
	int fd = open( Dev, O_RDWR | O_NOCTTY);         //| O_NOCTTY | O_NDELAY    
	if (-1 == fd)   
	{           
		perror("Can't Open Serial Port");
		return -1;      
	}   
	else    
		return fd;
}

void set_speed(int fd, int speed)
{
        unsigned int   i; 
        int   status; 
        struct termios   Opt;
        tcgetattr(fd, &Opt); 
        for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) 
		{ 
                if  (speed == name_arr[i]) 
				{     
                        tcflush(fd, TCIOFLUSH);     
                        cfsetispeed(&Opt, speed_arr[i]);  
                        cfsetospeed(&Opt, speed_arr[i]);   
                        status = tcsetattr(fd, TCSANOW, &Opt);  
                        if  (status != 0) {        
                                perror("tcsetattr fd1");  
                                return;     
                        }    
                        tcflush(fd,TCIOFLUSH);   
                }  
        }
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{ 
	struct termios options; 
	if  ( tcgetattr( fd,&options)  !=  0) 
	{ 
		perror("SetupSerial 1");     
		return(FALSE);  
	}
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/
    options.c_cflag &= ~CSIZE; 
	options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        switch (databits)
        {   
                case 7:     
                        options.c_cflag |= CS7; 
                        break;
                case 8:     
                        options.c_cflag |= CS8;
                        break;   
                default:    
                        fprintf(stderr,"Unsupported data size/n"); return (FALSE);  
        }
        switch (parity) 
        {   
                case 'n':
                case 'N':    
                        options.c_cflag &= ~PARENB;   /* Clear parity enable */
                        options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
                        break;  
                case 'o':   
                case 'O':     
                        options.c_cflag |= (PARODD | PARENB); 
                        options.c_iflag |= INPCK;             /* Disnable parity checking */ 
                        break;  
                case 'e':  
                case 'E':   
                        options.c_cflag |= PARENB;     /* Enable parity */    
                        options.c_cflag &= ~PARODD;   
                        options.c_iflag |= INPCK;       /* Disnable parity checking */
                        break;
                case 'S': 
                case 's':  /*as no parity*/   
                        options.c_cflag &= ~PARENB;
                        options.c_cflag &= ~CSTOPB;
						break;  
                default:   
                        fprintf(stderr,"Unsupported parity/n");    
                        return (FALSE);  
        }  

        switch (stopbits)
        {   
                case 1:    
                        options.c_cflag &= ~CSTOPB;  
                        break;  
                case 2:    
                        options.c_cflag |= CSTOPB;  
                        break;
                default:    
                        fprintf(stderr,"Unsupported stop bits/n");  
                        return (FALSE); 
        } 
        /* Set input parity option */ 
        if (parity != 'n')   
                options.c_iflag |= INPCK; 
        tcflush(fd,TCIFLUSH);
        options.c_cc[VTIME] = 150; // 500ms time out
        options.c_cc[VMIN] = 0; //Update the options and do it NOW 
        if (tcsetattr(fd,TCSANOW,&options) != 0)   
        { 
                perror("SetupSerial 3");   
                return (FALSE);  
        } 
        return (TRUE);  
}

ssize_t mcu_read(int fd, unsigned char *buf)
{
	int	nfds;
	unsigned char temp[1024];
	fd_set	readfds;
	struct timeval  tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500000;
	/*FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	nfds = select(fd+1, &readfds, NULL, NULL, &tv);
	if (nfds <= 0)
	{
		if (nfds == 0)
		{
			errno = ETIME;
		}
		return -1;
	}*/
	int headAreadyLen = 0,needReadLen = 0,ret;
	while(headAreadyLen != 5)
	{
	
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		nfds = select(fd+1, &readfds, NULL, NULL, &tv);	
		if (nfds <= 0)
		{
			if (nfds == 0)
			{
				errno = ETIME;
			}
			return -1;
		}
		ret = read(fd, temp+headAreadyLen, 5-headAreadyLen);
		if(ret<0)
		{
			return -1;
		}
		if(!ret)
		{
			usleep(100000);
			continue;
		}		  
		if(ret>0)
		{
			headAreadyLen += ret;
		}
	}
	//ÅÐ¶ÏÊÇ·ñÎªÆðÊ¼Í·
	if(temp[0] == 0xFF && (temp[1] == 0xBB || temp[1] == 0xCC || temp[1] == 0xAA))
	{
		if(temp[1] == 0xCC)
		{
			needReadLen = (temp[3] << 8) + temp[4]+1 ; //crcä½
		}
		else
		{
			needReadLen = temp[3]; //header only 4 buf already read 5 so not +1
		}

		
		int alreadyLen = 0;
		//¼õÈ¥Ö®Ç°¶ÁµÄ5×Ö½ÚÊý¾Ý
		while(needReadLen != alreadyLen )
		{		
			FD_ZERO(&readfds);
			FD_SET(fd, &readfds);
			nfds = select(fd+1, &readfds, NULL, NULL, &tv);
			if (nfds <= 0)
			{
				if (nfds == 0)
				{
					errno = ETIME;
				}
				return -1;
			}
			ret = read(fd,temp+5+alreadyLen,needReadLen - alreadyLen);
			
			if(ret<0)
			{
				return -1;
			}
			if(!ret)
			{
				usleep(100000);
			} 	  
			if(ret>0)
			{
				alreadyLen += ret;
			}
		}

		memcpy(buf,temp,needReadLen+5);
		return needReadLen+5;
	}
	else
	{
		//Çå¿Õ»º´æÇø
		tcflush(fd,TCIFLUSH);
		return -1;
	}
}


int mcu_send(unsigned char* buf,int len)
{
	int nread;
	if(mcu_upgrade){
		return 1;
	}
	unsigned char temp[512] = {0};
	memcpy(temp,buf,len);
	temp[len] = check_sum(buf + 2,len - 2);
	int i;
	Log_print(__FUNCTION__,"mcu send:");
	for(i = 0; i < len + 1;i++)
	{
		Log_print("[%X]",temp[i]);
	}
	Log_print("\n");
	pthread_mutex_lock(&mcu_send_mutex);
	usleep(100000);
	int alreadyWrite = 0;	
	while(alreadyWrite !=len+1){
    	nread = write(mcu_fd, temp+alreadyWrite ,len + 1-alreadyWrite);
		if(nread<0){
			break;
		}
		if(nread == 0){
			usleep(10000);			
			continue;
		}
		if(nread >0){
			alreadyWrite += nread;
		}
	}
	pthread_mutex_unlock(&mcu_send_mutex);
	return nread;
}

int upgrade_mcu_send(unsigned char* buf,int len)
{
	int nread;
	unsigned char temp[512] = {0};
	memcpy(temp,buf,len);
	temp[len] = check_sum(buf + 2,len - 2);
	int i;
	Log_print(__FUNCTION__,"mcu send:");
	for(i = 0; i < len + 1;i++)
	{
		Log_print("[%X]",temp[i]);
	}
	Log_print("\n");
	pthread_mutex_lock(&mcu_send_mutex);
	usleep(100000);
	int alreadyWrite = 0;	
	while(alreadyWrite !=len+1){
    	nread = write(mcu_fd, temp+alreadyWrite ,len + 1-alreadyWrite);
		if(nread<0){
			break;
		}
		if(nread == 0){
			usleep(10000);			
			continue;
		}
		if(nread >0){
			alreadyWrite += nread;
		}
	}
	pthread_mutex_unlock(&mcu_send_mutex);
	return nread;
}

void PC_Power_cmd(unsigned char cmd)
{
	unsigned char temp[7] = {CMD_HEAD,CMD_NOACK,CMD_POWER+0x80,0x03,0x01,STATUS_DATA,0x00};
	temp[6] = cmd;
	upgrade_mcu_send(temp,sizeof(temp));
}

void mcu_queue_send(unsigned char* buf,int len)
{
	struct net_msg_stru msg;
	msg.type = 1;
	msg.len = len;
	memcpy(msg.data,buf,len);
	if(msgsnd(msgid_net_to_mcu,(void*)&msg,sizeof(struct net_msg_stru),IPC_NOWAIT) == -1)
	{
		perror("msgsnd failed");
	}
	else
	{
		Log_print(__FUNCTION__,"msgsnd to mcu success\n");
	}		
}

void find_car_cmd()
{
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x03,0x01};
	if(mcu_ready)
	{
		mcu_send(temp,8);
	}
	else
	{
		mcu_queue_send(temp,8);
	}
}

void engine_remote_cmd(unsigned char value){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x01};
	temp[7] = value;
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);	
	}

}

void engine_start_cmd(unsigned char value)
{
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x04};
	temp[7] = value;
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}
}

void electromotor_ctrl_cmd(bool status){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x02};
	if(status){
		temp[7] = 2;
	}else{
		temp[7] = 1;
	}	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}
}

void charge_start_cmd(unsigned char value)
{
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x0B};
	temp[7] = value; 	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}
}

void remote_ctrl_enable_cmd(unsigned char value)
{
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x01};
	temp[7] = value;		
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}
}

void windows_ctrl_cmd(unsigned char place,unsigned char value)
{
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01};
	if(place == 0x0f || place == 0x10)
	{
		if(place == 0x0f)
		{
			temp[6] = 0x09;
		}

		if(place == 0x10)
		{
			temp[6] = 0x0a;
		}
		temp[7] = value;
		if(mcu_ready)
		{
			mcu_send(temp,8);	
		}
		else
		{
			mcu_queue_send(temp,8);
		}
	}	
}

void doors_ctrl_cmd(unsigned char place,unsigned char value){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,07};
	temp[7] = value;
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}

}

void chair_ctrl_cmd(unsigned char place,unsigned char value){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01};
	if(place & 0x1 || place & 0x02)
	{
		if(place & 0x1)
		{
			temp[6] = 0x08;
			if(value & 0x1)
			{
				temp[7] = 1;
			}
			else
			{
				temp[7] = 2;
			}	
			if(mcu_ready)
			{
				mcu_send(temp,8);	
			}
			else
			{
				mcu_queue_send(temp,8);
			}
		}
	}	
}

void panel_protection_cmd(unsigned char value){	
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x0D};
	temp[7] = value;
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}
}

void air_a_key_defrost(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x01};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_ac_max(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x02};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_heat_max(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x03};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_anion(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x04};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_cabin_clean(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x05};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_auto(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x06};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_purify_start(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x07};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_purify_stop(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x08};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_stop(){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x05,0x0f};
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}	
}

void air_set_temperature(unsigned char temperature){
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x03,0x01,0x06};
	temp[7] = temperature;
	
	if(mcu_ready)
	{
		mcu_send(temp,8);	
	}
	else
	{
		mcu_queue_send(temp,8);
	}		
}


void check_vehicle_status(){
	unsigned char temp0[8] = {0xFF,0xBB,0x82,0x04,0x02,0x01,0x10,0x01};
	mcu_send(temp0,8);
	unsigned char temp[8] = {0xFF,0xBB,0x82,0x04,0x02,0x01,0x10,0x02};
	mcu_send(temp,8);
	unsigned char temp1[8] = {0xFF,0xBB,0x82,0x04,0x02,0x01,0x10,0x03};
	mcu_send(temp1,8);	
}

int str_to_i(unsigned char *buf,int length)
{
	int i;
	int sum = 0; 
	for(i = 0;i < length;i++,sum = sum << 8)
	{
		sum += buf[i];
	}
	return sum >> 8;
}

unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;  
    FILE *fp;  
    fp = fopen(path, "r");  
    if(fp == NULL)  
        return filesize;  
    fseek(fp, 0L, SEEK_END);  
    filesize = ftell(fp);  
    fclose(fp);  
    return filesize;  
}  

#define PACKAGE_SIZE	256
void upgrade(void)
{
	car_data.vehicle.car_status = CAR_STATUS_OFF;
	int nread;
	unsigned char buff[128];
	
	int i;
	Log(__FUNCTION__,"uart thread start\n");
	unsigned long file_size = get_file_size("/home/root/mcu.bin");
	Log(__FUNCTION__,"file_size = %ld\n",file_size);
	short package_sum = file_size/PACKAGE_SIZE;
	if(file_size%PACKAGE_SIZE > 0)
	{
		package_sum++;
	}
	Log(__FUNCTION__,"package_sum = %d\n",package_sum);
	int package_size = 0;
	int req_package_num = 0;
	FILE *fd = fopen("/home/root/mcu.bin","rb");
	if(fd == NULL)
	{
		Log(__FUNCTION__,"open failed\n");
	
    }

	int step = 0;
	//request
	unsigned char step0_send[13] = {0xFF,0xBB,0x85,0x09,0x03,0x01,0x07,0x00,0x00,0x00,0x00,0x00,0x00};
	step0_send[7] = (file_size >> 24) & 0xFF;
	step0_send[8] = (file_size >> 16) & 0xFF;
	step0_send[9] = (file_size >> 8) & 0xFF;
	step0_send[10] = file_size & 0xFF;
	step0_send[11] = (package_sum >> 8) & 0xFF;
	step0_send[12] = package_sum & 0xFF;
	unsigned char step0_ack[8] = {0xFF,0xBB,0x05,0x04,0x01,0x01,0x07,0x01};
	//start send
	unsigned char step1_req[7] = {0xFF,0xBB,0x05,0x05,0x02,0x01,0x08};
	unsigned char step1_send[512] = {0xFF,0xCC,0x85,0x00,0x00,0x01,0x01,0x08};
	unsigned char step1_send_end[512] = {0xFF,0xBB,0x85,0x00,0x01,0x01,0x08};
	//finish
	unsigned char step2_req[8] = {0xFF,0xBB,0x05,0x04,0x01,0x01,0x09,0x01};
	unsigned char step2_send[7] = {0xFF,0xBB,0x85,0x03,0x01,0x01,0x09};
	
	unsigned char read_buf[256] = {0};
	while (1) 
	{   
		switch(step)
		{
		case 0://request
			//FF  AA  F2  01  00
			//FF  55  F2  01
			step0_send[7] =((file_size>>24)&0xff);
			step0_send[8] =((file_size>>16)&0xff);
			step0_send[9] =((file_size>>8)&0xff);
			step0_send[10] =(file_size&0xff);
			step0_send[11] = ((package_sum>>8)&0xff);
			step0_send[12] = (package_sum&0xff);
			upgrade_mcu_send(step0_send,sizeof(step0_send));
			if((nread = mcu_read(mcu_fd, buff))>0)
			{
				for(i = 0;i < nread;i++)
				{			
	 				Log_print("%X ",buff[i]);
				}
				Log_print("\n");
				if(!memcmp(buff,step0_ack,sizeof(step0_ack)))
				{
					step = 1;
				}
			}
			else
			{
				Log(__FUNCTION__,"timeout\n");
			}
			break;
		case 1://start
			if((nread = mcu_read(mcu_fd, buff))>0)
			{
				for(i = 0;i < nread;i++)
				{			
	 				Log_print("%X ",buff[i]);
				}
				Log_print("\n");

				if(!memcmp(buff,step1_req,sizeof(step1_req)))
				{
					req_package_num = (buff[7] << 8) + buff[8];
					if(req_package_num > 0)
					{
						fseek(fd,(req_package_num-1)*256,SEEK_SET);
					}
					memset(read_buf,0,sizeof(read_buf));
					package_size = fread(read_buf,1,sizeof(read_buf),fd);	
					if(package_size >= 251)  //251 + 5
					{
						Log_print(__FUNCTION__,"package_size = %d\n",package_size);
						step1_send[3] = ((package_size+5) >> 8) & 0xFF;
						step1_send[4] = (package_size+5) & 0xFF;
						step1_send[8] = (req_package_num >> 8) & 0xFF;
						step1_send[9] = req_package_num & 0xFF;
						memcpy(step1_send+10,read_buf,package_size); 

						upgrade_mcu_send(step1_send,package_size + 10);
					}else if(package_size > 0){						
						Log_print(__FUNCTION__,"package_size = %d\n",package_size);						
						step1_send_end[3] = package_size+5;
						step1_send_end[7] = (req_package_num >> 8) & 0xFF;
						step1_send_end[8] = req_package_num & 0xFF;
						memcpy(step1_send_end+9,read_buf,package_size); 
						
						upgrade_mcu_send(step1_send_end,package_size + 9);
					}
					step = 1;
				}
				else if(!memcmp(buff,step2_req,sizeof(step2_req)))
				{
					upgrade_mcu_send(step2_send,sizeof(step2_send));
					fclose(fd);
					step = 2;
				}else{
					return ;
				}
			}
			else
			{
				Log(__FUNCTION__,"timeout\n");
			}
			break;
		case 2:		
			Log(__FUNCTION__,"mcu upgrade success\n");
			upgrade_param.mcu_upgrade_status = 0;				
			init_save_upgrade();
			system("sync");
			return ;
		}
	}
}

int alert_start_count = 0;
int alert_stop_count = 0;
unsigned int oldAlertData = 0;
unsigned int nowAlertData = 0;
int alertClear = 0;
int tmpAlterNum = 0;
void decideAlert(unsigned char *buf){
	if(alert_start){
		alert_start_count++;
		//printf("######################################## %d %d\n",alert_start_count,param.before_alert_time);
		if(alert_start_count > param.alert_time){
			//Log(__FUNCTION__,"-------------alert finish-----------------------\n");
			alert_start_count = 0;
			alert_start = false;
		}
	}else{
		if(alertClear++ > 3600){
			//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ alertClear = %d\n",alertClear);
			alertClear = 0;
			oldAlertData = 0;
		}

		nowAlertData = (unsigned int)buf[7];	
		nowAlertData <<= 8;
		nowAlertData += (unsigned int)buf[8];
		nowAlertData <<= 8;
		nowAlertData += (unsigned int)buf[9];
		nowAlertData <<= 8;
		nowAlertData += (unsigned int)buf[11];
		//printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ alert = %d %d\n",nowAlertData,oldAlertData);
		//if(buf[6] == 0x3 && nowAlertData != oldAlertData){
		if(tmpAlterNum++ > 150){
			//Log(__FUNCTION__,"-----------------------alert start! ------------------------\n");
			sql_fault_mark();
			tmpAlterNum = 0;
			period = 0;
			alert_start = true;
			oldAlertData = nowAlertData;
		}
	}
}


void Deal_CAN(unsigned char* mcu_buf)
{
	//FF BB 58  01 1A  00  07...
	if(mcu_buf[2] == CMD_TBOX_CAR_INFO)
	{
		/*
		if(param.GB32690_type != e_off)
		{
		
			mcu_buf[8] = 0x00;
			if(param.GB32690_type == e_alert)
			{
				if((alert_start_count++ > (param.before_alert_time+20)) && (alert_stop_count < param.alert_time))
				{
					mcu_buf[8] = 0x03;
					mcu_buf[11] = 0x08;
					mcu_buf[34] = 0x00;
					mcu_buf[35] = 0x01;
					alert_stop_count++;
					param.data_period = 1;
					if(!alert_start)
					{
						sql_fault_mark();
						period = 0;
						alert_start = true;
						Log(__FUNCTION__,"-----------------------alert start! ------------------------\n");
					}
				}
				
				if(alert_stop_count == param.alert_time)
				{
					mcu_buf[8] = 0x00;
					mcu_buf[11] = 0x00;
					mcu_buf[34] = 0x02;
					mcu_buf[35] = 0xE6;
					alert_stop_count++;
					Log(__FUNCTION__,"-------------alert finish-----------------------\n");
				}
				else if(alert_stop_count > param.alert_time)
				{
				
					alert_start = false;
					param.data_period = 10;
				}
			}
		}
		else
		{*/
			//if((mcu_buf[8] == 0x03)&&(mcu_buf[7] == 0x07))
			decideAlert(mcu_buf);
			if(alert_start)
			{
				param.data_period = 1;
			}
			else
			{
				//alert_start = false;
				param.data_period = 10;
			}
		//}
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;
		int mcu_len;
		if(mcu_buf[1] == 0xBB){
			mcu_len = mcu_buf[3] ;
		}
		else if(mcu_buf[1] == 0xCC){
			mcu_len = (mcu_buf[3] << 8) + mcu_buf[4];
		}

		get_current_date(&date);

		head_pack(data,&offset,DATA_CAR_MSG_SEND,DATA_CMD,NO_ENCRYPT);
		date_pack(data,date,&offset);
		correctGps();
		location_data_pack(data,car_data.location,&offset);
		if(mcu_buf[1] == 0xBB){		
			memcpy(data+offset,mcu_buf+4,mcu_len);
		}
		else if(mcu_buf[1] == 0xCC){
			memcpy(data+offset,mcu_buf+5,mcu_len);
		}
		offset += mcu_len;
		
		//extend data
		extendQueue->pushNewData(&car_data.location);
		unsigned int aMileage;
		aMileage = (unsigned char)data[56];
		aMileage<<=8;
		aMileage += (unsigned char)data[57];
		aMileage<<=8;
		aMileage += (unsigned char)data[58];
		aMileage<<=8;
		aMileage += (unsigned char)data[59];
		
		
		get_extend_vechicle_param()->accumulatedMileage = aMileage;
		get_extend_vechicle_param()->soc = data[64];
		
		memcpy(gl_data.data,data,offset);
		gl_data.len = offset;
	}
}

void Deal_Air_Condition_Status(unsigned char *mcu_buf){
	air_condition_param_t *all_vehicle_air_condition_param;
	//*front_vehicle_air_condition_param,*rear_vehicle_air_condition_param;
	all_vehicle_air_condition_param = get_air_conditon(ALL_VEHICLE);
	//front_vehicle_air_condition_param = get_air_conditon(FRONT_VEHICLE);
	//rear_vehicle_air_condition_param = get_air_conditon(ALL_VEHICLE);

	unsigned int temp_status;
	int stair_num = mcu_buf[5];//second_num;
	int offset = 6;
	air_condition_param_t *temp_air_condition_param;
	int current_stair_num=0;//current_seconde_num=0;
	temp_air_condition_param = all_vehicle_air_condition_param;
	
	while(stair_num > current_stair_num){
		printf("offset = %d\n",offset);
		switch(mcu_buf[offset++]){
			case 0x1:
				temp_status = (unsigned int)mcu_buf[offset++];
				temp_air_condition_param->ac_status = temp_status;
				Log(__FUNCTION__,"air condition status ac_status = %d",temp_air_condition_param->ac_status);
				break;	
			case 0x2:	
				temp_status = (unsigned int)mcu_buf[offset++];
				temp_air_condition_param->heating_status = temp_status;
				Log(__FUNCTION__,"air condition status refrigeration_or_heating = %d",temp_air_condition_param->heating_status);
				break;
			case 0x3:			
				temp_air_condition_param->temperature = mcu_buf[offset++]+36;			
				Log(__FUNCTION__,"air condition temperature = %d",temp_air_condition_param->temperature);
				break;
			case 0x4:
				//temp_air_condition_param->temperature = mcu_buf[offset++];
				offset++;
				break;
			case 0x5:				
				temp_status = (unsigned int)mcu_buf[offset++];
				temp_air_condition_param->windLevel = temp_status;
				Log(__FUNCTION__,"air condition wind = %d",temp_air_condition_param->temperature);
				break;
			case 0x6:				
				temp_status = (unsigned int)mcu_buf[offset++];
				temp_air_condition_param->blow_model = temp_status;
				Log(__FUNCTION__,"air condition status blow_model = %d",temp_air_condition_param->blow_model);
				break;
			case 0x7:			
				temp_status = (unsigned int)mcu_buf[offset++];
				temp_air_condition_param->air_circulation_mode = temp_status;
				Log(__FUNCTION__,"air condition status air_circulation_mode = %d",temp_air_condition_param->air_circulation_mode);
				break;
			case 0x8:
				temp_status = (unsigned int)mcu_buf[offset++];
				temp_air_condition_param->forward_defront_status = temp_status;
				Log(__FUNCTION__,"air condition status forward_defront_status = %d",temp_air_condition_param->forward_defront_status);
				break;
			case 0x9:	
				temp_status = (unsigned int)mcu_buf[offset++];
				temp_air_condition_param->behind_defront_status = temp_status;
				Log(__FUNCTION__,"air condition status behind_defront_status = %d",temp_air_condition_param->behind_defront_status);
				break;
			default :
				offset++;
				break;
		}
		current_stair_num++;
	}

	ack_air_ctrl();
}

void Deal_CAD_Status(unsigned char *mcu_buf){
/*
	extend_vehicle_param_t *temp_extend_vehicle_param = get_extend_vechicle_param();
	unsigned int temp_status;
	int checkNum = mcu_buf[5];
	int status_type;//offset = 6,
	while(checkNum != 0){		
		status_type = mcu_buf[6+(checkNum-1)*2];
		Log_print(__FUNCTION__,"status type = %d status = %d\n",status_type,mcu_buf[6+(checkNum-1)*2+1]);
		switch(status_type){
			case 0x1:
				temp_status = (unsigned int)mcu_buf[6+(checkNum-1)*2+1];
				temp_extend_vehicle_param-> = temp_status;		
				Log_print(__FUNCTION__,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& engine_enable_status = %d\n",temp_extend_vehicle_param->ekey_status);
				break;
			case 0x4:				
				temp_status = (unsigned int)mcu_buf[6+(checkNum-1)*2+1];
				temp_extend_vehicle_param->engine_status = temp_status ;
				Log_print(__FUNCTION__,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& engine_status = %d\n",temp_extend_vehicle_param->engine_status);
				ack_engine_start();
				break;
			
			case 0x7:
				if(mcu_buf[6+(checkNum-1)*2+1] == 1){
					temp_extend_vehicle_param->door_lock_status = 0xf;
				}else{
					temp_extend_vehicle_param->door_lock_status = 0x0;
				}
				
				ack_control();
				Log_print(__FUNCTION__,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& door_lock_status = %d\n",temp_extend_vehicle_param->door_lock_status);
				break;
			case 0x8:
				
				if(mcu_buf[6+(checkNum-1)*2+1] == 1){
					temp_extend_vehicle_param->seet_heating_status = true;
				}else{
					temp_extend_vehicle_param->seet_heating_status = false;
				}
				Log(__FUNCTION__,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& seet_heating_status = %d\n",temp_extend_vehicle_param->seet_heating_status);
				ack_control();

				break;
			
			case 0x9:
				
				if(mcu_buf[6+(checkNum-1)*2+1] == 1){
					temp_extend_vehicle_param->window_status = 0xf;
				}else{
					temp_extend_vehicle_param->window_status = 0x0;
				}
				
				ack_control();
				Log_print(__FUNCTION__,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& window_status = %d\n",temp_extend_vehicle_param->window_status);
				break;
			case 0xb:
				
				if(mcu_buf[6+(checkNum-1)*2+1] == 1){
					temp_extend_vehicle_param->charge_status = true;
				}else{
					temp_extend_vehicle_param->charge_status = false;
				}
				
				ack_control();
				Log_print(__FUNCTION__,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& charge_status = %d\n",temp_extend_vehicle_param->charge_status);
			    break;
			
			case 0xd:			
				if(mcu_buf[6+(checkNum-1)*2+1] == 1){
					temp_extend_vehicle_param->control_panel_protection_status = true;
				}else{
					temp_extend_vehicle_param->control_panel_protection_status = false;
				}
				Log_print(__FUNCTION__,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& control_panel_protection_status = %d\n",temp_extend_vehicle_param->control_panel_protection_status);
				ack_control();

				break;
			default:

				break;
		}
	
		checkNum--;
	}
*/
}

void Deal_Basic_Information_check(unsigned char *mcu_buf){
	int checkNum = mcu_buf[5];
	unsigned char temp[200] = {CMD_HEAD,CMD_NOACK,PC_BASIC_INFOMATION,0x0,STATUS_DATA,mcu_buf[5]};
	int offset = 6;
	while(checkNum != 0){
		temp[offset++] = mcu_buf[5+checkNum];
		if(mcu_buf[5+checkNum] == 0x1){
			memcpy(&temp[offset],param.VIN,17);
			offset += 17;
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0x2){
			
			temp[offset++] = '\0';
		}
/*		if(mcu_buf[5+checkNum] == 0x3){
			memcpy(&temp[offset],HARDWARE_VERSION,strlen(HARDWARE_VERSION));
			offset += strlen(HARDWARE_VERSION);
			temp[offset++] = '\0';
		}*/
		if(mcu_buf[5+checkNum] == 0x4){
			memcpy(&temp[offset],param.ICCID,20);
			offset += 20;
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0x5){
			memcpy(&temp[offset],param.IMEI,strlen(param.IMEI));
			offset += strlen(param.IMEI);			
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0x6){
			memcpy(&temp[offset],param.IMSI,strlen(param.IMSI));
			offset += strlen(param.IMSI);
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0x7){
			memcpy(&temp[offset],APP_VERSION,strlen(APP_VERSION));
			offset += strlen(APP_VERSION);
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0x8){
			memcpy(&temp[offset],FIREWARE_VERSION,strlen(FIREWARE_VERSION));
			offset += strlen(FIREWARE_VERSION);
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0x9){
			temp[offset++] = get_device_check_param()->emmc_check_status;
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0xA){
			temp[offset++] = get_device_check_param()->tsp_check_status;		
			temp[offset++] = '\0';
		}
		if(mcu_buf[5+checkNum] == 0xB){
			temp[offset++] = get_device_check_param()->gps_check_status;		
			temp[offset++] = '\0';
		}
		checkNum--;
	}

	temp[3] = offset-4;   //å¤?ID+é•¿åº¦
	mcu_send(temp, offset);
}

void Deal_Basic_Infomation_Status(unsigned char *buf)
{
	int idNum = buf[5];
	int offset = 5;
	int startMark;
	
	while(idNum > 0){
		offset++;
		if(buf[offset] == 1){
			offset++;
			startMark = offset;
			char temp[100] = {0};
			while(buf[offset] != '\0'){
				temp[offset-startMark] = buf[offset];     //VIN
				offset++;
			}

			//if(strcmp(temp,param.VIN) && strcmp("00000000000000000",temp)){
				memcpy(param.VIN,temp,17);
				Log(__FUNCTION__,"VIN:%s",param.VIN);
				init_save_param(PARAM_PATH);
				init_save_param(BAK_PARAM_PATH);
				if(connect_ok)
				{
					logout(sock_cli,param.login_serial_num);
					param.login_serial_num++;					
					login(sock_cli,param.login_serial_num);
				}
			//}
		}

		// ÏÂÎ»»úÈí¼þ°æ±¾
		if(buf[offset] == 2){
			Log(__FUNCTION__,"mcu verion");
			offset++;
			startMark = offset;
			memset(mcu_version,0,sizeof(mcu_version));
			while(buf[offset] != '\0'){
				mcu_version[offset-startMark] = buf[offset];
				offset++;
			}
		}
		// Ó²¼þ°æ±¾
		if(buf[offset] == 3)
		{				
			offset++;
			startMark = offset;	
			memset(hardware_version,0,sizeof(hardware_version));
			while(buf[offset] != '\0'){
				hardware_version[offset-startMark] = buf[offset];
				offset++;
			}
		}

		if(buf[offset] == 0xc){
			offset++;
			startMark = offset;	
			memset(EOL,0,sizeof(EOL));
			while(buf[offset] != '\0'){
				EOL[offset-startMark] = buf[offset];
				offset++;
			}
		}

		if(buf[offset] == 0xd){
			offset++;
			startMark = offset;	
			memset(pipe_Skey,0,sizeof(pipe_Skey));
			while(buf[offset] != '\0'){
				pipe_Skey[offset-startMark] = buf[offset];
				offset++;
			}
		}
		
		idNum --;
	}
}

void Deal_Vehicle_Param(unsigned char *mcu_buf){
	int checkNum = mcu_buf[5];
	int status_type;//offset = 6,
	int offset = 6;
	unsigned int tempStatus;
	while(checkNum != 0){	
		
		status_type = mcu_buf[offset++];
		switch(status_type){
			case 0x1:
				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_exceprtion_status |=0x00000001 ;
				}else{			
					get_extend_vechicle_param()->door_exceprtion_status &=0xfffffffe ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_status |=0x00000001 ;	
					else 
						get_extend_vechicle_param()->door_status &=0xfffffffe ;
				}
				offset++;

				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_exceprtion_status |=0x00000002 ;
				}else{
					get_extend_vechicle_param()->door_exceprtion_status &=0xfffffffd ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_status |=0x00000002 ;
					else			
						get_extend_vechicle_param()->door_status &=0xfffffffd ; 

				}
				offset++;

				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_exceprtion_status |=0x00000004 ;
				}else{
					get_extend_vechicle_param()->door_exceprtion_status &=0xfffffffB ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_status |=0x00000004 ;
					else				
						get_extend_vechicle_param()->door_status &=0xfffffffB ; 

				}
				offset++;

				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_exceprtion_status |=0x00000008 ;
				}else{
					get_extend_vechicle_param()->door_exceprtion_status &=0xfffffff7 ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_status |=0x00000008 ;
					else 
						get_extend_vechicle_param()->door_status &=0xfffffff7 ;
				}
				offset++;

				Log(__FUNCTION__,"door  status1 = %X %X",get_extend_vechicle_param()->door_status,get_extend_vechicle_param()->door_exceprtion_status);
				break;
				
			case 0x2:
				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_lock_exception_status |=0x00000001 ;
				}else{
					get_extend_vechicle_param()->door_lock_exception_status &=0xfffffffe ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_lock_status |=0x00000001 ;	
					else 
						get_extend_vechicle_param()->door_lock_status &=0xfffffffe ;
				}
				offset++;

				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_lock_exception_status |=0x00000002 ;
				}else{
					get_extend_vechicle_param()->door_lock_exception_status &=0xfffffffd ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_lock_status |=0x00000002 ;
					else			
						get_extend_vechicle_param()->door_lock_status &=0xfffffffd ; 

				}
				offset++;

				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_lock_exception_status |=0x00000004 ;
				}else{
					get_extend_vechicle_param()->door_lock_exception_status &=0xfffffffB ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_lock_status |=0x00000004 ;
					else				
						get_extend_vechicle_param()->door_lock_status &=0xfffffffB ; 

				}
				offset++;

				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_lock_exception_status |=0x00000008 ;
				}else{		
					get_extend_vechicle_param()->door_lock_exception_status &=0xfffffff7 ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_lock_status |=0x00000008 ;
					else 
						get_extend_vechicle_param()->door_lock_status &=0xfffffff7 ;

				}
				offset++;
				ack_door_start();		
				Log(__FUNCTION__,"door lock status1 = %X %X",get_extend_vechicle_param()->door_lock_status,get_extend_vechicle_param()->door_lock_exception_status);
				break;
			case 0x3:
				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_exceprtion_status |=0x00000010 ;
				}else{		
					get_extend_vechicle_param()->door_exceprtion_status &=0xffffffef ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_status |=0x00000010 ;
					else 
						get_extend_vechicle_param()->door_status &=0xffffffef ;

				}
				offset++;
				Log(__FUNCTION__,"door  status2 = %X %X",get_extend_vechicle_param()->door_status,get_extend_vechicle_param()->door_exceprtion_status);
				
				break;
			case 0x4:
				if(mcu_buf[offset] == 0xff){
					get_extend_vechicle_param()->door_lock_exception_status |=0x00000010 ;
				}else{		
					get_extend_vechicle_param()->door_lock_exception_status &=0xffffffef ;
					if(mcu_buf[offset] == 1)
						get_extend_vechicle_param()->door_lock_status |=0x00000010 ;
					else 
						get_extend_vechicle_param()->door_lock_status &=0xffffffef ;

				}
				offset++;	
				
				ack_truck_lock_ctrl();
				Log(__FUNCTION__,"door lock status2 = %X %X",get_extend_vechicle_param()->door_lock_status,get_extend_vechicle_param()->door_lock_exception_status);
				break;
			case 0x5:
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_status[0] = tempStatus;				
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_status[1] = tempStatus;
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_status[2] = tempStatus;
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_status[3] = tempStatus;
				
				Log(__FUNCTION__,"windows lock status = %d",get_extend_vechicle_param()->window_status);
				break;
			case 0x6:
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_position[0] = tempStatus;				
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_position[1] = tempStatus;
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_position[2] = tempStatus;
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->window_position[3] = tempStatus;

				break;
		
			case 0x7:
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->sunroof_curtain_status = tempStatus;
				
				Log(__FUNCTION__,"sun_Curtain_Staus status = %d",get_extend_vechicle_param()->sunroof_curtain_status);
				break;
			case 0x8:
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->sunroof_glass_status = tempStatus;

				Log(__FUNCTION__,"sun_Glass_Status status = %d",get_extend_vechicle_param()->sunroof_glass_status);
				break;
			
			case 0x9:
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->sunroof_glass_position = tempStatus;
				break;
			case 0xa:
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->high_beam_status = tempStatus;
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->low_beam_status = tempStatus;	
				Log(__FUNCTION__,"high_low_beam_status status = %d",get_extend_vechicle_param()->high_beam_status);
				
				Log(__FUNCTION__,"high_low_beam_status status = %d",get_extend_vechicle_param()->low_beam_status);
				break;
			case 0xb:				
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->fog_light_status = tempStatus;
				offset++;
				Log(__FUNCTION__,"fog_light_statusstatus = %d",get_extend_vechicle_param()->fog_light_status);
			    break;
			case 0xc:
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->left_cornering_lamp = tempStatus;
				tempStatus = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->right_cornering_lamp = tempStatus;	
				
				Log(__FUNCTION__,"right_left_cornering_lamp status = %d %d",get_extend_vechicle_param()->left_cornering_lamp);
				Log(__FUNCTION__,"right_left_cornering_lamp status = %d %d",get_extend_vechicle_param()->right_cornering_lamp);
			    break;
			case 0xd:
				offset++;
				break;
			default:

				break;
		}
		checkNum --;
	}	
}

void Deal_Key_parameter(unsigned char* mcu_buf){
	int checkNum = mcu_buf[5];
	unsigned int temp_status;
	int status_type;//offset = 6,
	int offset = 6;
	while(checkNum != 0){		
		status_type = mcu_buf[offset++];
		switch(status_type){
			case 0x1:
				temp_status = (unsigned int)mcu_buf[offset++];
				get_extend_vechicle_param()->engine_status = temp_status;
				ack_engine_start();
				Log(__FUNCTION__,"engine_status = %d",get_extend_vechicle_param()->engine_status);	
				break;
			case 0x2:				
				temp_status = (unsigned int )mcu_buf[offset++];
				get_extend_vechicle_param()->electrify_status = temp_status;
				if(get_extend_vechicle_param()->electrify_status != 1){
					get_extend_vechicle_param()->engine_status = get_extend_vechicle_param()->electrify_status;
					ack_engine_start();	
				}	
				Log(__FUNCTION__,"door electrify_status = %d",get_extend_vechicle_param()->electrify_status);
				break;
			case 0x3:
				offset++;
				break;
			case 0x4:
				tbox_extend.rechargeMileage = mcu_buf[offset++]<<8;
				tbox_extend.rechargeMileage += mcu_buf[offset++];
				tbox_extend.rechargeMileage *= 5;
				get_extend_vechicle_param()->rechargeMileage = tbox_extend.rechargeMileage;
				break;
			case 0x5:
				get_extend_vechicle_param()->averageConsumption =mcu_buf[offset++];
				break;
			case 0x6:
				offset++;
				offset++;
				break;
		
			case 0x7:
				offset++;
				offset++;
				offset++;
				break;
			case 0x8:
				offset++;
				break;
			
			case 0x9:
				temp_status = (unsigned int )mcu_buf[offset++];
				//if(temp_status == 2){
				//	get_extend_vechicle_param()->door_lock_status &= 0xfffffff0; 
				//}else{
				//	get_extend_vechicle_param()->door_lock_status |= 0x0f;
				//}
				
				//ack_door_start();
				//Log(__FUNCTION__,"engine_enable_status = %d",get_extend_vechicle_param()->vehicle_body_defence_status);
				break;
			case 0xa:
				temp_status = (unsigned char)mcu_buf[offset++];
				get_extend_vechicle_param()->charge_status = temp_status;			
				//ack_charge_start();
				Log(__FUNCTION__,"charge_status = %d",get_extend_vechicle_param()->charge_status);
				break;
			case 0xb:
				temp_status = (unsigned int )mcu_buf[offset++];
				get_extend_vechicle_param()->control_panel_protection_status = temp_status;
				//ack_panel_protecion_ctrl();
				Log(__FUNCTION__,"control_panel_protection_status = %d ",get_extend_vechicle_param()->control_panel_protection_status);
			    break;
			
			default:
				break;
		}
		checkNum --;
	}

}


void Deal_Alarm(unsigned char* mcu_buf)
{
	int alertNum = mcu_buf[5];
	int alert_type;
	while(alertNum > 0){
		alert_type = mcu_buf[6+(alertNum-1)*2];
		if(alert_type == 0x02){				
			collision_alert();
		}

		if(alert_type == 0x03){			
			lilegal_encroachment();
		}
		alertNum--;
	}
}

void start_ql_timer()
{	
	QL_Timer_Start(QL_TIMER_ID_10, SLEEP_HEART_TIME*60000,1);
}

void stop_ql_timer()
{
	QL_Timer_Stop(QL_TIMER_ID_10);
}

int change_to_msg_mode_timer = 0;
bool heart_beat_ack = false;
int sleep_mode = 0;

int timer_check_network(ql_timer_id timer_id, void *params)
{
	Ql_SLP_WakeLock_Lock(wakelock_fd);
	void ** status = NULL;
	int ret,kill_rc;
	//Èç¹û5 * 2·ÖÖÓÃ»ÓÐÊÕµ½ÍøÂç»½ÐÑµÄÖ¸Áî£¬ÇÐ»»³É¶ÌÐÅ»½ÐÑÄ£Ê½
	if(++change_to_msg_mode_timer >= SWITCH_MSG_WAKEUP_MOD)
	{
		Log(__FUNCTION__,"Switch to msg wakeup mode!");
		kill_rc = pthread_kill(network_thread_t,0);
		if(kill_rc != ESRCH)
		{
			network_thread_cancel = true;
			pthread_join(network_thread_t,status);
			Log(__FUNCTION__,"network stop\n");
		}
		
		stop_ql_timer();
		sleep_mode = SWITCH_MSG_WAKEUP_MOD;
		connect_ok = false;
		Ql_SLP_WakeLock_Unlock(wakelock_fd);
		return 1;
	}
	//·¢ËÍÈý´ÎÐÄÌø°ü
	int heart_send_timeout = 3;
	heart_beat_ack =false;
	while(heart_send_timeout-- > 0)
	{
		heart_beat(sock_cli);
		//µÈ´ý2s£¬ÊÇ·ñÓÐÐÄÌø°ü·µ»Ø
		int heart_beat_ack_timeout = 20;
		while(heart_beat_ack_timeout-- > 0)
		{
			if(heart_beat_ack == true)
			{
				break;
			}
			usleep(100000);
		}
		if(heart_beat_ack_timeout > 0)
		{
			break;
		}
	}
	if(heart_send_timeout > 0)
	{
		Log(__FUNCTION__,"tcp network is OK,go on for sleep!");
	}
	else
	{
		//ÖØÆôÍøÂçÏß³Ì
		Log(__FUNCTION__,"Restart network!");
		if(network_thread_t != 0)
		{
			kill_rc = pthread_kill(network_thread_t,0);
			if(kill_rc != ESRCH)
			{
				network_thread_cancel = true;
				pthread_join(network_thread_t,status);
			}
			
			connect_ok = false;
			ret = pthread_create(&network_thread_t,NULL,TCP_Program,(void*)param.url);
			if(ret != 0)
			{
				Log(__FUNCTION__,"Network thread create failed!\n");
			}
		}
		//ÖØÆôÏß³ÌºóµÈ´ý40s,¿´ÍøÂçÊÇ·ñ»Ö¸´
		int network_restart_timeout = 40;
		while(network_restart_timeout-- > 0)
		{
			heart_beat(sock_cli);
			if(heart_beat_ack == true)
			{
				break;
			}
			sleep(1);
		}
		if(network_restart_timeout > 0)
		{
			Log(__FUNCTION__,"Network recover,go on sleep!");
		}
		else
		{
			Log(__FUNCTION__,"Network not good,Switch to msg wakeup mode!");
			stop_ql_timer();
			kill_rc = pthread_kill(network_thread_t,0);
			if(kill_rc != ESRCH)
			{
				network_thread_cancel = true;
				pthread_join(network_thread_t,status);
			}
			
			connect_ok = false;
			sleep_mode = SWITCH_MSG_WAKEUP_MOD;
		}
	}
	Log(__FUNCTION__,"ql timer !\n");
	Ql_SLP_WakeLock_Unlock(wakelock_fd);
	return 1;
}

void init_ql_timer()
{	
	QL_Timer_Stop(QL_TIMER_ID_10);
	QL_Timer_Register(QL_TIMER_ID_10, timer_check_network, NULL);
}



void* sleep_thread(void* sleep_param)
{
	//FF BB 81 01 02 82 
	void ** status = NULL;
	int ret,kill_rc;
	//prepare to sleep	
	Log(__FUNCTION__,"try to cancel thread");
	if(connect_ok)
	{
		logout(sock_cli,param.login_serial_num);
		param.login_serial_num++;	
		init_save_param(PARAM_PATH);
		init_save_param(BAK_PARAM_PATH);
	}

	if(sql_thread_t != 0)
	{
		kill_rc = pthread_kill(sql_thread_t,0);
		if(kill_rc != ESRCH)
		{
			sql_thread_cancel = true;
			pthread_join(sql_thread_t,status);	
		}
	}
	if(gps_thread_t != 0)
	{		
		kill_rc = pthread_kill(gps_thread_t,0);
		if(kill_rc != ESRCH)
		{
			gps_thread_cancel = true;
			pthread_join(gps_thread_t,status);				
		}
		gpsRecvFirstDataStatus = false;

		car_data.location.status = 1;
		car_data.location.latitude = 0;
		car_data.location.longitude = 0;
	}
	ql_wifi_disable();
	
	init_ql_timer();
	start_ql_timer();	
	PC_Power_cmd(PC_SLEEP);

	Ql_GPIO_SetLevel(GPIO_MCU_PWR, PINLEVEL_LOW);
	Ql_GPIO_SetLevel(GPIO_GPS_PWR, PINLEVEL_LOW);
	extendQueue->clearGPSQueue();
	Log(__FUNCTION__,"mcu send go to sleep");
	usleep(100000);
	mcu_ready = false;	
	real_wakeup = false;
	while(!real_wakeup)
	{
		if(change_to_msg_mode_timer >= SWITCH_MSG_WAKEUP_MOD)
		{
			//stop_ql_timer();
			//change_to_msg_mode_timer = 0;
		}
		Ql_SLP_WakeLock_Unlock(wakelock_fd);
		usleep(1000);
	}
	Ql_SLP_WakeLock_Lock(wakelock_fd);
	change_to_msg_mode_timer = 0;
	
	stop_ql_timer();
	if(connect_ok)
	{
		login(sock_cli,param.login_serial_num);
	}
	sleep(1);
	
	Log(__FUNCTION__,"mcu wackup");
	Ql_GPIO_SetLevel(GPIO_MCU_PWR, PINLEVEL_HIGH);
	Ql_GPIO_SetLevel(GPIO_GPS_PWR, PINLEVEL_HIGH);
	ql_wifi_enable();
	//delete log more than 6 days if the directory is bigger than 100M	
	ret = system("find ./ -mtime +2 -name \'*.log\' -exec rm {} \\;");
	
	ret = pthread_create(&sql_thread_t,NULL,sql_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"sql thread create failed!\n"); 
	}
	ret = pthread_create(&gps_thread_t,NULL,gps_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"gps thread create failed!\n");
	}
	if(sleep_mode == SWITCH_MSG_WAKEUP_MOD)
	{
		ret = pthread_create(&network_thread_t,NULL,TCP_Program,(void*)param.url);
		if(ret != 0)
		{
			Log(__FUNCTION__,"Network thread create failed!\n");
		}

	}
	return 0;
}

void Deal_mcu_cmd(unsigned char* buf,int len)
{
	unsigned char ret = check_sum(buf + 2,len - 3);
	if(ret != buf[len - 1])
	{
		Log(__FUNCTION__,"check sum failed\n");
		return;
	}
	if(buf[2] == MCU_AIR_CONDITION)
	{
		Deal_Air_Condition_Status(buf);
	}

	if(buf[2] == MCU_CAD)
	{
		Deal_CAD_Status(buf);
	}
	
	if(buf[2] == MCU_BASIC_INFOMATION)
	{
		if(buf[4] == CHECK_DATA){
			Deal_Basic_Information_check(buf);
		}

		if(buf[4] == STATUS_DATA){
			Deal_Basic_Infomation_Status(buf);
		}

		//check_vehicle_status();
	}
	
	if(buf[2] == MCU_ALERT)
	{
		Deal_Alarm(buf);
	}
	
	//Log(__FUNCTION__,"check sum ok\n");
	else if(buf[2] == 0x05)
	{
		if(buf[6] == 0x03)
		{
			mcu_ready = true;
			unsigned char temp[9] = {CMD_HEAD,CMD_NOACK,PC_BASIC_INFOMATION,0x05,CHECK_DATA,0x03,0x01,0x02,0x03};
			//unsigned char temp[8] = {CMD_HEAD,CMD_NOACK,PC_BASIC_INFOMATION,0x04,CHECK_DATA,0x02,0x02,0x03};
			mcu_send(temp, sizeof(temp));
			check_vehicle_status();
		}

		get_upgrade();

		if(upgrade_param.mcu_upgrade_status){
					int ret = down_load_verify((char*)upgrade_param.mcu_upgrade_sign,(char*)upgrade_param.mcu_upgrade_path);
					if(ret){			
						if(mcu_upgrade == false)
						{
							if(mcu_upgrade == false)
							{
								system("mv /home/root/tbox_decrypt /home/root/mcu.bin");
								system("sync");
								mcu_upgrade = true;
							}
						}
					}		
		}	

		
		else if(buf[6] == 0x05 && buf[7] == 0x02)
		{
			get_upgrade();
			if(upgrade_param.mcu_upgrade_status){/*
					int ret = down_load_verify((char*)upgrade_param.mcu_upgrade_sign,(char*)upgrade_param.mcu_upgrade_path);
					if(ret){			
						if(mcu_upgrade == false)
						{
							if(mcu_upgrade == false)
							{
								system("mv /home/root/tbox_decrypt /home/root/mcu.bin");
								system("sync");
								mcu_upgrade = true;
							}
						}
					}	*/
					mcu_upgrade = true;
			}else{
				if(real_wakeup)
				{
					if(sleep_thread_t != 0)
					{
						void ** status = NULL;
						int kill_rc;
						kill_rc = pthread_kill(sleep_thread_t,0);
						if(kill_rc != ESRCH)
						{
							pthread_join(sleep_thread_t,status);	
						}
					}
					ret = pthread_create(&sleep_thread_t,NULL,sleep_thread,NULL);
					if(ret != 0)
					{
						Log(__FUNCTION__,"sleep thread create failed!\n"); 
					}
			    }
			}
		}
	}
	else if(buf[2] == KEY_PARAM){
		Deal_Key_parameter(buf);
	}else if(buf[2] == VEHICLE_PARAM){
		Deal_Vehicle_Param(buf);
	}
	else// if((buf[2] == CMD_TBOX_CAR_INFO)||(buf[2] == CMD_EXTRA_CAR_STATE))
	{
		//gsensor FF BB 5A 02 01 05 5C
		Deal_CAN(buf);
	}
}

void* uart_thread(void* uart_param)
{
    extendQueue = new gpExtendQueue();
    if(pthread_mutex_init(&mcu_send_mutex,NULL) != 0 )  
    {  
        Log(__FUNCTION__,"Init metux error.");  
    } 
	Log(__FUNCTION__,"uart_thread\n");
	int mcu_len;
	unsigned char mcu_buf[1024] = {0};
	char *dev  = (char*)"/dev/ttyHSL1";
    Log(__FUNCTION__,"uart thread start\n");
	msgid_from_net = msgget((key_t)MSG_ID_NET_TO_MCU,IPC_EXCL);
	if(msgid_from_net == -1)
	{
		Log(__FUNCTION__,"msgget failed\n");
	}
	struct net_msg_stru msg;
	mcu_fd = OpenDev(dev);
	set_speed(mcu_fd,115200);
	if (set_Parity(mcu_fd,8,1,'N') == FALSE)  
	{
		Log(__FUNCTION__,"Set Parity Error/n");
		return NULL;
	}
	int i;	
	mcu_ready = false;
	while (1) 
	{   
		if((!mcu_ready) && real_wakeup)
		{
			PC_Power_cmd(PC_READY);
		}
		if(mcu_ready && mcu_upgrade)
		{
			upgrade();
			mcu_upgrade = false;
			//mcu_ready = false;
		}
		if(mcu_ready)
		{
			int ret;
			ret = msgrcv(msgid_from_net,(void*)&msg,sizeof(struct net_msg_stru),0,IPC_NOWAIT);
			if((0 != ret)&&(-1 != ret))
			{
				mcu_send(msg.data,msg.len);
			}
		}
		mcu_len = 0;
		memset(mcu_buf,0,sizeof(mcu_buf));
		mcu_len = mcu_read(mcu_fd, mcu_buf);
		if(mcu_len > 0)
		{
			if(param.mcu_debug == 1)
			{
				
				Log_print(__FUNCTION__,"Len %d\n",mcu_len); 
				for(i = 0;i < mcu_len;i++)
				{			
	 				Log_print("%X ",mcu_buf[i]);
				}
				Log_print("\n");
			}
			if((mcu_buf[0] == 0xFF)&&(mcu_buf[1] == 0xCC))
			{
				Deal_mcu_cmd(mcu_buf,((mcu_buf[3] << 8) + mcu_buf[4]) + 6);
			}
			else if((mcu_buf[0] == 0xFF)&&(mcu_buf[1] == 0xBB))
			{
				Deal_mcu_cmd(mcu_buf,mcu_buf[3]+ 5);
			}
		}
	}

	delete extendQueue;
	close(mcu_fd); 
}


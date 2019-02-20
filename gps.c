#include "4tbox_define.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "gps.h"
#include "ql_oe.h"
#include "general.h"

static GPS_INFO GPS;
extern car_data_t car_data;
extern device_check_param_t device_check_param;
extern int at_fd;
static int nmea_fd = -1;

int gpsRecvFirstDataStatus;
static int prevCapGpsTime;
static location_t prevLocation;
static pthread_t init_gps_id;
extern pthread_t gps_thread_t;


extern pthread_mutex_t AT_mutex;
bool gps_thread_cancel = false;

char server_name[256] = "gaei.tcptest.timasync.com";
int server_port = 9093;
int at_fd1;

static int _recv_nema_data(void);
static void Callback_onSignal(int sig);

void* init_gps_thread(void* gps_param){
	int ret,kill_rc;
	void ** status = NULL;
	if(gps_thread_t != 0)
	{
		kill_rc = pthread_kill(gps_thread_t,0);
		if(kill_rc != ESRCH)
		{
			gps_thread_cancel = true;
			pthread_join(gps_thread_t,status);	
		}
	}

	ret = pthread_create(&gps_thread_t,NULL,gps_thread,NULL);
	if(ret != 0)
	{
		Log(__FUNCTION__,"gps thread create failed!\n");
	}

	return NULL;
}


int getSysTime(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec;
}

void correctGps(){
	int nowTime;	
	nowTime = getSysTime();
	//Log(__FUNCTION__,"%d %f %f",car_data.location.status,car_data.location.latitude,car_data.location.longitude);

	if(gpsRecvFirstDataStatus){
		if(!(car_data.location.status)){
			if(nowTime - prevCapGpsTime > 300){
				int ret;
				gpsRecvFirstDataStatus = false;
				car_data.location.status = 1;
				car_data.location.latitude = 0;
				car_data.location.longitude = 0;
				
				prevCapGpsTime = nowTime;
				
				//Log(__FUNCTION__,"gps restart1");
				ret = pthread_create(&init_gps_id,NULL,init_gps_thread,NULL);
				if(ret != 0)
				{
					Log(__FUNCTION__,"gps thread create failed!\n");
				}		
			}else{
				double distenceLatitude = fabs(car_data.location.latitude - prevLocation.latitude);
				double distenceLongitude = fabs(car_data.location.longitude - prevLocation.longitude);
				if(distenceLatitude + distenceLongitude > 0.001*(nowTime - prevCapGpsTime)){
					car_data.location.status = 1;
					car_data.location.latitude = 0;
					car_data.location.longitude = 0;
				}else{
					prevCapGpsTime = nowTime;

					prevLocation.latitude = car_data.location.latitude;
					prevLocation.longitude = car_data.location.longitude;
				}
			}
		}else{
			if(nowTime - prevCapGpsTime > 300){
				int ret;
				gpsRecvFirstDataStatus = false;
				car_data.location.status = 1;
				car_data.location.latitude = 0;
				car_data.location.longitude = 0;
				
				prevCapGpsTime = nowTime;
				
				//Log(__FUNCTION__,"gps restart2");
				ret = pthread_create(&init_gps_id,NULL,init_gps_thread,NULL);
				if(ret != 0)
				{
					Log(__FUNCTION__,"gps thread create failed!\n");
				}			
			}else{
				car_data.location.latitude = 0;
				car_data.location.longitude = 0;
				
			}		
		}
	}else{
		if(!(car_data.location.status)){
			gpsRecvFirstDataStatus = true;
			prevCapGpsTime = getSysTime();
			prevLocation.longitude = car_data.location.longitude;
			prevLocation.latitude = car_data.location.latitude;
			
		}else{
			car_data.location.latitude = 0;
			car_data.location.longitude = 0;
			if(nowTime - prevCapGpsTime > 300){
				int ret;
				prevCapGpsTime = nowTime;
				
				//Log(__FUNCTION__,"gps restart3");
				ret = pthread_create(&init_gps_id,NULL,init_gps_thread,NULL);
				if(ret != 0)
				{
					Log(__FUNCTION__,"gps thread create failed!\n");
				}
				
			}
		}
	}
	
	//Log(__FUNCTION__,"over %d %f %f",car_data.location.status,car_data.location.latitude,car_data.location.longitude);
}

void* gps_thread(void* gps_param)
{
	car_data.location.status = 1;
	gpsRecvFirstDataStatus = false;
	Log(__FUNCTION__,"gps thread start");
	gps_thread_cancel = false;
	
	// Set the output port of GPS-NMEA (the default is like so, can skip)
	Ql_SendAT("AT+QGPSCFG=\"outport\",\"linuxsmd\"", "OK", 1000);

	// Enable gps option
	Ql_SendAT("AT+QGPS=1", "OK", 1000);	
	signal(SIGTERM | SIGINT, Callback_onSignal);

	// Open GPS-NMEA port, and start to receive gps nmea ...
    nmea_fd = open(_NMEA_PORT, O_RDWR | O_NONBLOCK | O_NOCTTY);
	Log(__FUNCTION__,"< open nmea port(\"%s\"), result:%d >\n", _NMEA_PORT, nmea_fd);
	_recv_nema_data();
	
   	close(nmea_fd);
	// Disable gps option
	Ql_SendAT("AT+QGPSEND", "OK", 1000);		
    Log(__FUNCTION__,"< QuecOpen: GPS example end >\n\n");
	return 0;
}

static void Callback_onSignal(int sig)
{
	Log(__FUNCTION__,"< Recv signal %d >\n", sig);
	if ((SIGTERM == sig) || (SIGINT == sig))
	{
		// Disable gps option
		Ql_SendAT("AT+QGPSEND", "OK", 1000);		
	}
}

char * get_target (const char* target,char * buf)
{
    char *buff=buf;
    //char *target="$GNRMC";
    char *p=NULL;
                
    if((p=strstr(buff,target))==NULL)
    {
        //Log(__FUNCTION__,"No fonud the string GPRMC\n");
        return 0;
    }


    return p;
}

int getcomma(int num,char *str)
{
    int i,j=0;
    int len=strlen(str);
    for(i=0;i<len;i++)
    {
        if(str[i]==',')j++;
        if(j==num)return i+1; 
    }
    return 0;
}


static double get_double_number(char *s)
{
    char buf[1024];
    int i;
    double rev;
    i=getcomma(1,s);   
    strncpy(buf,s,i);
    buf[i]=0;         
    rev=atof(buf);   
    return rev;
}

static void UTC2BTC(date_time *GPS)  
{  
        GPS->second++; 
        if(GPS->second>59){  
            GPS->second=0;  
            GPS->minute++;  
            if(GPS->minute>59){  
                GPS->minute=0;  
                GPS->hour++;  
            }  
        }     
  
        GPS->hour+=8;      
        if(GPS->hour>23)  
        {  
            GPS->hour-=24;  
            GPS->day+=1;  
            if(GPS->month==2 ||GPS->month==4 ||GPS->month==6 ||GPS->month==9 ||GPS->month==11 ){  
                if(GPS->day>30){          
                    GPS->day=1;  
                    GPS->month++;  
                }  
            }  
            else{  
                if(GPS->day>31){         
                    GPS->day=1;  
                    GPS->month++;  
                }  
            }  
            if(GPS->year % 4 == 0 ){ 
                if(GPS->day > 29 && GPS->month ==2){     
                    GPS->day=1;  
                    GPS->month++;  
                }  
            }  
            else{  
                if(GPS->day>28 &&GPS->month ==2){     
                    GPS->day=1;  
                    GPS->month++;  
                }  
            }  
            if(GPS->month>12){  
                GPS->month-=12;  
                GPS->year++;  
            }         
        }  
} 

void gps_parse(char *buff_gprmc,char *buff_gpgga,char *buff_gpvtg,char *buff_gpgsv,GPS_INFO *GPS)  
{  
    int tmp;  
	static int day = 0;
	if(buff_gprmc != NULL)
	{
		if((buff_gprmc[7] >= '0') && (buff_gprmc[7] <= '9'))
		{
		    GPS->D.hour   =(buff_gprmc[ 7]-'0')*10+(buff_gprmc[ 8]-'0');  
		    GPS->D.minute =(buff_gprmc[ 9]-'0')*10+(buff_gprmc[10]-'0');  
		    GPS->D.second =(buff_gprmc[11]-'0')*10+(buff_gprmc[12]-'0');  
			GPS->D.msecond=(buff_gprmc[14]-'0')*100+(buff_gprmc[15]-'0')*10;
		}
		//$GNRMC,013303.00,A,2231.60528,N,11356.12848,E,0.034,,090617,,,D*6B
	    tmp = getcomma(9,buff_gprmc);  
		if((buff_gprmc[tmp] >= '0') && (buff_gprmc[tmp] <= '9') &&
		   (buff_gprmc[tmp + 1] >= '0') && (buff_gprmc[tmp + 1] <= '9') &&
		   (buff_gprmc[tmp + 2] >= '0') && (buff_gprmc[tmp + 2] <= '9') &&
		   (buff_gprmc[tmp + 3] >= '0') && (buff_gprmc[tmp + 3] <= '9') &&
		   (buff_gprmc[tmp + 4] >= '0') && (buff_gprmc[tmp + 4] <= '9') &&
		   (buff_gprmc[tmp + 5] >= '0') && (buff_gprmc[tmp + 5] <= '9') &&(tmp != 0))
		{
	    	GPS->D.day    =(buff_gprmc[tmp+0]-'0')*10+(buff_gprmc[tmp+1]-'0');  
	    	GPS->D.month  =(buff_gprmc[tmp+2]-'0')*10+(buff_gprmc[tmp+3]-'0');  
	    	GPS->D.year   =(buff_gprmc[tmp+4]-'0')*10+(buff_gprmc[tmp+5]-'0');  
		}
	    tmp = getcomma(2,buff_gprmc);  
		if((buff_gprmc[tmp] != ',') && (tmp != 0))
		{
	    	GPS->status   =buff_gprmc[tmp]; 
			if(GPS->status == 'A')
			{
				car_data.location.status &= ~0x01;
				device_check_param.gps_check_status = 2;
			}
			else if(GPS->status == 'V')
			{			
				car_data.location.status |= 0x01;
			}

		}
		tmp = getcomma(3,buff_gprmc); 
		if((buff_gprmc[tmp] != ',') && (tmp != 0))
		{
			//2231.59242,N,11356.14871,E
			double num1 = get_double_number(&buff_gprmc[tmp])/100;
	    	GPS->latitude =(int)num1 + (((num1 - (int)num1)*100)/60); 
			if(GPS->latitude != 0)
			{
				car_data.location.latitude = GPS->latitude;
			}
		}
		tmp = getcomma(4,buff_gprmc); 
		if((buff_gprmc[tmp] != ',') && (tmp != 0))
		{
	    	GPS->NS       =buff_gprmc[tmp];  
			if(GPS->NS == 'N')
			{
				car_data.location.status &= ~(0x01 << 1);
			}
			else if(GPS->NS == 'S')
			{
				car_data.location.status |= 0x01 << 1;
			}

		}
		tmp = getcomma(5,buff_gprmc);
		if((buff_gprmc[tmp] != ',') && (tmp != 0))
		{
			double num2 = get_double_number(&buff_gprmc[tmp])/100;
	    	GPS->longitude =(int)num2 + (((num2 - (int)num2)*100)/60); 
			if(GPS->longitude != 0)
			{
				car_data.location.longitude = GPS->longitude;
			}
		}
		tmp = getcomma(6,buff_gprmc);
		if((buff_gprmc[tmp] != ',') && (tmp != 0))	
		{
	    	GPS->EW       =buff_gprmc[tmp];     
			if(GPS->EW == 'E')
			{
				car_data.location.status &= ~(0x01 << 2);
			}
			else if(GPS->EW == 'W')
			{
				car_data.location.status |= 0x01 << 2;
			}

		}
	    
	    UTC2BTC(&GPS->D);   
		if((day != GPS->D.day)&&(GPS->D.day != 0)&&
		   (buff_gprmc[7] >= '0') && (buff_gprmc[7] <= '9') &&
		   (buff_gprmc[8] >= '0') && (buff_gprmc[8] <= '9') && !car_data.location.status) 
		{
		    tmp = getcomma(9,buff_gprmc);  
			if((buff_gprmc[tmp] >= '0') && (buff_gprmc[tmp] <= '9') &&
			   (buff_gprmc[tmp + 1] >= '0') && (buff_gprmc[tmp + 1] <= '9') &&
			   (buff_gprmc[tmp + 2] >= '0') && (buff_gprmc[tmp + 2] <= '9') &&
			   (buff_gprmc[tmp + 3] >= '0') && (buff_gprmc[tmp + 3] <= '9') &&
			   (buff_gprmc[tmp + 4] >= '0') && (buff_gprmc[tmp + 4] <= '9') &&
			   (buff_gprmc[tmp + 5] >= '0') && (buff_gprmc[tmp + 5] <= '9') &&(tmp != 0))
			{
				char time_buf[48] = {0};
				int ret = 0;
				sprintf(time_buf,"date -s \"%04d-%02d-%02d %02d:%02d:%02d\"",
							GPS->D.year+2000,GPS->D.month,GPS->D.day,GPS->D.hour,GPS->D.minute,GPS->D.second);
				ret = system(time_buf);
				Log(__FUNCTION__,time_buf);
				if(ret == 0)
				{
					//delete log more than 7 days
					system("find ./log/ -mtime +6 -name \'*.log\' -exec rm {} \\;");
					day = GPS->D.day;
				}
			}
		}
	}
	if(buff_gpgga != NULL)
	{
		tmp = getcomma(9,buff_gpgga);
		if((buff_gpgga[tmp] != ',') && (tmp != 0))
		{
    		GPS->high     = get_double_number(&buff_gpgga[tmp]);   
		}
		tmp = getcomma(7,buff_gpgga);
		if((buff_gpgga[tmp] != ',') && (tmp != 0))
		{
    		GPS->satelite_num = get_double_number(&buff_gpgga[tmp]);   
		}
	}
	if(buff_gpvtg != NULL)
	{
		tmp = getcomma(1,buff_gpvtg);
		if((buff_gpvtg[tmp] != ',') && (tmp != 0))
		{
    		GPS->direction = get_double_number(&buff_gpvtg[tmp]);   
			car_data.location.direction = GPS->direction;
			//printf("direct = %f\n",car_data.location.direction);
		}	
		
		tmp = getcomma(7,buff_gpvtg);
		if((buff_gpvtg[tmp] != ',') && (tmp != 0))
		{
    		GPS->speed = get_double_number(&buff_gpvtg[tmp]);   
			car_data.location.speed = GPS->speed;
			
			//printf("direct = %f\n",car_data.location.speed);
		}		
	}
	if(buff_gpgsv != NULL)
	{
		tmp = getcomma(7,buff_gpgsv);
		if((buff_gpgsv[tmp] != ',') && (tmp != 0))
		{
    		GPS->GPS_signal= get_double_number(&buff_gpgsv[tmp]);   
		}			
	}
} 

static int _recv_nema_data(void)
{
    fd_set fds;
    int rdLen;
    int lenToRead = 200;
	char gps_data_buffer[200] = {0};
	struct timeval timeout = {0, 0};
	char *buff_gprmc,*buff_gpgga,*buff_gpvtg,*buff_gpgsv;  
    // Wait for the GPS data 
    gps_thread_cancel = false;
	while (!gps_thread_cancel)
	{
		FD_ZERO(&fds); 
		FD_SET(nmea_fd, &fds); 
		timeout.tv_sec  = 1;
		timeout.tv_usec = 0;

		switch (select(nmea_fd + 1, &fds, NULL, NULL, &timeout))
		{
			case -1: 
				Log(__FUNCTION__,"< select error >\n");
				return -1;

			case 0:
//				Log(__FUNCTION__,"< time out >\n");
				break;

			default: 
				if (FD_ISSET(nmea_fd, &fds)) 
				{
					do {
						memset(gps_data_buffer, 0x0, sizeof(gps_data_buffer));
						rdLen = read(nmea_fd, gps_data_buffer, lenToRead);
						//Log(__FUNCTION__,"%s",gps_data_buffer);
						buff_gprmc=get_target("$GPRMC",gps_data_buffer);
				        buff_gpgga=get_target("$GPGGA",gps_data_buffer);
						buff_gpvtg=get_target("$GPVTG",gps_data_buffer);
						buff_gpgsv=get_target("$GPGSV",gps_data_buffer);
						gps_parse(buff_gprmc,buff_gpgga,buff_gpvtg,buff_gpgsv,&GPS);
						GPS.GPS_error = false;
					} while (rdLen > 0);
				}
				else
				{
					Log(__FUNCTION__,"FD is missed\n");
				}
				break;
		}

   	}
   	return 0;
}

int Ql_SendAT(char* atCmd, char* finalRsp, long timeout_ms)
{
	pthread_mutex_lock(&AT_mutex);
    int iRet;
    int iLen;
    fd_set fds;
    int rdLen;
    int lenToRead = 100;
    char strAT[100];
    char strFinalRsp[100];
    char strResponse[100];
    struct timeval timeout = {0, 0};
    boolean bRcvFinalRsp = FALSE;

    memset(strAT, 0x0, sizeof(strAT));
    iLen = sizeof(atCmd);
    strncpy(strAT, atCmd, iLen);

    sprintf(strFinalRsp, "\r\n%s", finalRsp);
    
	timeout.tv_sec  = timeout_ms / 1000;
	timeout.tv_usec = timeout_ms % 1000;
    
    
    // Add <cr><lf> if needed
    iLen = strlen(atCmd);
    if ((atCmd[iLen-1] != '\r') && (atCmd[iLen-1] != '\n'))
    {
        iLen = sprintf(strAT, "%s\r\n", atCmd); 
        strAT[iLen] = 0;
    }

    // Send AT
    iRet = write(at_fd, strAT, iLen);
    Log(__FUNCTION__,">>Send AT: \"%s\", iRet=%d\n", atCmd, iRet);
    
    // Wait for the response
	while (1)
	{
		FD_ZERO(&fds); 
		FD_SET(at_fd, &fds); 

		switch (select(at_fd + 1, &fds, NULL, NULL, &timeout))
		{
		case -1: 
			Log(__FUNCTION__,"< select error >\n");
			pthread_mutex_unlock(&AT_mutex);
			return -1;

		case 0:
//			Log(__FUNCTION__,"< time out >\n");
			pthread_mutex_unlock(&AT_mutex);
			return 1; 

		default: 
			if (FD_ISSET(at_fd, &fds)) 
			{
				do {
					memset(strResponse, 0x0, sizeof(strResponse));
					rdLen = read(at_fd, strResponse, lenToRead);
					//Log(__FUNCTION__,">>Read response/urc, len=%d, content:\n%s\n", rdLen, strResponse);
					//Log(__FUNCTION__,"rcv:%s", strResponse);
					//Log(__FUNCTION__,"final rsp:%s", strFinalRsp);
					if ((rdLen > 0) && strstr(strResponse, strFinalRsp))
					{
					    if (strstr(strResponse, strFinalRsp)     // final OK response
					       || strstr(strResponse, "+CME ERROR:") // +CME ERROR
					       || strstr(strResponse, "+CMS ERROR:") // +CMS ERROR
					       || strstr(strResponse, "ERROR"))      // Unknown ERROR
					    {
					        //Log(__FUNCTION__,"\n< match >\n");
					        bRcvFinalRsp = TRUE;
					    }else{
					        Log(__FUNCTION__,"\n< not final rsp >\n");
					    }
					}
				} while ((rdLen > 0) && (lenToRead == rdLen));
			}else{
				Log(__FUNCTION__,"FD is missed\n");
			}
			break;
		}

		// Found the final response , return back
		if (bRcvFinalRsp)
		{
		    break;
		}	
   	}
	pthread_mutex_unlock(&AT_mutex);
   	return 0;
}

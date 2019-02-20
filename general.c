#include "general.h"
#include "string.h"
#include "cJSON.h"
#include "ql_tzone.h"

extern pthread_mutex_t log_mutex;

param_t param = {
	.VIN = "00000000000000000",
	.ICCID = "00000000000000000000",
	.IMEI = "000000000000000",
	.IMSI = "000000000000000",
	.url = "mx-vehicle-monitor-ta.mxnavi.com",
	.platform_url = "mx-vehicle-monitor-ta.mxnavi.com",
	.heart_period = 15,
	.data_period = 10,
	.btry_sys_num = 1,
	.sys_num_length = 0,
	.btry_num = 88,
	.alert_period = 1,
	.tbox_timeout = 10,
	.port = 8474,
	.platform_port = 8474,
	.save_period = 1,
	.tcp_rcv_timeout = 30,
	.next_login_time = 60,
	.temp_p_num = 30,
	.login_serial_num = 1,
	.GB32690_type = 0,
	.GB32690_type_note = "0-off,1-login,2-rep,3-alert,4-normal",
	.before_offline_time = 10,
	.offline_time = 300,
	.before_alert_time = 120,
	.alert_time = 60,
	.mcu_debug = 0,
	.decrypt = 1,
	.usbnet=1,
};

upgrade_param_t upgrade_param = {
	.pc_upgrade_status = 0,
	.mcu_upgrade_status = 0,
	.vcu_upgrade_status = 0,
	.mcu_mcu_upgrade_status = 0,
	.bms_upgrade_status = 0,
	
	.pc_upgrade_path = "",
	.mcu_upgrade_path = "",
	.vcu_upgrade_path = "",
	.mcu_mcu_upgrade_path = "",
	.bms_upgrade_path = "",
	
	.pc_upgrade_sign = "",
	.mcu_upgrade_sign = "",
	.vcu_upgrade_sign = "",
	.mcu_mcu_upgrade_sign = "",
	.bms_upgrade_sign = "",
};


#define TYPE_CHAR_P  1
#define TYPE_INT	 2

#define PARAM_NUM	29


set_param_t set_param[PARAM_NUM] = {
	{"VIN",TYPE_CHAR_P,(void*)param.VIN},
	{"ICCID",TYPE_CHAR_P,(void*)param.ICCID},
	{"IMEI",TYPE_CHAR_P,(void*)param.IMEI},
	{"IMSI",TYPE_CHAR_P,(void*)param.IMSI},
	{"url",TYPE_CHAR_P,(void*)param.url},
	{"platform_url",TYPE_CHAR_P,(void*)param.platform_url},
	{"heart_period",TYPE_INT,(void*)(&param.heart_period)},
	{"data_period",TYPE_INT,(void*)(&param.data_period)},
	{"btry_sys_num",TYPE_INT,(void*)(&param.btry_sys_num)},
	{"sys_num_length",TYPE_INT,(void*)(&param.sys_num_length)},
	{"btry_num",TYPE_INT,(void*)(&param.btry_num)},
	{"alert_period",TYPE_INT,(void*)(&param.alert_period)},
	{"tbox_timeout",TYPE_INT,(void*)(&param.tbox_timeout)},
	{"port",TYPE_INT,(void*)(&param.port)},
	{"platform_port",TYPE_INT,(void*)(&param.platform_port)},
	{"save_period",TYPE_INT,(void*)(&param.save_period)},
	{"tcp_rcv_timeout",TYPE_INT,(void*)(&param.tcp_rcv_timeout)},
	{"next_login_time",TYPE_INT,(void*)(&param.next_login_time)},
	{"login_serial_num",TYPE_INT,(void*)(&param.login_serial_num)},
	{"temp_p_num",TYPE_INT,(void*)(&param.temp_p_num)},
	{"GB32690_type",TYPE_INT,(void*)(&param.GB32690_type)},
	{"GB32690_type_note",TYPE_CHAR_P,(void*)param.GB32690_type_note},
	{"before_offline_time",TYPE_INT,(void*)(&param.before_offline_time)},
	{"offline_time",TYPE_INT,(void*)(&param.offline_time)},
	{"before_alert_time",TYPE_INT,(void*)(&param.before_alert_time)},
	{"alert_time",TYPE_INT,(void*)(&param.alert_time)},
	{"mcu_debug",TYPE_INT,(void*)(&param.mcu_debug)},
	{"decrypt",TYPE_INT,(void*)(&param.decrypt)},
	{"usbnet",TYPE_INT,(void*)(&param.usbnet)}
};

set_param_t set_upgrade_param[6] = {
	{"pc_upgrade_status",TYPE_INT,(void*)(&upgrade_param.pc_upgrade_status)},
	{"mcu_upgrade_status",TYPE_INT,(void*)(&upgrade_param.mcu_upgrade_status)},
	{"pc_upgrade_path",TYPE_CHAR_P,(void*)(&upgrade_param.pc_upgrade_path)},
	{"mcu_upgrade_path",TYPE_CHAR_P,(void*)(&upgrade_param.mcu_upgrade_path)},
	{"pc_upgrade_sign",TYPE_CHAR_P,(void*)(&upgrade_param.pc_upgrade_sign)},
	{"mcu_upgrade_sign",TYPE_CHAR_P,(void*)(&upgrade_param.mcu_upgrade_sign)},	
};


air_condition_param_t air_condition_param[3]; //0 全车  1 前排 2 后排 
extend_vehicle_param_t extend_vehicle_param;
device_check_param_t device_check_param;

unsigned char app_version[32];
unsigned char mcu_version[32];

unsigned char hardware_version[32];


air_condition_param_t *get_air_conditon(int positon){
	return &air_condition_param[positon];
}

extend_vehicle_param_t *get_extend_vechicle_param(){
	return &extend_vehicle_param;
}

device_check_param_t *get_device_check_param(){
	return &device_check_param;
}

void init_get_device_check_param(){
	device_check_param.emmc_check_status = 2;
	device_check_param.gps_check_status = 1;
	device_check_param.tsp_check_status =1;
	int ret;
	if((access(KEY_PATH,F_OK))!=-1)   
	{	
		ret = ql_tzapp_aes_encrypt_data(KEY_PATH,EN_PATH);
		if(ret != -1)
		{
			remove(KEY_PATH);
		}
	}	

}

void Log(const char *fun,const char *fmt, ...)
{
#ifdef PRINT_LOG
	if((fmt == NULL) || (fun == NULL))
	{
		return;
	}
	
	time_t now;   
	struct tm *timenow;      
	time(&now); 
	timenow = localtime(&now);   

	char file_name[32] = {0};
	sprintf(file_name,LOG_PATH,timenow->tm_year + 1900,timenow->tm_mon + 1,timenow->tm_mday);

	pthread_mutex_lock(&log_mutex);
#ifdef SAVE_LOG
	FILE *fd = fopen(file_name,"a");
	if(fd == NULL)
	{
		pthread_mutex_unlock(&log_mutex);
		return;
	}
	fprintf(fd,"[%04d-%02d-%02d %02d:%02d:%02d %s]",
			timenow->tm_year + 1900,timenow->tm_mon + 1,timenow->tm_mday,
			timenow->tm_hour,timenow->tm_min,timenow->tm_sec,fun);
#endif
	printf("[%04d-%02d-%02d %02d:%02d:%02d %s]",
			timenow->tm_year + 1900,timenow->tm_mon + 1,timenow->tm_mday,
			timenow->tm_hour,timenow->tm_min,timenow->tm_sec,fun);

	va_list ap; 
	int d;
	char c, *s; 
	int x,X;
	double f;
	va_start(ap, fmt);
	while(*fmt)
	{   
		if(*(fmt - 1) == '%')
		{
			switch (*fmt) 
			{   
			case 's': /* string */
					s = va_arg(ap, char *); 
#ifdef SAVE_LOG
					fprintf(fd,"%s", s); 
#endif
					printf("%s", s);
					break;
			case 'd': /* int */
					d = va_arg(ap, int);
#ifdef SAVE_LOG
					fprintf(fd,"%d", d); 
#endif
					printf("%d", d); 
					break;
			case 'x':
					x = (int) va_arg(ap, int);
#ifdef SAVE_LOG
					fprintf(fd,"%02x",x);
#endif
					printf("%02x",x);
					break;
			case 'X':
					X = (int) va_arg(ap, int);
#ifdef SAVE_LOG
					fprintf(fd,"%02X",X);
#endif
					printf("%02X",X);
					break;
			case 'c': /* char */
			/* need a cast here since va_arg only takes fully promoted types */
					c = (char) va_arg(ap, int);
#ifdef SAVE_LOG
					fprintf(fd,"%c", c); 
#endif
					printf("%c", c); 
					break;
			case 'f':				
					f = (double) va_arg(ap, double);
#ifdef SAVE_LOG
					fprintf(fd,"%f", f); 
#endif
					printf("%f", f); 
					break;				
			}   
		}
		else
		{
			if(*fmt!='%')  
			{
#ifdef SAVE_LOG
				fprintf(fd,"%c",*fmt);  
#endif
				printf("%c",*fmt); 
			}
		}
		fmt++;
	} 
   va_end(ap);
#ifdef SAVE_LOG
   fprintf(fd,"\n");
#endif
   printf("\n");
#ifdef SAVE_LOG
   fclose(fd);
#endif
   pthread_mutex_unlock(&log_mutex);
#endif
}

void Log_print(const char *fmt, ...)
{
#ifdef PRINT_LOG
	 if(fmt == NULL)
	 {
		 return;
	 }
	 
	 time_t now;   
	 struct tm *timenow;	  
	 time(&now);   
	 timenow = localtime(&now);   
	
	 char file_name[32] = {0};
	 sprintf(file_name,LOG_PATH,timenow->tm_year + 1900,timenow->tm_mon + 1,timenow->tm_mday);
	
	 pthread_mutex_lock(&log_mutex);

	 va_list ap; 
	 int d;
	 char c, *s,x,X; 
	 double f;
	 va_start(ap, fmt);
	 while(*fmt)
	 {	 
		 if(*(fmt - 1) == '%')
		 {
			 switch (*fmt) 
			 {	 
			 case 's': /* string */
					 s = va_arg(ap, char *); 

					 printf("%s", s); 
					 break;
			 case 'd': /* int */
					 d = va_arg(ap, int);

					 printf("%d", d); 
					 break;
			 case 'x':
					 x = (char) va_arg(ap, int);

					 printf("%02x",x);
					 break;
			 case 'X':
					 X = (char) va_arg(ap, int);

					 printf("%02X",X);
					 break;
			 case 'c': /* char */
			 /* need a cast here since va_arg only takes fully promoted types */
					 c = (char) va_arg(ap, int);

					printf("%c", c); 
					break;
			case 'f':				
					f = (double) va_arg(ap, double);

					printf("%f", f); 
					break;				
			}  		
		 }	 
		 else
		 {
			 if(*fmt!='%')	
			 {

			 	printf("%c",*fmt);  
			 }
		 }
		 fmt++;
	 } 
	va_end(ap);

	pthread_mutex_unlock(&log_mutex);
#endif
}

unsigned char check_sum(unsigned char * buf,int len)
{
	int i;
	unsigned char sum = 0;
	sum = buf[0] ^ buf[1];
	for(i = 2;i < len;i++)
	{
		sum^= buf[i];
	}
	return sum;
}

bool getFirewareVersion(char *firewareVersion){
	int nowIndex=0,verStartIndex=0,verEndIndex=0;
	int infoLen = 0;

	char firewareInfo[1024] = {0};
	FILE *fd = fopen(FIERWARE_PATH,"a+");
	if(fd == NULL)
	{
		return false;
	}
	
	fseek(fd,0,SEEK_END);
	infoLen = ftell(fd);  	
	fseek(fd,0,SEEK_SET);
	fread(firewareInfo,sizeof(firewareInfo),1,fd);		
	fclose(fd);
	
	for(;nowIndex<infoLen;++nowIndex){
		if(firewareInfo[nowIndex] == 'P'){
			if(!strncmp(&firewareInfo[nowIndex],"Project Rev",11)){
				nowIndex += 11;
				break;
			}
		}
	}

	if(nowIndex<infoLen){
		for(;nowIndex<infoLen;++nowIndex){			
			if(firewareInfo[nowIndex] == ':'){
				verStartIndex = nowIndex + 1;
				break;
			}
		}
	}else{
		return false;
	}

	if(nowIndex<infoLen){
		for(;nowIndex<infoLen;++nowIndex){			
			if(firewareInfo[nowIndex] == '\n'){
				verEndIndex = nowIndex - 1;
				break;
			}
		}
	}else{
		return false;
	}

	int verLen = verEndIndex - verStartIndex;
	if(verLen>30){
		verLen = 30;
	}
	memcpy(firewareVersion,firewareInfo+verStartIndex,verLen);
	return true;
}

bool get_json_upgrade(char *jsonStr)
{
	cJSON * jsonNode = NULL;
	jsonNode = cJSON_Parse(jsonStr);
	int i;
	if (!jsonNode)
	{
		return false;
	}
	else
	{
		cJSON *temp = NULL;
		for(i = 0;i < 6;i++)
		{
			temp = cJSON_GetObjectItem(jsonNode,set_upgrade_param[i].keyword);
			if(temp)
			{
				switch(set_upgrade_param[i].type)
				{
				case TYPE_INT:
					*((int*)set_upgrade_param[i].value) = temp->valueint;
					Log(__FUNCTION__,"%d\n",temp->valueint);
					break;
				case TYPE_CHAR_P:
					memcpy(set_upgrade_param[i].value,temp->valuestring,strlen(temp->valuestring));	
					Log(__FUNCTION__,"%s\n",temp->valuestring);
					break;
				}
			}			 
		}		
	}
	cJSON_Delete(jsonNode);
	return true;
}

void init_save_upgrade()
{
	FILE *fd = fopen(UPGRADE_PATH,"wb");
	if(fd == NULL)
	{
		return;
	}
	cJSON *paramJson = NULL;
	paramJson = cJSON_CreateObject();
	int i;
	if(paramJson == NULL)
	{
		return ;
	}
	else
	{	
		for(i = 0;i < 6;i++)
		{
			switch(set_upgrade_param[i].type)
			{
			case TYPE_INT:
				cJSON_AddNumberToObject(paramJson,set_upgrade_param[i].keyword,*((int*)set_upgrade_param[i].value));
				break;
			case TYPE_CHAR_P:
				cJSON_AddStringToObject(paramJson,set_upgrade_param[i].keyword,(char*)set_upgrade_param[i].value);
				break;
			}			
		}
	}
    char jsonStr[2048]={0};
	sprintf(jsonStr,"%s",cJSON_Print(paramJson));	
	if(fd){				
		fseek(fd,0,SEEK_SET);
		fwrite(jsonStr,strlen(jsonStr),1,fd);		
		fclose(fd);
	}
	cJSON_Delete(paramJson);
}

void get_upgrade()
{
	if (access(UPGRADE_PATH,F_OK))
	{
		init_save_upgrade();
	}

	FILE *fd = fopen(UPGRADE_PATH,"rb");
	char jsonStr[2048] = {0};				
	fseek(fd,0,SEEK_SET);
	fread(jsonStr,sizeof(jsonStr),1,fd);		
	fclose(fd);
	get_json_upgrade(jsonStr);
	//因为版本号都会发生变化，所以每次更新版本号到文件里
	init_save_upgrade(); 
	system("sync");
}

bool get_json_param(char *jsonStr)
{
	cJSON * jsonNode = NULL;
	jsonNode = cJSON_Parse(jsonStr);
	int i;
	if (!jsonNode)
	{
		return false;
	}
	else
	{
		cJSON *temp = NULL;	
		size_t len = 0;
		for(i = 0;i < PARAM_NUM;i++)
		{
			temp = cJSON_GetObjectItem(jsonNode,set_param[i].keyword);
			if(temp)
			{
				switch(set_param[i].type)
				{
				case TYPE_INT:
					*((int*)set_param[i].value) = temp->valueint;
					Log(__FUNCTION__,"%d\n",temp->valueint);
					break;
				case TYPE_CHAR_P:
					len = sizeof(set_param[i].value);
					memset(set_param[i].value,0,len);
					memcpy(set_param[i].value,temp->valuestring,strlen(temp->valuestring));	
					Log(__FUNCTION__,"%s\n",temp->valuestring);						
					break;
				}
			}			 
		}		
	}
	cJSON_Delete(jsonNode);
	return true;
}

void init_save_param(const char* path)
{
	FILE *fd = fopen(path,"wb");
	if(fd == NULL)
	{
		return;
	}
	cJSON *paramJson = NULL;
	paramJson = cJSON_CreateObject();
	int i;
	if(paramJson == NULL)
	{
		return ;
	}
	else
	{	
		cJSON_AddStringToObject(paramJson, "app_version", APP_VERSION);
		for(i = 0;i < PARAM_NUM;i++)
		{
			switch(set_param[i].type)
			{
			case TYPE_INT:
				cJSON_AddNumberToObject(paramJson,set_param[i].keyword,*((int*)set_param[i].value));
				break;
			case TYPE_CHAR_P:
				cJSON_AddStringToObject(paramJson,set_param[i].keyword,(char*)set_param[i].value);
				break;
			}			
		}
	}
    char jsonStr[2048]={0};
	sprintf(jsonStr,"%s",cJSON_Print(paramJson));	
	if(fd){				
		fseek(fd,0,SEEK_SET);
		fwrite(jsonStr,strlen(jsonStr),1,fd);		
		fclose(fd);
	}
	cJSON_Delete(paramJson);
}

void get_param()
{
	if (access(PARAM_PATH,F_OK) && access(BAK_PARAM_PATH,F_OK))
	{
		init_save_param(PARAM_PATH);
	}
	else if (access(PARAM_PATH,F_OK))
	{
		system("cp /usrdata/param.json /home/root/param.json");
		system("sync");
	}
	FILE *fd = fopen(PARAM_PATH,"rb");
	char jsonStr[2048] = {0};				
	fseek(fd,0,SEEK_SET);
	fread(jsonStr,sizeof(jsonStr),1,fd);		
	fclose(fd);
	get_json_param(jsonStr);
	//因为版本号都会发生变化，所以每次更新版本号到文件里
	init_save_param(PARAM_PATH); 
	system("cp /home/root/param.json /usrdata/param.json");
	system("sync");
	memcpy(app_version,APP_VERSION,strlen(APP_VERSION));
}




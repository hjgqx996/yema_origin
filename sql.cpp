extern "C" 
{	
#include "4tbox_define.h"
#include <sqlite3.h>
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <sys/ipc.h>
#include <arpa/inet.h>  
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdbool.h> 
#include <pthread.h>
#include <linux/rtc.h> 
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>
#include "network.h"
#include "mcu.h"
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "general.h"
}

#include "gpExtendQueue.h"
//#include "tbox_abstract.h"

static int heart_test_timer = 0;
static int sql_save_timer = 0;
int period = 0;
bool sql_thread_cancel = false;
extern param_t param;
extern pthread_mutex_t sql_mutex;
car_data_t car_data;
extern bool connect_ok;
AES_KEY aes;

unsigned char AES_key[AES_BLOCK_SIZE] = {0xA8,0x68,0xf6,0x84,0xFb,0x99,0x0D,0xdF,
										 0x1f,0xDf,0xbb,0x50,0x0e,0xba,0xeB,0xCd};
sqlite3 *db=NULL; 
sqlite3 *plat_db = NULL;

extern int sock_cli;
extern int plat_sock;
extern gl_data_t gl_data;
extern gpExtendQueue *extendQueue;


static size_t PKCS5_padding(unsigned char *src, size_t srcLen)
{
    // PKCS#5
    size_t paddNum = 8 - srcLen % 8;
	unsigned int i;
    for (i = 0; i < paddNum; ++i) {
        src[srcLen + i] = paddNum;
    }   
    return srcLen + paddNum;
}

unsigned char my_itoh(int val)
{
	val %= 100;
	return (((val/10) << 4) + (val%10));
}

int my_htoi(unsigned char buf)
{
    return (buf >> 4)*10 + (buf & 0xf);
}

bool check_RSA_key()
{
   RSA *p_rsa;
   FILE *file;
   if((file=fopen(RSA_FILE_NAME,"r"))==NULL)
   {
       perror("open key file error");
       return false;
   }
   if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL)
   {
       ERR_print_errors_fp(stdout);
       fclose(file);
       return false;
   }
   RSA_free(p_rsa);
   fclose(file);
   return true;
}

void RSA_pub_encrypt(char* pubkey_path,unsigned char *str,char *p_en)
{
   RSA *p_rsa;
   FILE *file;
   int rsa_len;
   if((file=fopen(pubkey_path,"r"))==NULL)
   {
       perror("open key file error");
       return;
   }
   if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL)
   {
       ERR_print_errors_fp(stdout);
	   RSA_free(p_rsa);
       fclose(file);
       return;
   }
   rsa_len=RSA_size(p_rsa);
   memset(p_en,0,rsa_len+1);
   if(RSA_public_encrypt(rsa_len,str,(unsigned char*)p_en,p_rsa,RSA_NO_PADDING)<0)
   {
       RSA_free(p_rsa);
   	   fclose(file);
       return;
   }
   RSA_free(p_rsa);
   fclose(file);
   
   Log_print(__FUNCTION__,"RSA_encrypt finish");
}

void RSA_pub_decrypt(const char* pubkey_path,unsigned char *str,char *p_de)  
{  
	RSA *p_rsa;
	FILE *file;
	int rsa_len;
	if((file=fopen(pubkey_path,"r"))==NULL)
	{
		perror("open key file error");
		return;
	}
	if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL)
	{
		ERR_print_errors_fp(stdout);
		RSA_free(p_rsa);
		fclose(file);
		return;
	}
	rsa_len=RSA_size(p_rsa);
	memset(p_de,0,rsa_len+1);
	if(RSA_public_decrypt(rsa_len,str,(unsigned char*)p_de,p_rsa,RSA_NO_PADDING)<0)
	{
		RSA_free(p_rsa);
		fclose(file);
		return;
	}
	RSA_free(p_rsa);
	fclose(file);
	Log_print(__FUNCTION__,"RSA_decrypt finish");

}  

void aes_encrypt(unsigned char* input_string,int input_len,int *offset)
{
    //unsigned char iv[AES_BLOCK_SIZE] = {0};        // init vector
    unsigned int len = 0;        // encrypt length (in multiple of AES_BLOCK_SIZE)
	unsigned char buf[1024] = {0};
	unsigned char encrypt_string[1024] = {0};
	memcpy(buf,input_string,input_len);
	len = PKCS5_padding(buf,input_len);
/*	if(input_len % AES_BLOCK_SIZE)
	{
		len = input_len + AES_BLOCK_SIZE - (input_len%AES_BLOCK_SIZE);
		(*offset) += AES_BLOCK_SIZE - (input_len%AES_BLOCK_SIZE);
	}
	else
	{
		len = input_len;
	}*/
	//(*offset) += AES_BLOCK_SIZE - (input_len%AES_BLOCK_SIZE);
    // encrypt (iv will change)
    Log_print(__FUNCTION__,"encrypt len = %d\n",len);
	if (AES_set_encrypt_key(AES_key, 128, &aes) < 0) 
	{
        Log(__FUNCTION__,"Unable to set encryption key in AES\n");
        return;
    }
    AES_ecb_encrypt(buf, encrypt_string,&aes,AES_ENCRYPT);
	memcpy(input_string,encrypt_string,len);
}

void aes_decrypt(unsigned char* encrypt_string,int* len,FILE* fd)
{
	unsigned char decrypt_string[1024] = {0};
	unsigned char temp_buf[16] = {0};
	int i,read_len;
  	if (AES_set_decrypt_key(AES_key, 128, &aes) < 0) 
	{
        fprintf(stderr, "Unable to set decryption key in AES\n");
        return;
    }
    // decrypt
    AES_ecb_encrypt(encrypt_string, decrypt_string,&aes,AES_DECRYPT);
	//防止pkcs5填充出现错误
	if((decrypt_string[15] <= 0x10) && (decrypt_string[15] > 0x01))
	{
		for(i = 0;i < decrypt_string[15];i++)
		{
			if(decrypt_string[15-i] != decrypt_string[15])
			{
				break;
			}
		}
		if(i >= decrypt_string[15])
		{
			Log_print(__FUNCTION__,"i = %d\n",i);
			*len -= decrypt_string[15];
			Log_print(__FUNCTION__,"len = %d\n",*len);
		}
	}
	else if(decrypt_string[15] == 0x01)
	{
		read_len = fread(temp_buf,1,sizeof(temp_buf),fd);	
		if(read_len > 0)
		{
			fseek(fd,-read_len,SEEK_CUR);
		}
		else
		{
			*len -= 1;
		}
	}
	memcpy(encrypt_string,decrypt_string,*len);
}

void sha256_file(const char* file_path,unsigned char* hash)
{
	unsigned char buf[1024] = {0};
	unsigned int i,read_len;
	FILE* fd = fopen(file_path,"rb");
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
	do{
		read_len = fread(buf,1,sizeof(buf),fd);	
		SHA256_Update(&sha256,buf,read_len);
	}while(read_len >= sizeof(buf));
    SHA256_Final(hash, &sha256);
	Log_print(__FUNCTION__,"%s hash:",file_path);
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        Log_print("%X",hash[i]);
    }
	Log_print("\n");
	fclose(fd);
}

int base64_decode(char *in_str, int in_len, char *out_str)
{
     BIO *b64, *bio;
     int size = 0;
 
     if (in_str == NULL || out_str == NULL)
         return -1;
 
     b64 = BIO_new(BIO_f_base64());
     BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
 
     bio = BIO_new_mem_buf(in_str, in_len);
     bio = BIO_push(b64, bio);
 
     size = BIO_read(bio, out_str, in_len);
     out_str[size] = '\0';
 
     BIO_free_all(bio);
     return size;
 }

bool aes_ecb_file_decrypt(const char* input_file,const char* output_file)
{
	int read_len = 0;
	unsigned char aes_input[16] = {0};	
	FILE *in_fd = fopen(input_file,"rb");
	FILE *out_fd = fopen(output_file,"wb");
	if(in_fd == NULL)
	{
		Log(__FUNCTION__,"open failed\n");
	}
	if(out_fd == NULL)
	{
		Log(__FUNCTION__,"open failed\n");
	}
	do{
		memset(aes_input, 0x00, sizeof(aes_input)); 
		read_len = fread(aes_input,1,sizeof(aes_input),in_fd);	
		aes_decrypt(aes_input, &read_len,in_fd);
		fwrite(aes_input,read_len,1,out_fd);
	}while(read_len >= 16);
	fclose(in_fd);
	fclose(out_fd);
	return true;
}

unsigned long long str_to_long(char* buf)
{
	if(buf == NULL)
	{
		return 0;
	}
	unsigned long long sum = 0;
	while(*buf != '\0')
	{
		sum *= 10;
		sum += *buf - '0';
		buf++;
	}
	return sum;
}

unsigned char str_to_char(char* str)
{
        if(str == NULL)
        {
                return 0;
        }
        return (((str[0] >= '0' && str[0] <= '9')?(str[0] - '0'):(str[0] - 'A' + 10)) << 4)+
                        ((str[1] >= '0' && str[1] <= '9')?(str[1] - '0'):(str[1] - 'A' + 10));
}


void get_current_date(date_time *date)
{
	time_t now;   
	struct tm *timenow;      
	time(&now);   
	timenow = localtime(&now);   
	date->tick = now;
	date->year = (timenow->tm_year + 1900) % 100;
	date->month = timenow->tm_mon + 1;
	date->day = timenow->tm_mday;
	date->hour = timenow->tm_hour;
	date->minute = timenow->tm_min;
	date->second = timenow->tm_sec;	
}

int sql_get_data(sqlite3 *db,unsigned char *date,unsigned char *data)
{
	int nrow = 0, ncolumn = 0,i = 0;
	char **azResult; 
	char *zErrMsg = 0;

	pthread_mutex_lock(&sql_mutex); 
	//SELECT * FROM car_log where SEND_FINISH=0 or SEND_FINISH=2 order by SEND_FINISH desc,SERIAL_NUM limit 0,1;
	char sql[256] = {0};
	sprintf(sql,"SELECT * FROM unsend_log where DATE=%s;",date);
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
	if(nrow != 0)
	{
		if(azResult[ncolumn] != NULL)
		{
			while(azResult[ncolumn + 1][2*i] != '\0')
			{
				data[i] = str_to_char(azResult[ncolumn + 1] + 2*i);
				i ++;
			}
			sprintf(sql,"delete from unsend_log where VEHICLE_DATA = '%s';",azResult[ncolumn + 1]);
			sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );  
			if(zErrMsg != 0)
			{
				Log(__FUNCTION__,zErrMsg); 
			}
		}
		else
		{
			sqlite3_free_table( azResult );
			pthread_mutex_unlock(&sql_mutex);
			return 0;
		}
			
	}
	else
	{
		sqlite3_free_table( azResult );
		pthread_mutex_unlock(&sql_mutex);
		return 0;
	}
	sqlite3_free_table( azResult );
	pthread_mutex_unlock(&sql_mutex);
	return i;	
}

int sql_check_unfinish(sqlite3 *db,unsigned char *data)
{
	int nrow = 0, ncolumn = 0,i = 0;
	char **azResult; 
	char *zErrMsg = 0;

	pthread_mutex_lock(&sql_mutex); 
	//SELECT * FROM car_log where SEND_FINISH=0 or SEND_FINISH=2 order by SEND_FINISH desc,SERIAL_NUM limit 0,1;
	char sql[2048] = {0};
	sprintf(sql,"SELECT * FROM unsend_log where (SEND_FINISH=0 or SEND_FINISH=2) order by SEND_FINISH desc,DATE limit 0,1;");
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
	if(nrow != 0)
	{
		if(azResult[ncolumn] != NULL)
		{
			while(azResult[ncolumn + 1][2*i] != '\0')
			{
				data[i] = str_to_char(azResult[ncolumn + 1] + 2*i);
				i ++;
			}
			sprintf(sql,"delete from unsend_log where VEHICLE_DATA = '%s';",azResult[ncolumn + 1]);
			sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );  
			if(zErrMsg != 0)
			{
				Log(__FUNCTION__,zErrMsg); 
			}
		}
		else
		{
			sqlite3_free_table( azResult );
			pthread_mutex_unlock(&sql_mutex);
			return 0;
		}
			
	}
	else
	{
		sqlite3_free_table( azResult );
		pthread_mutex_unlock(&sql_mutex);
		return 0;
	}
	sqlite3_free_table( azResult );
	pthread_mutex_unlock(&sql_mutex);
	return i;
}

void head_pack(unsigned char* data,int* offset,unsigned char cmd_flag,unsigned char ack_flag,unsigned char encrypt_type)
{
	//head
	*offset = 0;
	data[(*offset)++] = DATA_HEAD;
	data[(*offset)++] = DATA_HEAD;
	data[(*offset)++] = cmd_flag;//DATA_CAR_MSG_SEND  DATA_CAR_MSG_REP
	data[(*offset)++] = ack_flag;
	memcpy(data + *offset,param.VIN,VIN_LENGTH);
	(*offset) += VIN_LENGTH;
	data[(*offset)++] = encrypt_type;
	//leave data length space
	(*offset) += 2;
}

void date_pack(unsigned char* data,date_time date,int* offset)
{
	//message package
	data[(*offset)++] = date.year;
	data[(*offset)++] = date.month;
	data[(*offset)++] = date.day;
	data[(*offset)++] = date.hour;
	data[(*offset)++] = date.minute;
	data[(*offset)++] = date.second;
}

void AES_pack(unsigned char* data,int* offset)
{
//	unsigned char buf[128] = {0};
	unsigned char after_encrypt[128] = {0};
	date_time date;
	int temp = 0;
	get_current_date(&date);
//	date_pack(buf,date,&temp);
//	buf[6] = 0x00;
//	buf[7] = 0x10;
//	RAND_bytes(buf + 8,16);
//	memcpy(AES_key,buf+8,16);
	unsigned char buf[128] = {0};
	date_pack(buf,date,&temp);
	buf[6] = 0x00;
	buf[7] = 0x10;
	RAND_bytes(buf + 8,16);
	memcpy(AES_key,buf+8,16);

	Log_print(__FUNCTION__,"aes key:");
	int i;
	for(i = 0;i < 128;i++)
	{
		Log_print("%X, ",buf[i]);
		if((i+1)%10 == 0)
		{
			Log_print("\n");
		}
	}
	Log_print("\n");
//	RSA_encrypt(buf,(char*)after_encrypt);
/*	for(i = 0;i < 128;i++)
	{
		Log_printf("0x%02X, ",buf[i]);
		if((i+1)%10 == 0)
		{
			Log_print("\n");
		}
	}
	Log_print("\n\n");
	for(i = 0;i < 128;i++)
	{
		Log_printf("0x%02X, ",after_encrypt[i]);
		if((i+1)%10 == 0)
		{
			Log_print("\n");
		}
	}*/
	memcpy(data+(*offset),after_encrypt,sizeof(after_encrypt));
	(*offset) += sizeof(after_encrypt);
}

void login_pack(unsigned char* data,int serial_num,int* offset)
{
	int i;//,j;
	data[(*offset)++] = (serial_num >> 8) & 0xFF;
	data[(*offset)++] = serial_num & 0xFF;
	for(i = 0;i < 20;i++)
	{
		data[(*offset)++] = param.ICCID[i];
	}
	data[(*offset)++] = param.btry_sys_num;
	data[(*offset)++] = param.sys_num_length;
/*	for(i = 0;i < param.btry_sys_num;i++)
	{
		for(j = 0;j < param.sys_num_length;j++)
		{
			data[(*offset)++] = param.sys_num[i][j];
		}
	}*/
}

void location_data_pack(unsigned char* data,location_t location,int* offset)
{
	data[(*offset)++] = LOCATION_DATA;
	data[(*offset)++] = location.status;
	data[(*offset)++] = ((int)(location.longitude*1000000) >> (3*8)) & 0xFF;
	data[(*offset)++] = ((int)(location.longitude*1000000) >> (2*8)) & 0xFF;
	data[(*offset)++] = ((int)(location.longitude*1000000) >> 8) & 0xFF;
	data[(*offset)++] = (int)(location.longitude*1000000) & 0xFF;	
	data[(*offset)++] = ((int)(location.latitude*1000000) >> (3*8)) & 0xFF;
	data[(*offset)++] = ((int)(location.latitude*1000000) >> (2*8)) & 0xFF;
	data[(*offset)++] = ((int)(location.latitude*1000000) >> 8) & 0xFF;
	data[(*offset)++] = (int)(location.latitude*1000000) & 0xFF;	
}
void can_pack(unsigned char* data,car_data_t car_data,int* offset)
{
	unsigned int i;
	data[(*offset)++] = CUSTOM_CAN;
	data[(*offset)++] = (car_data.custom_can.length>> 8) & 0xFF;
	data[(*offset)++] = car_data.custom_can.length & 0xFF;	
	for(i = 0; i< car_data.custom_can.length;i++)
	{
		data[(*offset)++] = car_data.custom_can.can_data[i];
	}
	
}

void end_pack(unsigned char* data,int offset)
{
	data[DATA_LENGTH_ADDR] = ((offset - 24) >> 8)&0xFF;
	data[DATA_LENGTH_ADDR+1] = (offset - 24)&0xFF;
}

void data_replacement()
{
	unsigned char data[1024] = {0};
	int length = 0;
	if(connect_ok)
	{
		length = sql_check_unfinish(db,data);
		if(length)
		{
			int offset = 0;
			head_pack(data,&offset,DATA_CAR_MSG_REP,DATA_CMD,NO_ENCRYPT);
			tcp_heart_send(sock_cli,data,length);
			//printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&send req date = %d %d\n",(int)data[28],(int)data[29]);
		}

	}
}

void sql_init(const char* path,sqlite3 **db)
{
	char *zErrMsg = 0;  
	int rc;  
	char *buf = {0};
	rc = sqlite3_open(path, db); 
	int timeout = 10;
	while(rc)  
	{  
	  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));  
	  sqlite3_close(*db);
	  sprintf(buf,"rm %s",path);
	  system(buf);
	  rc = sqlite3_open(path, db);
	  if(timeout-- < 0)
	  {
	  	system("reboot -f");
	  }
	}   
	//create a table,won't create if it exist then show in the zErrMsg
	char *sql = (char*)"CREATE TABLE IF NOT EXISTS car_log(\
	DATE VARCHAR(25) PRIMARY KEY,\
	VEHICLE_DATA VARCHAR(2048),\
	SEND_FINISH INTEGER\
	);" ;

	char *sql1 = (char*)"CREATE TABLE IF NOT EXISTS unsend_log(\
	DATE VARCHAR(25) PRIMARY KEY,\
	VEHICLE_DATA VARCHAR(2048),\
	SEND_FINISH INTEGER\
	);" ;

	char *sql2 =  (char*)"CREATE TABLE IF NOT EXISTS send_log(\
	DATE VARCHAR(25) PRIMARY KEY,\
	VEHICLE_DATA VARCHAR(2048),\
	SEND_FINISH INTEGER\
	);" ;
	pthread_mutex_lock(&sql_mutex);
	sqlite3_exec(*db,sql,0,0,&zErrMsg);
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}
	sqlite3_exec(*db,sql1,0,0,&zErrMsg);
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}
	sqlite3_exec(*db,sql2,0,0,&zErrMsg);
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}

	pthread_mutex_unlock(&sql_mutex);
}

void sql_save_unsend_data(sqlite3 *db,unsigned char *data,date_time param_date,int length)
{		
	char query_str[2048] = {0};
	char *zErrMsg = 0;  
	char date[25] = {0};
	char old_date[25] = {0};
	struct tm *time_history;
	time_t tick_history = param_date.tick - 3600 * 24 * SAVE_MAX_DAY;
	time_history = localtime(&tick_history);
	sprintf(old_date,"%02d-%02d-%02d %02d:%02d:%02d",
			(time_history->tm_year + 1900)%100,time_history->tm_mon + 1,time_history->tm_mday,time_history->tm_hour,time_history->tm_min,time_history->tm_sec);

	sprintf(date,"%02d-%02d-%02d %02d:%02d:%02d",
			data[24],data[25],data[26],data[27],data[28],data[29]);
	char sql_data[2048] = {0};
	int i;
	for(i = 0;i < length;i++)
	{
		sprintf(sql_data + i*2,"%02X",data[i]);
	}

	sprintf(query_str,"insert or ignore into unsend_log values('%s','%s',0);",date,sql_data);
	pthread_mutex_lock(&sql_mutex); 
	//Log(__FUNCTION__,"%s",query_str);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );  
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}	
	//delete log older than 7 days	
	memset(query_str,0,sizeof(query_str));
	sprintf(query_str,"delete from unsend_log where DATE < '%s';",old_date);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}

	pthread_mutex_unlock(&sql_mutex); 
}

void sql_save_send_data(sqlite3 *db,unsigned char *data,date_time param_date,int length)
{		
	char query_str[2048] = {0};
	char *zErrMsg = 0;  
	char date[25] = {0};
	char old_date[25] = {0};
	struct tm *time_history;
	time_t tick_history = param_date.tick - 3600 * 24 * SAVE_MAX_DAY;
	time_history = localtime(&tick_history);
	sprintf(old_date,"%02d-%02d-%02d %02d:%02d:%02d",
			(time_history->tm_year + 1900)%100,time_history->tm_mon + 1,time_history->tm_mday,time_history->tm_hour,time_history->tm_min,time_history->tm_sec);

	sprintf(date,"%02d-%02d-%02d %02d:%02d:%02d",
			data[24],data[25],data[26],data[27],data[28],data[29]);
	char sql_data[2048] = {0};
	int i;
	for(i = 0;i < length;i++)
	{
		sprintf(sql_data + i*2,"%02X",data[i]);
	}

	sprintf(query_str,"insert or ignore into send_log values('%s','%s',0);",date,sql_data);
	pthread_mutex_lock(&sql_mutex); 
	//Log(__FUNCTION__,"%s",query_str);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );  
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}	
	//delete log older than 7 days	
	memset(query_str,0,sizeof(query_str));
	sprintf(query_str,"delete from send_log where DATE < '%s';",old_date);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}

	pthread_mutex_unlock(&sql_mutex); 
}



bool sql_save_data(unsigned char *data,date_time param_date,int length)
{
	//printf("$$$$$$$$$$$$$$$$$$$$$$$ insert date = %d %d\n",param_date.minute,param_date.second);
	int i;  
	int ret = true;
	char date[25] = {0};
	char old_date[25] = {0};
	struct tm *time_history;
	time_t tick_history = param_date.tick - 3600 * 24 * SAVE_MAX_DAY;
	time_history = localtime(&tick_history);
	sprintf(date,"%02d-%02d-%02d %02d:%02d:%02d",
			param_date.year,param_date.month,param_date.day,param_date.hour,param_date.minute,param_date.second);
	sprintf(old_date,"%02d-%02d-%02d %02d:%02d:%02d",
			(time_history->tm_year + 1900)%100,time_history->tm_mon + 1,time_history->tm_mday,time_history->tm_hour,time_history->tm_min,time_history->tm_sec);
	pthread_mutex_lock(&sql_mutex); 	
	char *zErrMsg = 0;  
	char query_str[2048] = {0}; 
	char sql_data[2048] = {0};
	for(i = 0;i < length;i++)
	{
		sprintf(sql_data + i*2,"%02X",data[i]);
	}
	sprintf(query_str,"insert or ignore into car_log values('%s','%s',0);",date,sql_data);
	Log_print(__FUNCTION__,"sql save data\n");
	//Log(__FUNCTION__, "%s\n",query_str);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );  
	if(zErrMsg != 0)
	{
		ret = false;
		Log(__FUNCTION__,zErrMsg); 
	}
	//delete log older than 7 days	
	memset(query_str,0,sizeof(query_str));
	sprintf(query_str,"delete from car_log where DATE < '%s';",old_date);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );
	if(zErrMsg != 0)
	{
		ret = false;
		Log(__FUNCTION__,zErrMsg); 
	}
	pthread_mutex_unlock(&sql_mutex);
	return ret;
}

void sql_mark_send_failed(char* date)
{
	char query_str[256] = {0};	
	char *zErrMsg = 0; 
	pthread_mutex_lock(&sql_mutex);
	sprintf(query_str,"insert or ignore into unsend_log select * from send_log where DATE='%s'",date);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );  
	Log_print(__FUNCTION__,query_str);
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}
	pthread_mutex_unlock(&sql_mutex);
}

void sql_fault_mark()
{
	char date[25] = {0};
	time_t now;   
	struct tm *timenow;      
	time(&now);  
	//Retrospective data 
	now -= ERROR_REPLACEMENT_TIME;
	timenow = localtime(&now);   
	
	pthread_mutex_lock(&sql_mutex);
	char *zErrMsg = 0;  
	sprintf(date,"%02d-%02d-%02d %02d:%02d:%02d",
			(timenow->tm_year + 1900)%100,timenow->tm_mon + 1,timenow->tm_mday,timenow->tm_hour,
			timenow->tm_min,timenow->tm_sec);
	char query_str[256] = {0}; 
	char query_str1[256] = {0};
	char query_str2[256] = {0};
	//update car_log set SEND_FINISH=2 where DATE>"2017-04-05 17:14:52:000";
	//insert into unsend_log select * from car_log where SEND_FINISH=2;
	sprintf(query_str,"insert or ignore into unsend_log select * from car_log where DATE>'%s' and SEND_FINISH!=2;",date);
	sprintf(query_str1,"update car_log set SEND_FINISH=2 where DATE>'%s';",date);
	sprintf(query_str2,"update unsend_log set SEND_FINISH=2 where DATE>'%s';",date);
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );  
	Log_print(__FUNCTION__,query_str);
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}
	sqlite3_exec( db , query_str1 , 0 , 0 , &zErrMsg );  
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}	
	sqlite3_exec( db , query_str2 , 0 , 0 , &zErrMsg );  
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}		
	pthread_mutex_unlock(&sql_mutex);
	
}

/*void sql_resend_time_plus(sqlite3 *db,unsigned char* date)
{
	//update score_manage set  score = score - 积分 where id =1;
	char query_str[256] = {0}; 	
	sprintf(query_str,"update unsend_log set RESEND_TIME=RESEND_TIME+1 where DATE='%s';",date);
	pthread_mutex_lock(&sql_mutex);
	char *zErrMsg = 0;  
	sqlite3_exec( db , query_str , 0 , 0 , &zErrMsg );  
	Log(__FUNCTION__,query_str);
	if(zErrMsg != 0)
	{
		Log(__FUNCTION__,zErrMsg); 
	}
	pthread_mutex_unlock(&sql_mutex);
}*/
void heart_beat(int sock)
{
	date_time date;
	unsigned char data[1024] = {0};
	int offset = 0;
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_HEART,DATA_CMD,NO_ENCRYPT);
	date_pack(data,date,&offset);
	tcp_heart_send(sock,data,offset);
}

static char send_old_time[6] = {0};
static char save_old_time[6] = {0};
void* sql_thread(void* sql_param)
{
	Log(__FUNCTION__,"sql thread start->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	unsigned char data[1024];
	int length;
#ifdef SQL_OPEN
	sql_init(SQL_FILE_PATH,&db);
#endif
	struct timeval tv;
	sql_thread_cancel = false;
	int heart_beat_time = 0;
	while(!sql_thread_cancel)
	{
		tv.tv_sec = 0; 
		tv.tv_usec = 500000;		
		if(select(0, NULL, NULL, NULL, &tv) == -1)
 		{
	        		perror("sql select");
	    	}
		else
		{
			int offset = 24;
			

			memset(data,0,sizeof(data));
			memcpy(data,gl_data.data,gl_data.len);
			date_time date;
			get_current_date(&date);
			date_pack(data,date,&offset);
			offset = gl_data.len;
			length = gl_data.len;
			//printf("$$$$$$$$$$$$$$$$$$$$$$$ now date = %d %d\n",date.minute,date.second);
			if(--heart_test_timer <= 0)
			{
				if((param.GB32690_type == e_off) || (param.GB32690_type == e_normal))
				{	
					heart_beat(sock_cli);
				}
				else if(++heart_beat_time < (param.before_offline_time/10))
				{
					heart_beat(sock_cli);
				}
				heart_test_timer = param.heart_period*2;
			}
			if(--sql_save_timer <= 0)
			{
				if(memcmp(data+24,save_old_time,6))
				{
#ifdef SQL_OPEN
					if(!sql_save_data(data,date,offset))
					{
						Log(__FUNCTION__,"sql is break\n");
						remove(SQL_FILE_PATH);
						sql_init(SQL_FILE_PATH,&db);
					}
#endif
					memcpy(save_old_time,data+24,6);
				}
				sql_save_timer = param.save_period*2;
			}
			if(--period <= 0)
			{
				if(length != 0)
				{
				
					if(data[31]){
						extendQueue->packetGPSOffset(data, &offset,false);
					}else{					
						extendQueue->packetGPSOffset(data, &offset,true);
					}
					length = offset;
					
					if(connect_ok)
					{
						if(memcmp(data+24,send_old_time,6))
						{
							tcp_heart_send(sock_cli,data,offset);
#ifdef SQL_OPEN
							sql_save_send_data(db,data,date,offset);
#endif
							memcpy(send_old_time,data+24,6);
						}
					}
					else
					{
						Log(__FUNCTION__,"network not ready save unsend data");
#ifdef SQL_OPEN
						sql_save_unsend_data(db,data,date,offset);
#endif
					}
				}
				else
				{
					Log(__FUNCTION__,"len = 0");
				}
				Log_print(__FUNCTION__,"param.data_period=%d",param.data_period);
				period = param.data_period*2;
			}
			else
			{
#ifdef SQL_OPEN
				data_replacement();
#endif
			}	
		}
	}
	sqlite3_close(db); 
	return NULL;
}


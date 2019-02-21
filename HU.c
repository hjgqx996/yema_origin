#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include<string.h>
#include"HU.h"
#include <pthread.h>
#include <ql_nw.h>
#include<general.h>
#include <stdbool.h> 
#include  <netinet/in.h>
#include <netinet/tcp.h>


//#include<WinSock2.h>
//#include <socket.h>
#define TBOX_SOCK_PORT 6000
#define TBOX_SERVICE_ADDR "192.168.225.1"
#define MAX_4G_BUF_SIZE  4*1024
#define MSG_HEADER_SIZE 4
#define DISP_MSG_SIZE 6
#define TBOX_MSG_FIX_HEAD 0x5566
#define AID_
#define MAX_CMD_SIZE 200
#define MAX_TIMEOUT_SECONDS      1 //1
#define HEART_BEAT_TIME 60//
#define SO_DONTLINGER TRUE
#define TRUE 1
//#define SO_REUSEADDR 2

extern param_t param;
static uint8_t send_4g_state_ack[] 	= {0x02, 0x01, 0x00, 0x02, 0x00, 0x01, 0x01};//AID,MID,Data_lenACK
static uint8_t send_4g_state[] 	= {0x02,0x01, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00};//AID,MID,Data_len,stata,CSQ
static uint8_t send_heart_beat_ack[]     = {0x01, 0x01, 0x00, 0x02, 0x00, 0x00};
static uint8_t send_VIN_data_ack[]       = {0x03, 0x01, 0x00, 0x02, 0x00, 0x01,0x01};
static uint8_t send_VIN_data[23]		  = {0x03, 0x01, 0x00, 0x03, 0x00, 0x11,0 };

static uint8_t send_ICCID_data_ack[]       = {0x04, 0x01, 0x00, 0x02, 0x00, 0x01,0x01};
static uint8_t send_ICCID_data[26]		  = {0x04, 0x01, 0x00, 0x03, 0x00, 0x14,0 };

static uint8_t send_sim_state_ack[]       = {0x05, 0x01, 0x00, 0x02, 0x00, 0x01,0x01};
static uint8_t send_sim_state[]		  = {0x05, 0x01, 0x00, 0x03, 0x00, 0x01,0x00};

static uint8_t send_call_state_ack[]       = {0x06, 0x01, 0x00, 0x02, 0x00, 0x01,0x01};
static uint8_t send_call_state[]		  = {0x06, 0x01, 0x00, 0x03, 0x00, 0x01,0x00};

static uint8_t send_call_start_ack[]       = {0x07, 0x01, 0x00, 0x02, 0x00, 0x01,0x01};
//static uint8_t send_call_start[]		  = {0x07, 0x01, 0x00, 0x03, 0x00, 0x01,0x00};
static uint8_t send_call_end_ack[]       = {0x08, 0x01, 0x00, 0x02, 0x00, 0x01,0x01};
static uint8_t send_sms_send_ack[]       = {0x09, 0x01, 0x00, 0x02, 0x00, 0x01,0x01};

static uint8_t send_location_info_ack[] 	= {0x0A, 0x01, 0x00, 0x02, 0x00, 0x01, 0x01};//AID,MID,Data_lenACK
static uint8_t send_location_info[10] 	= {0x0A,0x01, 0x00, 0x03, 0x00, 0x04};//AID,MID,Data_len,stata,CSQ


//int network_stat_report_start = 1;
static uint8_t send_4g_state_flag = 0;
static uint8_t send_4g_state_ack_flag = 0;
static uint8_t send_heart_beat_flag = 0;
static uint8_t send_VIN_data_ack_flag = 0;
static uint8_t send_VIN_data_flag = 0;
static uint8_t send_ICCID_data_ack_flag = 0;
static uint8_t send_ICCID_data_flag = 0;

static uint8_t send_sim_state_ack_flag = 0;
static uint8_t send_sim_state_flag = 0;
static uint8_t send_call_state_ack_flag = 0;
static uint8_t send_call_state_flag = 0;
static uint8_t send_call_start_ack_flag = 0;
static uint8_t send_call_start_flag = 0;
static uint8_t send_call_end_ack_flag = 0;
static uint8_t send_call_end_flag = 0;
static uint8_t send_sms_send_ack_flag = 0;
static uint8_t send_sms_send_flag = 0;

static uint8_t sen_locaton_info_ack_flag = 0;
static uint8_t send_locaton_info_flag = 0;


int recv_4g_state_reporte_cycle = 5;//Ĭʹֵ5
char recv_buff[256];
char actual_recv_buff[4*1024];
char send_buff[256];
char actual_send_buff[256];
pthread_mutex_t HU_mutex;
time_t cur_time1;
time_t pre_time1= 0;
time_t pre_time2= 0;

char vin[]={1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};
char phone_number[32]={0};
ST_VCALL_CallRef reference=0;
E_QL_VOICE_CALL_STATE_T event=3;
voice_client_handle_type *ph_voice;
uint8_t encode_type = 0;
char sms_buf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};
int location_info_reporte_cycle = 5;//Ĭʹֵ5
QL_LOC_LOCATION_INFO_T *pt_location;
voice_client_handle_type h_voice = 0;
loc_client_handle_type h_loc = 0;

static void ql_loc_rx_ind_msg_cb(loc_client_handle_type  h_loc,E_QL_LOC_NFY_MSG_ID_T e_msg_id,void *pv_data, void *contextPtr)
{
    QL_USER_LOG("e_msg_id=%d\n", e_msg_id); 
    switch(e_msg_id)
    {
        case E_QL_LOC_NFY_MSG_ID_STATUS_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_LOCATION_INFO:
        {
            pt_location = (QL_LOC_LOCATION_INFO_T *)pv_data;
            printf("**** flag=0x%X, Latitude = %lf, Longitude=%lf, accuracy = %lf ****\n", 
                        pt_location->flags, 
                        pt_location->latitude, 
                        pt_location->longitude, 
                        pt_location->accuracy);
            break;
        }
        case E_QL_LOC_NFY_MSG_ID_SV_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_NMEA_INFO:
        {
            QL_LOC_NMEA_INFO_T  *pt_nmea = (QL_LOC_NMEA_INFO_T  *)pv_data;

            printf("NMEA info: timestamp=%lld, length=%d, nmea=%s\n", 
                    pt_nmea->timestamp, pt_nmea->length, pt_nmea->nmea); 
            break;
        }
        case E_QL_LOC_NFY_MSG_ID_CAPABILITIES_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_AGPS_STATUS:
            break;
        case E_QL_LOC_NFY_MSG_ID_NI_NOTIFICATION:
            break;
        case E_QL_LOC_NFY_MSG_ID_XTRA_REPORT_SERVER:
            break;
    }
}

void get_gps_info_by_cb(void)
{
    int                     ret         = E_QL_OK;
    int                     bitmask     = 1;
    QL_LOC_POS_MODE_INFO_T  t_mode      = {0};
    //QL_LOC_LOCATION_INFO_T  t_loc_info  = {0};

    ret = QL_LOC_Client_Init(&h_loc);
    printf("QL_LOC_Client_Init ret %d with h_loc=%d\n", ret, h_loc);
    
    ret = QL_LOC_AddRxIndMsgHandler(ql_loc_rx_ind_msg_cb, (void*)h_loc);  
    printf("QL_LOC_AddRxIndMsgHandler ret %d\n", ret);  
    /* Set what we want callbacks for */
    ret = QL_LOC_Set_Indications(h_loc, bitmask);
    printf("QL_LOC_Set_Indications ret %d\n", ret);

    t_mode.mode                 = E_QL_LOC_POS_MODE_STANDALONE;
    t_mode.recurrence           = E_QL_LOC_POS_RECURRENCE_PERIODIC;
    t_mode.min_interval         = 1000;  //report nmea frequency 1Hz
    t_mode.preferred_accuracy   = 50;    // <50m
    t_mode.preferred_time       = 90;    // 90s
    ret = QL_LOC_Set_Position_Mode(h_loc, &t_mode);
    printf("QL_LOC_Set_Position_Mode ret %d\n", ret);
    
    ret = QL_LOC_Start_Navigation(h_loc);
    printf("QL_LOC_Start_Navigation ret=%d\n", ret);
}

void do_not_get_gps_info(void)
{
	int ret;
	ret = QL_LOC_Stop_Navigation(h_loc);
    printf("QL_LOC_Stop_Navigation ret=%d\n", ret);
    ret = QL_LOC_Client_Deinit(h_loc);
    printf("QL_LOC_Client_Deinit ret=%d\n", ret);
}
static void ql_voice_call_cb_func(int                     call_id,
                                  char*                   phone_num,
                                  E_QL_VOICE_CALL_STATE_T state,
                                  void                    *contextPtr)
{
    ph_voice = (voice_client_handle_type *)contextPtr;
    if(*ph_voice == 0)//Call_id got here, you can save it for the later operation
    {
        *ph_voice = call_id;
    }
    event = state;
}

int UTF8ToUnicode ( char *ch, int *unicode)  
{  
    char *p = NULL;  
    int e = 0, n = 0;  
    if((p = ch) && unicode)  
    {  
        if(*p >= 0xfc)  
        {
            e = (p[0] & 0x01) << 30;  
            e |= (p[1] & 0x3f) << 24;  
            e |= (p[2] & 0x3f) << 18;  
            e |= (p[3] & 0x3f) << 12;  
            e |= (p[4] & 0x3f) << 6;  
            e |= (p[5] & 0x3f);  
            n = 6;  
        }  
        else if(*p >= 0xf8)   
        {
            e = (p[0] & 0x03) << 24;  
            e |= (p[1] & 0x3f) << 18;  
            e |= (p[2] & 0x3f) << 12;  
            e |= (p[3] & 0x3f) << 6;  
            e |= (p[4] & 0x3f);  
            n = 5;  
        }  
        else if(*p >= 0xf0)  
        {
            e = (p[0] & 0x07) << 18;  
            e |= (p[1] & 0x3f) << 12;  
            e |= (p[2] & 0x3f) << 6;  
            e |= (p[3] & 0x3f);  
            n = 4;  
        }  
        else if(*p >= 0xe0)  
        {
            e = (p[0] & 0x0f) << 12;  
            e |= (p[1] & 0x3f) << 6;  
            e |= (p[2] & 0x3f);  
            n = 3;  
        }  
        else if(*p >= 0xc0)   
        {
            e = (p[0] & 0x1f) << 6;  
            e |= (p[1] & 0x3f);  
            n = 2;  
        }  
        else   
        {  
            e = p[0];  
            n = 1;  
        }  
        *unicode = e;  
    }  
     
    return n;  
}  


int UTF8StrToUnicodeStr( char   *utf8_str,  
                        uint16_t        *unicode_str, 
                        int             unicode_str_size)  
{  
    int unicode = 0;  
    int n = 0;  
    int count = 0;  
    char *s = NULL;  
    uint16_t *e = NULL;  
      
    s = utf8_str;  
    e = unicode_str;  
      
    if ((utf8_str) && (unicode_str))  
    {  
        while (*s)  
        {  
            if ((n = UTF8ToUnicode (s, &unicode)) > 0)  
            {  
                if ((count + 1) >= unicode_str_size)  
                {  
                    return count;  
                }  
                else 
                {  
                #if 0//LE
                    *e = (unsigned short) unicode;  
                #else//BE
                    *e = ((unicode&0xFF)<<8) | ((unicode&0xFF00) >> 8);  
                #endif
                    e++;  
                    *e = 0;                     
                    s += n;  
                    count++;
                }  
            }  
            else 
            {
                return count;  
            }  
        }  
    }  
      
    return count;  
} 

char char_to_dec(char c) 
{
	char value = 0;
	
	if ((c >= '0')&&(c <= '9')) {
		value = c - 48;
	} else if ((c >= 'A')&&(c <= 'Z')) {
		value = c - 65 + 10;
	} else if ((c >= 'a')&&(c <= 'z')) {
		value = c - 97 + 10;
	}
	
	return value;	
}

int char_to_hex(char *pbuf, int read_len) 
{
	char tmp_buf[MAX_4G_BUF_SIZE] = {0};
	int i,len=0;
	
	if (!pbuf)
		return -1;

#if 0	
	printf("read org data[%d]: ", read_len);
	for (i = 0; i < read_len; i++) {
		printf("0x%02x,",pbuf[i]);
	}
	printf("\n");
#endif

	if (read_len % 2 == 1)
		read_len--;
	
	for (i = 0; i < read_len; i+=2) {
		tmp_buf[len] = (char_to_dec(pbuf[i])) * 16 + char_to_dec(pbuf[i + 1]);
		len++;
	}

	memset(pbuf, 0, read_len);
	memcpy(pbuf, tmp_buf, len);
	
#if 0
	printf("read hex data[%d]: ", len);
	for (i = 0; i < len; i++) {
		printf("0x%02x,",pbuf[i]);
	}
	printf("\n");
#endif

	return len;
}

static void data_parse(char *recv_data_buf, int org_data_len) 
{
	char *pbuf = NULL;
    header_t header;
    dispatcher_message_t disp_msg;
    int msg_len,i;
    // int gps_data_start = 0;
    char check_sum = 0;
    uint16_t data_len = 0;
    //	uint8_t network_state, network_csq;
		
	pbuf = recv_data_buf;
	msg_len = char_to_hex(pbuf, org_data_len);//תЭӦĸʽ
    if (msg_len < (MSG_HEADER_SIZE + DISP_MSG_SIZE)) {
        //Log(__FUNCTION__,"receive data too short! msg_len=%d\n",msg_len);
		printf("receive data too short! msg_len=%d\n",msg_len);
        return;
    }
        printf("the received data is:\n");
	for (i = 0; i < msg_len-1; i++)
		{
	    	check_sum ^= pbuf[i];
			printf("%c",pbuf[i]);
		}

    //DBG("msg_len=%d, check_sum=0x%02x\n",msg_len,check_sum);
    //DBG("pbuf end: 0x%02x,0x%02x,0x%02x\n", pbuf[msg_len-3],pbuf[msg_len-2],pbuf[msg_len-1]);        

    if (check_sum != pbuf[msg_len-1]){
		printf("the check sum is :%c\n",pbuf[msg_len-1]);
        printf("Warnning: receive data check sum error!\n");
    }
	memcpy(&header, pbuf, MSG_HEADER_SIZE);
	memcpy(&disp_msg, pbuf + MSG_HEADER_SIZE, DISP_MSG_SIZE);

	pbuf = pbuf + MSG_HEADER_SIZE + DISP_MSG_SIZE;
	msg_len = msg_len - MSG_HEADER_SIZE - DISP_MSG_SIZE;

    data_len = disp_msg.app_data_len & 0xff;//ģʽλڵλ
    data_len = (data_len << 8) | ((disp_msg.app_data_len >> 8) &0xff);
	//Log(__FUNCTION__,"aid=0x%x, mid=0x%x, app_data_len=%d\n", disp_msg.aid, disp_msg.mid, data_len);
	printf("aid=0x%x, mid=0x%x, app_data_len=%d\n", disp_msg.aid, disp_msg.mid, data_len);
	pthread_mutex_lock(&HU_mutex);
	 if (disp_msg.aid == AID_NETWORK_STATE) {//network state	
		if (disp_msg.mid == MID_NETWORK_STATE_REQUEST) {
			if (pbuf[0] == CMD_NETWORK_STATE_SEND_START) {
				send_4g_state_flag = 1;
				recv_4g_state_reporte_cycle = pbuf[1];
				send_4g_state_ack_flag = 1;
				//Log(__FUNCTION__,"recv network state request\n");
				printf("recv network state request\n");
				printf("recv network state request\n");
				printf("recv network state request\n");
			} else if(pbuf[0] == CMD_NETWORK_STATE_SEND_STOP){
				send_4g_state_flag = 0;
				send_4g_state_ack_flag = 1;
			}else
				printf("network state cmd request fail!\n");
		}
		
	 }else if (disp_msg.aid == AID_HEART_BEAT) {//tbox heart beat ack		
		if (disp_msg.mid == MID_HEART_BEAT_REQUEST){
			send_heart_beat_flag = 1;
			//Log(__FUNCTION__,"Receive tbox heart beat request!\n");
			printf("Receive tbox heart beat request!\n");
	 	}
	 }else if(disp_msg.aid == AID_VIN_DATA ){
		if(disp_msg.mid == MID_VIN_REQUEST_DATA ){
			send_VIN_data_flag =1;
			send_VIN_data_ack_flag = 1;
		}
	 }else if(disp_msg.aid == AID_ICCID ){
		if(disp_msg.mid == MID_ICCID_REQUEST ){
			send_ICCID_data_flag =1;
			send_ICCID_data_ack_flag = 1;
		}
	 }else if(disp_msg.aid == AID_SIM_STATE){
		if(disp_msg.mid == MID_SIM_STATE_REQUEST){
			send_sim_state_ack_flag = 1;
			send_sim_state_flag = 1;
		}
	 }else if(disp_msg.aid == AID_CALL_STATE){
		if(disp_msg.mid == MID_CALL_STATE_REQUEST){
			send_call_state_ack_flag = 1;
			send_call_state_flag = 1;
		}
	 }else if(disp_msg.aid == AID_CALL_START){
		if(disp_msg.mid == MID_CALL_START_REQUEST){
         	for(i=0;i<data_len-1;i++)
         	{
				phone_number[i]=pbuf [i];
			}
			send_call_start_ack_flag = 1;
			send_call_start_flag = 1;
		}
	 }else if(disp_msg.aid == AID_CALL_END){
		if(disp_msg.mid == MID_CALL_END_REQUEST){
			
			send_call_end_ack_flag = 1;
			send_call_end_flag = 1;
		}
	 }else if(disp_msg.aid == AID_SMS_SEND){
		if(disp_msg.mid == MID_SMS_SEND_REQUEST){
			memcpy(phone_number,pbuf,32);
			memcpy(&encode_type,pbuf+32,1);
			memcpy(sms_buf,pbuf+32+1,QL_SMS_MAX_MT_MSG_LENGTH);
			send_sms_send_ack_flag = 1;
		    send_sms_send_flag = 1;
		}
	 }else if(disp_msg.aid == AID_LOCATIN_INFO_SEND){
		if(disp_msg.mid == MID_LOCATIN_INFO_REQUEST){
			if(pbuf[0] == 0){
				location_info_reporte_cycle = pbuf[1];
				sen_locaton_info_ack_flag = 1;
                send_locaton_info_flag = 1;
				get_gps_info_by_cb();
			}else{
				sen_locaton_info_ack_flag = 1;
                send_locaton_info_flag = 0;
				do_not_get_gps_info();
			}
			
		}
	 }
	pthread_mutex_unlock(&HU_mutex);
	return;
}

char dec_to_char(char value) 
{
	char c = ' ';
	if ((value >= 0)&&(value <= 9))
		c = value + 48;
	else if ((value >= 10)&&(value <= 15))
		c = value - 10 + 65;
	return c;	
}

int hex_to_char(char *pbuf, int len)
{
	char tmp_buf[MAX_CMD_SIZE]={0};
	int char_len = 0;
	int i;

	if (!pbuf)
		return -1;

	for (i = 0; i < len; i++) {
		tmp_buf[char_len] = dec_to_char(pbuf[i]/16);
		tmp_buf[char_len+1] = dec_to_char(pbuf[i]%16);
		char_len+=2;
	}
	
	memcpy(pbuf, tmp_buf, char_len);

	return char_len;
}
int get_network_state(char* pnetwork_state)
{
	 static nw_client_handle_type h_nw = 0;
	 int ret = E_QL_OK;
	 QL_MCM_NW_SIGNAL_STRENGTH_INFO_T    t_info;
	 ret=QL_MCM_NW_Client_Init(&h_nw);
	 printf("QL_MCM_NW_Client_Init ret = %d, detail info:\n", ret);
     memset(&t_info, 0, sizeof(QL_MCM_NW_SIGNAL_STRENGTH_INFO_T));
     ret = QL_MCM_NW_GetSignalStrength(h_nw, &t_info);
     printf("QL_MCM_NW_GetSignalStrength ret = %d, detail info:\n", ret);
	 
     if(t_info.gsm_sig_info_valid)
     {
     	printf("t_info.gsm_sig_info.rssi:%d\n",t_info.gsm_sig_info.rssi);
		*pnetwork_state=1;
        return t_info.gsm_sig_info.rssi;
     } 
     else if(t_info.wcdma_sig_info_valid)
     {
     	printf("t_info.wcdma_sig_info.rssi:%d\n",t_info.wcdma_sig_info.rssi);
		*pnetwork_state=2;
        return t_info.wcdma_sig_info.rssi;
     } 
     else if(t_info.tdscdma_sig_info_valid)
     {
		printf("t_info.tdscdma_sig_info.rssi:%d\n",t_info.tdscdma_sig_info.rssi);
		*pnetwork_state=2;
		return t_info.tdscdma_sig_info.rssi;
     } 
     else if(t_info.lte_sig_info_valid)
     {
		printf("t_info.lte_sig_info.rssi:%d\n",t_info.lte_sig_info.rssi);
		*pnetwork_state=3;
		return t_info.lte_sig_info.rssi;
     } 
     else if(t_info.cdma_sig_info_valid)
     {
		printf("t_info.cdma_sig_info.rssi:%d\n",t_info.cdma_sig_info.rssi);
		*pnetwork_state=2;
		return t_info.cdma_sig_info.rssi;
     } 
     else if(t_info.hdr_sig_info_valid)
     {     
     	printf("t_info.hdr_sig_info.rssi:%d\n",t_info.hdr_sig_info.rssi);
		*pnetwork_state=2;
        return t_info.hdr_sig_info.rssi;
     }
	 else
	 {
	 	printf("no service\n");
		*pnetwork_state=0;
		return 85;
	 }
	 
	 return 0;
}
int make_cmd(char *pbuf, uint8_t *cmd_data, int len)
{
	uint8_t app_data[100];
	header_t header;
	dispatcher_message_t disp_msg;
	uint8_t check_sum;
	int msg_len = 0;
	int data_len = 0;
	int i;
	if (len < 6) {
		//Log(__FUNCTION__,"Cmd_data buffer too small len = %d", len);
		printf("Cmd_data buffer too small len = %d", len);
		return -1;
	}
	

	memset(&header, 0, sizeof(header_t));
	memset(&disp_msg, 0, sizeof(dispatcher_message_t));
	header.msg_fix_header = TBOX_MSG_FIX_HEAD;
	disp_msg.aid = cmd_data[1];
	disp_msg.aid = (disp_msg.aid << 8)|cmd_data[0]; //0x0128;
	disp_msg.mid = cmd_data[3];
	disp_msg.mid = (disp_msg.mid << 8)|cmd_data[2]; //0x0100;
	disp_msg.app_data_len = cmd_data[5];
	disp_msg.app_data_len = (disp_msg.app_data_len << 8)|cmd_data[4]; //0x0200

	data_len = (cmd_data[4] << 8) | cmd_data[5];//cmd_data[4]ǷУ
	header.msg_size = sizeof(dispatcher_message_t) + data_len;
	
	if (len < (6 + data_len)) {
		//Log(__FUNCTION__,"Cmd_data buffer too small len = %d", len);
		printf("Cmd_data buffer too small len = %d", len);
		return -1;
	}
	
	for (i = 0; i < data_len; i++)
		app_data[i] = cmd_data[6 + i];

	check_sum = 0;
	msg_len = 0;
		
	memcpy(pbuf, &header, sizeof(header_t));
	msg_len = MSG_HEADER_SIZE;

	memcpy(pbuf + msg_len, &disp_msg, sizeof(dispatcher_message_t));
	msg_len += DISP_MSG_SIZE;

	memcpy(pbuf + msg_len, app_data, data_len);
	msg_len += data_len;

	//DBG("msg_len=%d", msg_len);
	for (i = 0; i < msg_len; i++)
		check_sum ^= pbuf[i];
	//DBG("check_sum=0x%02x", check_sum);

	pbuf[msg_len] = check_sum;
	msg_len++;

	if (MAX_CMD_SIZE < msg_len * 2) {
		//Log(__FUNCTION__,"msg_len=%d, is over size!", msg_len);
		printf("msg_len=%d, is over size!", msg_len);
		return -1;
	}
	
	return hex_to_char(pbuf, msg_len);
}

int send_cmd (int fd)
{
	char cmd_buf[MAX_CMD_SIZE];//Ҫ͵תɿɼַ
	int cmd_len;
	char cmd_buf1[MAX_CMD_SIZE];
	//int cmd_len1;
	int ret=1;
	int stat;
	int err=0;
	int i;
	int len;
	int voice_call_id = 0;
	E_QL_SMS_FORMAT_T   e_format= 0;
    char sms_buf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};
    ql_sms_info_t *pt_sms_info = NULL;
	static sms_client_handle_type h_sms;
	char network_state;
	int rssi=0;
	char csq=0;
	if (fd < 0) {
		//Log(__FUNCTION__,"Warnning: fd = %d\n",fd);
		printf("Warnning: fd = %d\n",fd);
		ret = 0;
		return ret;
	}
	err = recv(fd, cmd_buf1, sizeof(cmd_buf1), MSG_DONTWAIT|MSG_PEEK);  
	if(err == 0 || (err < 0 && errno != EAGAIN))  
	{
		printf("the client is off\n");
		usleep(500*1000);
		ret = 0;
		return ret;
	}
	time(&cur_time1);//
	if (send_4g_state_ack_flag == 1){
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_4g_state_ack, sizeof(send_4g_state_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"Send ack for request_4g_state_cmd, cmd_len = %d\n", cmd_len);
			printf("Send ack for request_4g_state_cmd, cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		if (send_4g_state_flag == 1)
			time(&pre_time1);
		send_4g_state_ack_flag = 0;
	}else if(send_4g_state_flag == 1){
		if(cur_time1 - pre_time1 >recv_4g_state_reporte_cycle){//עʱ4g״̬źţöʱܱHUĻǰһ㷢(recv_4g_state_reporte_cycle -1)
			err = recv(fd, cmd_buf1, sizeof(cmd_buf1), MSG_DONTWAIT|MSG_PEEK);  
			if(err == 0 || (err < 0 && errno != EAGAIN))  
			{
				printf("the client is off\n");
				usleep(500*1000);
				ret = 0;
				return ret;
			}
			usleep(1000*1000);
			rssi=get_network_state(&network_state);
			if(network_state!=0)
			{
				if(rssi>=-51) 
				{
					rssi=-51;
				}
				else if(rssi<=-113)
				{
					rssi=-113;
				}
			}
			else 
				rssi=85;
			csq=(char)((rssi+113)/2);
			send_4g_state[6]=network_state;
			send_4g_state[7]=csq;
			cmd_len = make_cmd(cmd_buf, send_4g_state, sizeof(send_4g_state));//send_4g_state
			if (cmd_len > 0)
			{
				//Log(__FUNCTION__,"Send 4g state cmd_len = %d\n", cmd_len);
				printf("Send 4g state cmd_len = %d\n", cmd_len);
				printf("Send 4g state h_nw = %c\n", send_4g_state[6]);
				printf("Send 4g state csq = %c\n", send_4g_state[7]);
				write(fd, cmd_buf, cmd_len);
			}
			pre_time1= cur_time1;
		}
	
	}

	time(&cur_time1);//
	if (sen_locaton_info_ack_flag== 1){
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_location_info_ack, sizeof(send_location_info_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"Send ack for request_4g_state_cmd, cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		if (send_locaton_info_flag == 1){
			time(&pre_time2);
			printf("Send ack for location info send, cmd_len = %d\n", cmd_len);
		}else{
			printf("Send ack for location info send  stop, cmd_len = %d\n", cmd_len);
		}
		send_locaton_info_flag = 0;
	}else if(send_locaton_info_flag == 1){
		if(cur_time1 - pre_time2 > location_info_reporte_cycle){		
			usleep(1000*1000);
			send_location_info[6]= (pt_location->flags & 0xFF);
			send_location_info[7]= ((pt_location->flags >>8) & 0xFF);
			cmd_len = make_cmd(cmd_buf, send_location_info, sizeof(send_location_info));//send_4g_state
			sprintf(cmd_buf+cmd_len ,"%lf", pt_location->latitude);
			sprintf(cmd_buf+cmd_len+1,"%lf", pt_location->longitude);
			sprintf(cmd_buf+cmd_len+2,"%lf", pt_location->accuracy);
			cmd_len+=3;
			if (cmd_len > 0)
			{
				//Log(__FUNCTION__,"Send 4g state cmd_len = %d\n", cmd_len);
				printf("location info  cmd_len = %d\n", cmd_len);
				write(fd, cmd_buf, cmd_len);
			}
			pre_time2= cur_time1;
			//usleep(3000*1000);
		}
	}
	
	if(send_heart_beat_flag == 1){
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_heart_beat_ack, sizeof(send_heart_beat_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send heart beat ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_heart_beat_flag = 0;
		//usleep(1000*1000);
	}

	else if(send_VIN_data_ack_flag ==1){
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_VIN_data_ack, sizeof(send_VIN_data_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send VIN data ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_VIN_data_ack_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_VIN_data_flag == 1)
	{	
		usleep(1000*1000);
		for (i= 0; i <17;++i){//18
			send_VIN_data[6+i] = param.VIN[i];
		}
		cmd_len = make_cmd(cmd_buf, send_VIN_data, sizeof(send_VIN_data));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send VIN data , final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
			send_VIN_data_flag = 0;
		}
		//usleep(1000*1000);
	}
	else if(send_ICCID_data_ack_flag ==1){
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_ICCID_data_ack, sizeof(send_ICCID_data_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send ICCID data ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_ICCID_data_ack_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_ICCID_data_flag == 1)
	{
		usleep(1000*1000);
		for (i= 0; i <20; ++i){//21
			send_ICCID_data[6+i] = param.ICCID[i];
		}
		cmd_len = make_cmd(cmd_buf, send_ICCID_data, sizeof(send_ICCID_data));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send ICCID data , final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
			send_ICCID_data_flag  = 0;
		}
		//usleep(1000*1000);
	}
	else if(send_sim_state_ack_flag ==1){
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_sim_state_ack, sizeof(send_sim_state_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send sim state ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_sim_state_ack_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_sim_state_flag == 1)
	{
		usleep(1000*1000);
		send_sim_state[6]=QL_SIM_GetState(E_QL_SIM_EXTERNAL_SLOT_1);
		cmd_len = make_cmd(cmd_buf, send_sim_state, sizeof(send_sim_state));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send sim state , final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
			send_sim_state_flag = 0;
		}
		
	}
	else if(send_call_state_ack_flag == 1)
	{
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_call_state_ack, sizeof(send_call_state_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send call state ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_call_state_ack_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_call_state_flag == 1)
	{
		ql_mcm_voice_calls_state_t t_info = {0};
		usleep(1000*1000);
		stat= QL_Voice_Call_Client_Init(&h_voice);
	    if(stat!= 0 && stat!= 1)
	    {
	        printf("QL_Voice_Call_Client_Init FAIL\n");
	       	ret = 0;
			return ret;
	    }
		QL_Voice_Call_GetCallStatus(h_voice, -1, &t_info); // -1<0 means all, you can specified it with voice_call_id
		printf("QL_Voice_Call_GetCallStatus ret = %d\n", ret);
		printf("the call number is %d\n",t_info.calls_len);
		if(t_info.calls_len == 1)
		{
			send_call_state[6]=t_info.calls[0].state & 0x07;
			cmd_len = make_cmd(cmd_buf, send_call_state, sizeof(send_call_state));
			if (cmd_len > 0) {
				//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
				printf("send call state, final_cmd_len = %d\n", cmd_len);
				write(fd, cmd_buf, cmd_len);
			}
		}
		
		send_call_state_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_call_start_ack_flag == 1)
	{
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_call_start_ack, sizeof(send_call_start_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send call start ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_call_start_ack_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_call_start_flag == 1)
	{
		usleep(1000*1000);
		stat = QL_Voice_Call_Start(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1, phone_number, &voice_call_id);
		if(stat ==0)
		{
			printf(" voice call could not be processed\n");
		}else 
		{
			printf("the phone is on\n");
		}
		send_call_start_flag = 0;
		//usleep(1000*1000);
	}
	
	else if(send_call_end_ack_flag == 1)
	{
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_call_end_ack, sizeof(send_call_end_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send call end ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_call_end_ack_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_call_end_flag == 1)
	{
		usleep(1000*1000);
		stat = QL_Voice_Call_End(h_voice, *ph_voice);
		if(stat ==0)
		{
			printf(" the end of voice call could not be processed\n");
		}else 
		{
			printf("the  voice call  is end \n");
		}
		send_call_end_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_sms_send_ack_flag == 1)
	{
		usleep(1000*1000);
		cmd_len = make_cmd(cmd_buf, send_sms_send_ack, sizeof(send_sms_send_ack));
		if (cmd_len > 0) {
			//Log(__FUNCTION__,"send heart beat ack, final_cmd_len = %d\n", cmd_len);
			printf("send sms send ack, final_cmd_len = %d\n", cmd_len);
			write(fd, cmd_buf, cmd_len);
		}
		send_sms_send_ack_flag = 0;
		//usleep(1000*1000);
	}
	else if(send_sms_send_flag == 1)
	{
	     QL_SMS_Client_Init(&h_sms);
		 pt_sms_info = (ql_sms_info_t*)malloc(sizeof(ql_sms_info_t));
         if(pt_sms_info == NULL)
         {
               printf("Malloc fail!\n");
               return ret;
         }
		 memset(pt_sms_info, 0, sizeof(ql_sms_info_t));
       	 memcpy(pt_sms_info->src_addr,phone_number,strlen(phone_number));
		 e_format = encode_type;
         e_format = e_format & 0x03;

         if((e_format == E_QL_SMS_FORMAT_GSM_7BIT) || (e_format == E_QL_SMS_FORMAT_UCS2))
         {
             len= strlen(sms_buf); //to skip 0x0A
             sms_buf[len-1] = '\0';
             printf("textData[%d]:%s\n", len, sms_buf);                            
             printf("input %d byte data:", len);
             for(i=0; i<len; i++)
             {
                 printf("%.2X ", sms_buf[i]);
             }
             printf("\n");
         }
		 else{
			 len= strlen(sms_buf);		
		 }
		 if(e_format == E_QL_SMS_FORMAT_GSM_7BIT)
         {
             memcpy(pt_sms_info->sms_data, sms_buf, QL_SMS_MAX_MT_MSG_LENGTH);
             pt_sms_info->sms_data_len = strlen(sms_buf);
         }
         else if(e_format == E_QL_SMS_FORMAT_BINARY_DATA)
         {
             memcpy(pt_sms_info->sms_data, sms_buf, len);
             pt_sms_info->sms_data_len = len;
         }
         else
         {
             len = UTF8StrToUnicodeStr(sms_buf, (uint16_t*)pt_sms_info->sms_data,len);// Got BE data
             pt_sms_info->sms_data_len = len * 2;//in bytes
             printf("raw UTF-16 len=%d, data:", pt_sms_info->sms_data_len);
             for(i=0; i<pt_sms_info->sms_data_len; i++)
             {
                 printf("%.2X ", pt_sms_info->sms_data[i]);
             }
             printf("\n");
        }
        pt_sms_info->format = e_format;  
        stat = QL_SMS_Send_Sms(h_sms, pt_sms_info);
        printf("#QL_SMS_Send_Sms stat=%d \n", stat);
        free(pt_sms_info);
		stat = QL_SMS_Client_Deinit(h_sms);
        printf("QL_SMS_Client_Deinit ret=%d \n", stat);
	}
	return ret;
}


void* HU_thread(void* HU_param)
{
	int s_fd,c_fd;
	int s_len,c_len;
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	char recv_data_buf[MAX_4G_BUF_SIZE];
	int recv_data_len;
	time_t pre_time, cur_time;
	//time_t cur_time1;
	int fcntl_flag;
	int on=1;
	//bool bDontLinger = FALSE;
	struct linger so_linger;
	so_linger.l_onoff = TRUE;
	so_linger.l_linger = 0;
	s_addr.sin_family=AF_INET;
	s_addr.sin_addr.s_addr=inet_addr(TBOX_SERVICE_ADDR);//inet_addr(TBOX_SERVICE_ADDR);htonl(INADDR_ANY);//ʽʮַתΪʽַ
	s_addr.sin_port=htons(TBOX_SOCK_PORT);//ж˿ں:ݵĴ轫ֽתֽ
	s_len=sizeof(s_addr);
	c_len=sizeof(c_addr);
	for (;;) {	
		/*socket,ʧظִиö*/
		if ((s_fd = socket(AF_INET,SOCK_STREAM, 0)) == -1) {
	    	//Log(__FUNCTION__,"Create socket fail, errno=%d\n", errno);//ʹϵͳûʹÿ⺯cŻλ
	    	printf("Create socket fail \n");
	        usleep(500*1000);	
			continue;
	    }
		printf("s_fd = %d\n", s_fd);
		time(&cur_time);
		time(&pre_time);
	    for (;;) {
			
 			time(&cur_time);
			printf("iam listening \n");
			printf("ICCID=%s\n",param.ICCID);
			printf("VIN=%s\n",param.VIN);
			pre_time=cur_time;
			
			setsockopt(s_fd,SOL_SOCKET,SO_LINGER,&so_linger,sizeof(so_linger));
			setsockopt (s_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) ;
			int ret1=0;
			ret1=bind(s_fd,(struct sockaddr *)&s_addr,s_len);
			printf("bind ecm %d\n",ret1);
			
			ret1=listen(s_fd,5);
			printf("listen ecm %d\n",ret1);
	        if ((c_fd=accept(s_fd, (struct sockaddr*)&c_addr,(socklen_t *)&c_len)) == -1) {
	            //Log(__FUNCTION__,"Socket accept fail, errno=%d\n", errno);
	            printf("Socket accept fail \n");
	        } else {
	            //Log(__FUNCTION__,"Socket accept successful!\n");
	            printf("Socket accept successful!\n");
	            break;
	        }
	    }
		fcntl_flag = fcntl(c_fd, F_GETFL, 0);
	   	if (fcntl_flag >= 0) {
			fcntl_flag |= O_NONBLOCK;
			if (fcntl(c_fd, F_SETFL, fcntl_flag) < 0)
				//Log(__FUNCTION__,"fcntl set O_NONBLOCK mode fail!\n");
				printf("fcntl set O_NONBLOCK mode fail!\n");
			else
				//Log(__FUNCTION__,"fcntl set O_NONBLOCK mode successful!\n");
				printf("client fcntl set O_NONBLOCK mode successful!\n");
		} else {
				printf("fcntl get flag fail!\n");
		}
		time(&pre_time);
		/*HU˵ʱݣѭȡHU˵ݲʱδյ*/
	    for (;;) {
			time(&cur_time);//pre_time
			if (send_cmd(c_fd) ==0)
			{
				printf("break\n");
				break;
			}
			memset(recv_data_buf, 0, sizeof(recv_data_buf));
			recv_data_len = read(c_fd, recv_data_buf, sizeof(recv_data_buf));//ѭȡ˵ݣʱ˳
			if (recv_data_len > 0){//ȡݣݽд
				data_parse(recv_data_buf, recv_data_len);
				pre_time = cur_time;
			 	printf("[len=%d] recv HU data: %s\n", recv_data_len, recv_data_buf);
			}
			else if(send_heart_beat_flag == 0){//δȡҳʱδܵźţӿͻ˵
				if(cur_time - pre_time > 2*HEART_BEAT_TIME){
					printf("did not received the heart beat");
					pre_time = cur_time;
					break;
				}
			}
	    }
		shutdown(s_fd,SHUT_RDWR);
		close(s_fd);//رsocketļ
		s_fd = -1;
		shutdown(c_fd,SHUT_RDWR);
		close(c_fd);//رsocketļ
		c_fd = -1;
		//Log(__FUNCTION__,"====>Socket connect again!\n");
		printf("====>Socket connect again!\n");
		QL_Voice_Call_RemoveStateHandler(h_voice);
    	printf("QL_Voice_Call_RemoveStateHandler\n");
		QL_Voice_Call_Client_Deinit(h_voice);
		printf("QL_Voice_Call_Client_Deinit\n");
	}
	return 0;
}

	





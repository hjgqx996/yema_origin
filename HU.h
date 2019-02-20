#ifndef __HU_H
#define __HU_H
#include "ql_oe.h"


typedef struct _header_t {
	uint16_t msg_fix_header;
	uint16_t msg_size;
} header_t; //10bytes,  actually take up 12bytes because of align


typedef struct _dispatcher_message_t {
	uint16_t aid;
	uint16_t mid;
	uint16_t app_data_len;
} dispatcher_message_t; //20bytes

typedef enum _tbox_network_state
{
	AID_NETWORK_STATE = 0x0102,
	MID_NETWORK_STATE_REQUEST = 0x0100,
	MID_NETWORK_STATE_RESPOND = 0x0200,
	MID_NETWORK_STATE_REPORT = 0x0300,
	CMD_NETWORK_STATE_SEND_START = 0,
	CMD_NETWORK_STATE_SEND_STOP = 1,
	//CMD_NETWORK_STATE_ACK_OK = 1,
	//CMD_NETWORK_STATE_ACK_FAIL = 0,
}tbox_network_state;

typedef enum _tbox_heart_beat
{
	AID_HEART_BEAT = 0x0101,
	MID_HEART_BEAT_REQUEST = 0x0100,
	MID_HEART_BEAT_RESPOND = 0x0200,
}tbox_heart_beat;	

typedef enum _tbox_vin
{
	AID_VIN_DATA = 0x0103,
	MID_VIN_REQUEST_DATA = 0x0100,
	MID_VIN_RESPOND_DATA  = 0x0200,
	MID_VIN_REPORT_DATA= 0x0300,
	CMD_VIN_DATA_ACK_OK= 1,
	CMD_VIN_DATA_ACK_FAIL = 0,
}tbox_vin;

typedef enum _tbox_iccid
{
	AID_ICCID = 0x0104,
	MID_ICCID_REQUEST = 0x0100,
	MID_ICCID_RESPOND  = 0x0200,
	MID_ICCID_REPORT = 0x0300,
	CMD_ICCID_ACK_OK = 1,
	CMD_ICCID_ACK_FAIL = 0,
}tbox_iccid;

typedef enum _tbox_sim
{
	AID_SIM_STATE =0x0105,
	MID_SIM_STATE_REQUEST = 0x0100,
	MID_SIM_STATE_RESPOND  = 0x0200,
	MID_SIM_STATE_REPORT = 0x0300,
	CMD_SIM_STATE_ACK_OK = 1,
	CMD_SIM_STATE_ACK_FAIL = 0,
}tbox_sim;

typedef enum _tbox_call_state
{
	AID_CALL_STATE =0x0106,
	MID_CALL_STATE_REQUEST = 0x0100,
	MID_CALL_STATE_RESPOND  = 0x0200,
	MID_CALL_STATE__REPORT = 0x0300,
	CMD_CALL_STATE_ACK_OK = 1,
	CMD_CALL_STATE_ACK_FAIL = 0,
}tbox_call_state;
	

typedef enum _tbox_call_start
{
	AID_CALL_START=0x0107,
	MID_CALL_START_REQUEST = 0x0100,
	MID_CALL_START_RESPOND  = 0x0200,
	MID_CALL_START__REPORT = 0x0300,
	CMD_CALL_START_ACK_OK = 1,
	CMD_CALL_START_ACK_FAIL = 0,
}tbox_call_start;

typedef enum _tbox_call_end
{
	AID_CALL_END=0x0108,
	MID_CALL_END_REQUEST = 0x0100,
	MID_CALL_END_RESPOND  = 0x0200,
	MID_CALL_END__REPORT = 0x0300,
	CMD_CALL_END_ACK_OK = 1,
	CMD_CALL_END_ACK_FAIL = 0,
}tbox_call_end;


typedef enum _tbox_sms_send
{
	AID_SMS_SEND=0x0109,
	MID_SMS_SEND_REQUEST = 0x0100,
	MID_SMS_SEND_RESPOND  = 0x0200,
	MID_SMS_SEND_REPORT = 0x0300,
	CMD_SMS_SEND_ACK_OK = 1,
	CMD_SMS_SEND_ACK_FAIL = 0,
}tbox_sms_send;


typedef enum _tbox_location_info
{
	AID_LOCATIN_INFO_SEND=0x010A,
	MID_LOCATIN_INFO_REQUEST = 0x0100,
	MID_LOCATIN_INFO_RESPOND  = 0x0200,
	MID_LOCATIN_INFO_REPORT = 0x0300,
	CMD_LOCATIN_INFO_ACK_OK = 1,
	CMD_LOCATIN_INFO_ACK_FAIL = 0,
}tbox_location_info;
static void ql_loc_rx_ind_msg_cb(loc_client_handle_type  h_loc,E_QL_LOC_NFY_MSG_ID_T e_msg_id,void *pv_data, void *contextPtr);
void get_gps_info_by_cb(void);
void do_not_get_gps_info(void);
static void ql_voice_call_cb_func(int call_id, char* phone_num, E_QL_VOICE_CALL_STATE_T state, void *contextPtr);	
int UTF8ToUnicode (char *ch, int *unicode) ;

int UTF8StrToUnicodeStr( char   *utf8_str,uint16_t *unicode_str,  int unicode_str_size);

char char_to_dec(char c) ;
int char_to_hex(char *pbuf, int read_len); 
static void data_parse(char *recv_data_buf, int org_data_len);
char dec_to_char(char value);
int hex_to_char(char *pbuf, int len);
int get_network_state(char* pnetwork_state);
int make_cmd(char *pbuf, uint8_t *cmd_data, int len);
int send_cmd (int fd);
void* HU_thread(void* HU_param);
int recv_data_checksum(char* buff);

#endif

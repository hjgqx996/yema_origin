//------------------------------------------------------------------------------
/*
 * Thie example creates a transparent transmission channel between standard 
 * input/output device and tcp server, which demonstrates how to program 
 * network function, including activating network, create socket, connect
 * to tcp server, and recv/send socket data.
 */
//------------------------------------------------------------------------------
extern "C" 
{	
#include "ql_oe.h"
#include "network.h"
#include "4tbox_define.h"
#include "sql.h"
#include "mcu.h"
#include "gps.h"
#include "audio.h"
#include "comdef.h"
#include "ql_qcmap_client_api.h"
#include "general.h"
#include "t_ssl.h"
}

#include "ctrl_air.pb.h"
#include "ctrl_tbox_param.pb.h"
#include "ctrl_charge.pb.h"
#include "ctrl_windows.pb.h"
#include "ctrl_engine.pb.h"
#include "ctrl_tbox_param.pb.h"
#include "ctrl_all_state.pb.h"
#include "ctrl_doors.pb.h"
#include "state_oil_mass.pb.h"
#include "ctrl_ekey.pb.h"
#include "ctrl_chair.pb.h"
#include "term_version_upload.pb.h"
#include "term_command_result.pb.h"
#include "ctrl_fota.pb.h"
#include <string>
#include "shellUpdate.h"


//--------------------------------------------------------------
/*
 * Define IP address and socket port
 */
//--------------------------------------------------------------


#define MAX_CHARACTER_SIZE    8 

#define BUFFER_SIZE 1024


extern int wakelock_fd;
extern pthread_mutex_t tcp_mutex;

bool connect_ok = false;
bool network_thread_cancel = false;
bool mcu_upgrade = false;

int GPRS_signal;
extern param_t param;
extern upgrade_param_t upgrade_param;
extern bool mcu_ready;
extern bool real_wakeup;
extern bool heart_beat_ack;
int sock_cli;
int plat_sock;
int msgid_net_to_mcu;
bool platform_link_on = false;
bool connected = false;

extern unsigned char EOL[32];
extern unsigned char pipe_Skey[32];

extern unsigned char app_version[32];
extern unsigned char mcu_version[32];

extern unsigned char air_state[20];
extern unsigned char door_state[4];
extern unsigned char tire_preture[8];
extern unsigned char oil_mass[2];
extern device_check_param_t device_check_param;
extern SSL *ssl;
bool module_init_thread_restart = false;

static unsigned long long find_car_request_id;
static unsigned long long engine_start_request_id;
//static unsigned long long charge_start_request_id;
static unsigned long long remote_ctrl_enable_request_id;
//static unsigned long long top_windows_ctrl_request_id;
//static unsigned long long windows_ctrl_request_id;
static unsigned long long air_ctrl_request_id;
static unsigned long long air_temperature_ctrl_request_id;
//static unsigned long long char_ctrl_request_id;
//static unsigned long long electromotor_ctrl_request_id;
static unsigned long long door_lock_request_id;
static unsigned long long truck_door_lock_request_id;
//static unsigned long long panel_protection_request_id;

static unsigned long long door_control_valide;
static unsigned long long air_control_valide;

enum remote_control_type{
	no_control_type =0 ,
	find_car_type ,
	engine_start_type,
	charge_start_type,
	electromotor_type,
	remote_ctrl_enable_type,
	windows_ctrl_type,
	top_windows_ctrl_type,
	air_ctrl_type,
	air_temperature_ctr_type,
	char_ctrl_type,
	door_lock_type,
	truck_door_lock_type,
	panel_protection_type
};

unsigned char statusBuf[BUFFER_SIZE];
static remote_control_type control_type = no_control_type;

static void deal_net_check(int sock)
{
	std::string msg;
	unsigned char data[1024] = {0};
	date_time date;
	int offset = 0;
	CtrlTboxParamMessage ctrlParamMessage;
	ConfigInfo configInfo;
	VersionInfo versionInfo;
	StateInfo stateInfo;
	//config info
	configInfo.set_local_cycle(param.save_period);
	configInfo.set_report_cycle(param.data_period);
	configInfo.set_alarm_cycle(param.alert_period);
	configInfo.set_enterprise_platform_url(param.url);
	configInfo.set_enterprise_platform_port(param.port);
	configInfo.set_public_platform_url(param.platform_url);
	configInfo.set_public_platform_port(param.platform_port);
	configInfo.set_tbox_timeout(param.tbox_timeout);
	configInfo.set_platform_timeout(param.tcp_rcv_timeout);
	configInfo.set_interval(param.next_login_time);
	configInfo.set_heartbeat_cycle(param.heart_period);
	configInfo.set_tbox_usb_net_switch(param.usbnet);
	configInfo.set_offline_config_param((const char*)EOL);
	configInfo.set_peps_security_auth_key((const char*)pipe_Skey);
	ctrlParamMessage.set_allocated_config_info(&configInfo);

	//version info
	versionInfo.set_upper_ware_version(APP_VERSION);
	versionInfo.set_lower_ware_version("V1.03");
	versionInfo.set_upper_firmware_version("EC20");
	ctrlParamMessage.set_allocated_version_info(&versionInfo);
	//stateinfo
	stateInfo.set_sampling(0);
	ctrlParamMessage.set_allocated_state_info(&stateInfo);
	
	ctrlParamMessage.SerializeToString(&msg);
		
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_CHECK,DATA_SUCCESS,NO_ENCRYPT);
	date_pack(data,date,&offset);
	memcpy(data+offset,msg.data(),msg.length());
	offset += msg.length();
	tcp_heart_send(sock,data,offset);	

	ctrlParamMessage.release_config_info();
	ctrlParamMessage.release_version_info();
	ctrlParamMessage.release_state_info();
	return;
}

static void deal_get_version(int sock,unsigned char *buf)
{
	std::string msg;
	unsigned char data[1024] = {0};
	date_time date;
	int offset = 0;

	TermVersionUploadData termVersionUploadData;
	termVersionUploadData.set_respcode(100);
	
	TermVersionUpload *PC_version = termVersionUploadData.add_termversionupload();
	PC_version->set_termtype(3);
	PC_version->set_manufacturerid("LUCHANG");
	PC_version->set_hardwareversion("EC20_YEMA_TBOX_V0.1");
	PC_version->set_firmwareversion(APP_VERSION);
	Log(__FUNCTION__,"app_version = %s\n",APP_VERSION);
	
	TermVersionUpload *MCU_version = termVersionUploadData.add_termversionupload();
	MCU_version->set_termtype(4);
	MCU_version->set_manufacturerid("LUCHANG");
	MCU_version->set_hardwareversion("EC20_YEMA_TBOX_V0.1");
	MCU_version->set_firmwareversion((char*)mcu_version);
	
	Log(__FUNCTION__,"mcu_version = %s\n",mcu_version);

	char firewareVersion[32]={0};
	getFirewareVersion(firewareVersion);
	TermVersionUpload *firmware_ver = termVersionUploadData.add_termversionupload();
	firmware_ver->set_termtype(5);
	firmware_ver->set_manufacturerid("LUCHANG");
	firmware_ver->set_hardwareversion("EC20_YEMA_TBOX_V0.1");
	firmware_ver->set_firmwareversion(firewareVersion);
	
	Log(__FUNCTION__,"firewareVersion = %s\n",firewareVersion);

	termVersionUploadData.SerializeToString(&msg);

	get_current_date(&date);
	head_pack(data,&offset,DATA_GET_VERSION,DATA_SUCCESS,NO_ENCRYPT);
	date_pack(data,date,&offset);
	memcpy(data+offset,msg.data(),msg.length());
	offset += msg.length();
	tcp_heart_send(sock,data,offset);	
}

static void deal_net_set(int sock,unsigned char *buf)
{
	date_time date;
	unsigned char data[512] = {0};
	int offset = 0;
	CtrlTboxParamMessage ctrlParamMessage;
	ConfigInfo configInfo;
	int str_len = 0;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	bool ret = ctrlParamMessage.ParseFromArray(buf+30,len - 6);
	Log(__FUNCTION__,"ret= %d\n",ret);
/*	if(ret != true)
	{
		return;
	}*/
	if(ctrlParamMessage.config_info().has_local_cycle());
	{
		param.save_period = ctrlParamMessage.config_info().local_cycle();
		Log(__FUNCTION__,"param.save_period=%d",param.save_period);
	}
	if(ctrlParamMessage.config_info().has_report_cycle())
	{
		param.data_period = ctrlParamMessage.config_info().report_cycle();
		Log(__FUNCTION__,"param.data_period=%d",param.data_period);
	}
	if(ctrlParamMessage.config_info().has_alarm_cycle())
	{
		param.alert_period = ctrlParamMessage.config_info().alarm_cycle();
		Log(__FUNCTION__,"param.alert_period=%d",param.alert_period);
	}
	if(ctrlParamMessage.config_info().has_enterprise_platform_url())
	{
		str_len = ctrlParamMessage.config_info().enterprise_platform_url().length();
		ctrlParamMessage.config_info().enterprise_platform_url().copy(param.url,str_len,0);
		Log(__FUNCTION__,"param.url=%s",param.url);
	}
	if(ctrlParamMessage.config_info().has_enterprise_platform_port())
	{
		param.port = ctrlParamMessage.config_info().enterprise_platform_port();
		Log(__FUNCTION__,"param.port=%d",param.port);
	}
	if(ctrlParamMessage.config_info().has_public_platform_url())
	{
		str_len = ctrlParamMessage.config_info().public_platform_url().length();
		ctrlParamMessage.config_info().public_platform_url().copy(param.platform_url,str_len,0);
		Log(__FUNCTION__,"param.platform_url=%s",param.platform_url);
	}
	if(ctrlParamMessage.config_info().has_public_platform_port())
	{
		param.platform_port= ctrlParamMessage.config_info().public_platform_port();
		Log(__FUNCTION__,"param.platform_port=%d",param.platform_port);
	}
	if(ctrlParamMessage.config_info().has_tbox_timeout())
	{
		param.tbox_timeout= ctrlParamMessage.config_info().tbox_timeout();
		Log(__FUNCTION__,"param.tbox_timeout=%d",param.tbox_timeout);
	}
	if(ctrlParamMessage.config_info().has_platform_timeout())
	{
		param.tcp_rcv_timeout= ctrlParamMessage.config_info().platform_timeout();
		Log(__FUNCTION__,"param.tcp_rcv_timeout=%d",param.tcp_rcv_timeout);
	}
	if(ctrlParamMessage.config_info().has_interval())
	{
		param.next_login_time= ctrlParamMessage.config_info().interval();
		Log(__FUNCTION__,"param.next_login_time=%d",param.next_login_time);
	}
	if(ctrlParamMessage.config_info().has_heartbeat_cycle())
	{
		param.heart_period= ctrlParamMessage.config_info().heartbeat_cycle();
		Log(__FUNCTION__,"param.heart_period=%d",param.heart_period);
	}
	if(ctrlParamMessage.config_info().has_tbox_usb_net_switch())
	{
		param.usbnet= ctrlParamMessage.config_info().tbox_usb_net_switch();
		if(param.usbnet){
			system("iptables -D FORWARD -m physdev --physdev-in ecm0 -j DROP");
		}else{
			system("iptables -A FORWARD -m physdev --physdev-in ecm0 -j DROP");
		}
		Log(__FUNCTION__,"param.usbnet=%d",param.usbnet);
	}
	
	init_save_param(PARAM_PATH);
	init_save_param(BAK_PARAM_PATH);
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_SET,DATA_SUCCESS,NO_ENCRYPT);
	date_pack(data,date,&offset);
	tcp_heart_send(sock,data,offset);
}

bool down_load_verify(char* sign_base64,const char* download_path)
{

	char sign_enc[256] = {0};
	char sign_dec[1024] = {0};
	unsigned char hash[32] = {0};
	char exec[128] = {0};
	int len;
	//下载文件
	sprintf(exec,"wget -T 30 -t 5 -O /home/root/tbox_encrypt %s",download_path);
	system(exec);
	system("sync");
	//解密文件
	aes_ecb_file_decrypt("/home/root/tbox_encrypt","/home/root/tbox_decrypt");
	//获取文件摘要
	sha256_file("/home/root/tbox_decrypt",hash);
	//解密签名
	len =  base64_decode(sign_base64,strlen(sign_base64),sign_enc);
	RSA_pub_decrypt("/home/root/MX_pub.key",(unsigned char*)sign_enc,sign_dec);
	
	Log_print(__FUNCTION__,"hash:");
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        Log_print("%X",sign_dec[len-32+i]);
    }
	Log_print(__FUNCTION__,"\n");

	//验证签名
	if(!memcmp(hash,sign_dec+len-32,32))
	{
		Log(__FUNCTION__,"verify success\n");
		return true;
	}
	else
	{
		Log(__FUNCTION__,"verify failed\n");
		return false;
	}	
}

static void deal_upgrade(int sock,unsigned char *buf)
{
	unsigned char data[1024] = {0};
	int offset = 0;
	date_time date;
	std::string msg,msg1;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	CtrlFotaData ctrlFotaData;
	ctrlFotaData.ParseFromArray(buf+31,len-7);
	ctrlFotaData.set_respcode(100);
	ctrlFotaData.SerializeToString(&msg);
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
	date_pack(data,date,&offset);
	data[offset++] = buf[CHECK_CMD_NUM_ADDR];
	memcpy(data+offset,msg.data(),msg.length());
	offset += msg.length();
	
	Log(__FUNCTION__,"upgrade type = %d\n",ctrlFotaData.termtype());
	Log(__FUNCTION__,"offset = %d\n",offset);
	tcp_heart_send(sock,data,offset);
	
	Log(__FUNCTION__,"path = %s\n",ctrlFotaData.upgradepath().c_str());
	Log(__FUNCTION__,"sign = %s\n",ctrlFotaData.sign().c_str());

	get_upgrade();
	if(ctrlFotaData.termtype() == 3){
		upgrade_param.pc_upgrade_status = 1;
		memset(upgrade_param.pc_upgrade_path,0,sizeof(upgrade_param.pc_upgrade_path));
		memset(upgrade_param.pc_upgrade_sign,0,sizeof(upgrade_param.pc_upgrade_sign));
		memcpy(upgrade_param.pc_upgrade_path,ctrlFotaData.upgradepath().c_str(),ctrlFotaData.upgradepath().length());
		memcpy(upgrade_param.pc_upgrade_sign,ctrlFotaData.sign().c_str(),ctrlFotaData.sign().length());
	}

	if(ctrlFotaData.termtype() == 4){
		upgrade_param.mcu_upgrade_status = 1;
		memset(upgrade_param.mcu_upgrade_path,0,sizeof(upgrade_param.mcu_upgrade_path));
		memset(upgrade_param.mcu_upgrade_sign,0,sizeof(upgrade_param.mcu_upgrade_sign));
		memcpy(upgrade_param.mcu_upgrade_path,ctrlFotaData.upgradepath().c_str(),ctrlFotaData.upgradepath().length());
		memcpy(upgrade_param.mcu_upgrade_sign,ctrlFotaData.sign().c_str(),ctrlFotaData.sign().length());
	}

	if(ctrlFotaData.termtype() == 6){
		upgrade_param.vcu_upgrade_status = 1;
		memset(upgrade_param.vcu_upgrade_path,0,sizeof(upgrade_param.vcu_upgrade_path));
		memset(upgrade_param.vcu_upgrade_sign,0,sizeof(upgrade_param.vcu_upgrade_sign));
		memcpy(upgrade_param.vcu_upgrade_path,ctrlFotaData.upgradepath().c_str(),ctrlFotaData.upgradepath().length());
		memcpy(upgrade_param.vcu_upgrade_sign,ctrlFotaData.sign().c_str(),ctrlFotaData.sign().length());
	}

	if(ctrlFotaData.termtype() == 7){
		upgrade_param.mcu_upgrade_status = 1;
		memset(upgrade_param.mcu_mcu_upgrade_path,0,sizeof(upgrade_param.mcu_mcu_upgrade_path));
		memset(upgrade_param.mcu_mcu_upgrade_sign,0,sizeof(upgrade_param.mcu_mcu_upgrade_sign));
		memcpy(upgrade_param.mcu_mcu_upgrade_path,ctrlFotaData.upgradepath().c_str(),ctrlFotaData.upgradepath().length());
		memcpy(upgrade_param.mcu_mcu_upgrade_sign,ctrlFotaData.sign().c_str(),ctrlFotaData.sign().length());
	}

	if(ctrlFotaData.termtype() == 8){
		upgrade_param.bms_upgrade_status = 1;
		memset(upgrade_param.bms_upgrade_path,0,sizeof(upgrade_param.bms_upgrade_path));
		memset(upgrade_param.bms_upgrade_sign,0,sizeof(upgrade_param.bms_upgrade_sign));
		memcpy(upgrade_param.bms_upgrade_path,ctrlFotaData.upgradepath().c_str(),ctrlFotaData.upgradepath().length());
		memcpy(upgrade_param.bms_upgrade_sign,ctrlFotaData.sign().c_str(),ctrlFotaData.sign().length());
	}

	init_save_upgrade();
	system("sync");

	offset = 0;
	TermCommandResult termCommandResult;
	termCommandResult.set_termtype(ctrlFotaData.termtype());
	termCommandResult.set_requestid(ctrlFotaData.requestid());
	char sign[1024] = {0};
	strcpy(sign,ctrlFotaData.sign().c_str());
	int ret = down_load_verify(sign,ctrlFotaData.upgradepath().c_str());
	if(ret)
	{
		termCommandResult.set_status(2);
		termCommandResult.set_message("no problem");
		termCommandResult.set_respcode(100);
	}
	else
	{
		termCommandResult.set_status(3);
		termCommandResult.set_message("upgrade failed");
		termCommandResult.set_respcode(-100);		
	}	

	termCommandResult.SerializeToString(&msg1);

	get_current_date(&date);
	head_pack(data,&offset,DATA_RES_UPLOAD,DATA_CMD,NO_ENCRYPT);
	date_pack(data,date,&offset);
	memcpy(data+offset,msg1.data(),msg1.length());
	offset += msg1.length();
	tcp_heart_send(sock,data,offset);	
	if(ret)
	{
		sleep(1);
		switch(ctrlFotaData.termtype())
		{
		case 3:
			if(mcu_upgrade == false)
			{
				PC_Power_cmd(PC_UPGRADE);
				Ql_SLP_WakeLock_Destroy(wakelock_fd);
				upgrade_param.pc_upgrade_status = 0;				
				init_save_upgrade();				
				addShellUpdateNum(backupShellType);
				system("sync");
				system("/home/root/update.sh &");
			}
			break;
		case 4:
			if(mcu_upgrade == false)
			{
				system("mv /home/root/tbox_decrypt /home/root/mcu.bin");
				system("sync");
				//mcu_upgrade = true;
			}
			break;

		case 6:
			if(mcu_upgrade == false)
			{
				system("mv /home/root/tbox_decrypt /home/root/vcu.bin");
				system("sync");
				//mcu_upgrade = true;
			}
			break;
		case 7:
			if(mcu_upgrade == false)
			{
				system("mv /home/root/tbox_decrypt /home/root/mcu_mcu.bin");
				system("sync");
				//mcu_upgrade = true;
			}
			break;

		case 8:
			if(mcu_upgrade == false)
			{
				system("mv /home/root/tbox_decrypt /home/root/bms.bin");
				system("sync");
				//mcu_upgrade = true;
			}
			break;
		}
		
	}
}

void get_all_status()
{
	std::string msg;
	int sock = sock_cli;
	unsigned char *buf = statusBuf;
	date_time date;
	unsigned char data[1024] = {0};
	int offset = 0;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];

	CtrlAllStateMessage recvStateMessage;
	CtrlAllStateMessage allStateMessage;
	
	recvStateMessage.ParseFromArray(buf+31,len-7);

	CtrlAirTemperatureMessage ctrlAirTemperatureMessage;
	AirTemperatureStateInfo airTemperatureStateInfo;
	airTemperatureStateInfo.set_set_temperature((get_air_conditon(ALL_VEHICLE)->temperature)*5);
	ctrlAirTemperatureMessage.set_allocated_air_temperature_state_info(&airTemperatureStateInfo);
	allStateMessage.set_allocated_air_temperature_message(&ctrlAirTemperatureMessage);

	CtrlAirMessage ctrlAirMessage;
	AirStateInfo airStateInfo;
	airStateInfo.set_ac(get_air_conditon(ALL_VEHICLE)->ac_status);
	airStateInfo.set_after_defrosting(get_air_conditon(ALL_VEHICLE)->behind_defront_status);
	airStateInfo.set_pre_defrosting(get_air_conditon(ALL_VEHICLE)->forward_defront_status);
	airStateInfo.set_blower_mode(get_air_conditon(ALL_VEHICLE)->blow_model);
	airStateInfo.set_cycle_mode(get_air_conditon(ALL_VEHICLE)->air_circulation_mode);
	airStateInfo.set_ptc(get_air_conditon(ALL_VEHICLE)->heating_status);
	airStateInfo.set_wind_level(get_air_conditon(ALL_VEHICLE)->windLevel);
	ctrlAirMessage.set_allocated_air_state_info(&airStateInfo);
	allStateMessage.set_allocated_air_message(&ctrlAirMessage);

	CtrlDoorsMessage ctrlDoorsMessage;
	DoorsStateInfo doorsStateInfo;
	doorsStateInfo.set_door_on_off((get_extend_vechicle_param()->door_status)&0xf);
	doorsStateInfo.set_door_state_exception((get_extend_vechicle_param()->door_exceprtion_status)&0xf);
	doorsStateInfo.set_lock_on_off((get_extend_vechicle_param()->door_lock_status)&0xf);
	doorsStateInfo.set_lock_state_exception((get_extend_vechicle_param()->door_lock_exception_status)&0xf);
	ctrlDoorsMessage.set_allocated_doors_state_info(&doorsStateInfo);
	allStateMessage.set_allocated_doors_message(&ctrlDoorsMessage);
	Log(__FUNCTION__,"door status = %x %x %x %x",get_extend_vechicle_param()->door_lock_status,get_extend_vechicle_param()->door_exceprtion_status,get_extend_vechicle_param()->door_lock_exception_status);

	CtrlTrunkDoorsMessage ctrlTrunkDoorsMessage;
	TrunkDoorsStateInfo trunkDoorsStateInfo;
	trunkDoorsStateInfo.set_trunk_on_off((get_extend_vechicle_param()->door_status)>>4);
	trunkDoorsStateInfo.set_trunk_state_exception((get_extend_vechicle_param()->door_exceprtion_status)>>4);
	trunkDoorsStateInfo.set_lock_on_off((get_extend_vechicle_param()->door_lock_status)>>4);
	trunkDoorsStateInfo.set_lock_state_exception((get_extend_vechicle_param()->door_lock_exception_status)>>4);
	Log(__FUNCTION__,"door status1 = %x %x %x %x",trunkDoorsStateInfo.trunk_on_off(),trunkDoorsStateInfo.trunk_state_exception(),trunkDoorsStateInfo.lock_on_off(),trunkDoorsStateInfo.lock_state_exception());
	ctrlTrunkDoorsMessage.set_allocated_trunk_doors_state_info(&trunkDoorsStateInfo);
	allStateMessage.set_allocated_trunk_doors_message(&ctrlTrunkDoorsMessage);

	CtrlElectromotorMessage electromotorMessage;
	ElectromotorStateInfo electromotorStateInfo;
	electromotorStateInfo.set_electromotor_on_off(get_extend_vechicle_param()->electrify_status);
	electromotorMessage.set_allocated_electromotor_state_info(&electromotorStateInfo);
	allStateMessage.set_allocated_electromotor_message(&electromotorMessage);

	CtrlEngineMessage ctrlEngineMessege;
	EngineStateInfo engineStateInfo;
	engineStateInfo.set_engine_start_state(get_extend_vechicle_param()->engine_status);
	ctrlEngineMessege.set_allocated_engine_state_info(&engineStateInfo);
	allStateMessage.set_allocated_engine_message(&ctrlEngineMessege);

	CtrlChargeMessage ctrlChargeMessege;
	ChargeStateInfo chargeStateInfo;
	chargeStateInfo.set_charge_on_off(get_extend_vechicle_param()->charge_status);
	ctrlChargeMessege.set_allocated_charge_state_info(&chargeStateInfo);
	allStateMessage.set_allocated_charge_message(&ctrlChargeMessege);

	CtrlWindowsMessage ctrlWindowsMessege;
	WindowsStateInfo *windowsStateInfo;
	for(int i=0;i<4;i++){
		windowsStateInfo = ctrlWindowsMessege.add_windows_state_info();
		windowsStateInfo->set_window_pos(WindowPos(i+1));
		windowsStateInfo->set_window_opened_value(get_extend_vechicle_param()->window_position[i]);
		windowsStateInfo->set_window_status(WindowStatus(get_extend_vechicle_param()->window_status[i]));
	}
	allStateMessage.set_allocated_windows_message(&ctrlWindowsMessege);

	CtrlTopWindowsMessage ctrlTopWindowsMessage;
	TopWindowsStateInfo topWindowsStateInfo;
	topWindowsStateInfo.set_sunroof_curtain(get_extend_vechicle_param()->sunroof_curtain_status);
	topWindowsStateInfo.set_sunroof_glass(get_extend_vechicle_param()->sunroof_glass_status);
	topWindowsStateInfo.set_sunroof_possition(get_extend_vechicle_param()->sunroof_glass_position);
	ctrlTopWindowsMessage.set_allocated_top_windows_state_info(&topWindowsStateInfo);
	allStateMessage.set_allocated_top_windows_message(&ctrlTopWindowsMessage);

	StateLightingMessage lightingMessage;
	lightingMessage.set_high_beam(get_extend_vechicle_param()->high_beam_status);
	lightingMessage.set_low_beam(get_extend_vechicle_param()->low_beam_status);
	lightingMessage.set_fog_light(get_extend_vechicle_param()->fog_light_status);
	lightingMessage.set_left_cornering_lamp(get_extend_vechicle_param()->left_cornering_lamp);
	lightingMessage.set_right_cornering_lamp(get_extend_vechicle_param()->right_cornering_lamp);
	allStateMessage.set_allocated_lighting_state(&lightingMessage);

	StateBatteryMessage batteryMessege;
	batteryMessege.set_soc(get_extend_vechicle_param()->soc);
	batteryMessege.set_avg(get_extend_vechicle_param()->averageConsumption);
	allStateMessage.set_allocated_battery_state(&batteryMessege);

	StateMileageMessage mileageMessege;
	mileageMessege.set_odo_mileage(get_extend_vechicle_param()->accumulatedMileage);
	mileageMessege.set_endurance_mileage(get_extend_vechicle_param()->rechargeMileage);
	allStateMessage.set_allocated_mileage_state(&mileageMessege);

	
	CtrlPrivateInformationMessage ctrlPrivateInformationMessage;
	PrivateInfoStateInfo privateInfoStateInfo;
	privateInfoStateInfo.set_lock_state(get_extend_vechicle_param()->control_panel_protection_status);
	ctrlPrivateInformationMessage.set_allocated_private_info_state_info(&privateInfoStateInfo);
	allStateMessage.set_allocated_private_information_message(&ctrlPrivateInformationMessage);

	CtrlCommonParamTbox ctrlCommonParamTbox,recvCommonParamTbox;
	recvCommonParamTbox = recvStateMessage.common_param();
	ctrlCommonParamTbox.set_request_id(recvCommonParamTbox.request_id());
	
	allStateMessage.set_allocated_common_param(&ctrlCommonParamTbox);
	allStateMessage.SerializeToString(&msg);
	offset = 0;
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
	date_pack(data,date,&offset);
	data[offset++] = 0xA0;
	memcpy(data+offset,msg.data(),msg.length());
	for(size_t i=0;i<msg.length();++i){
		Log_print("[%x] ",data[offset+i]);
	}
	Log(__FUNCTION__,"\n");
	offset += msg.length();
	Log_print(__FUNCTION__,"net ack");
	tcp_heart_send(sock,data,offset);
	
	ctrlAirTemperatureMessage.release_air_temperature_state_info();
	allStateMessage.release_air_temperature_message();

	ctrlAirMessage.release_air_state_info();
	allStateMessage.release_air_message();
	
	ctrlDoorsMessage.release_doors_state_info();
	allStateMessage.release_doors_message();

	ctrlTrunkDoorsMessage.release_trunk_doors_state_info();
	allStateMessage.release_trunk_doors_message();

	electromotorMessage.release_electromotor_state_info();
	allStateMessage.release_electromotor_message();

	ctrlEngineMessege.release_engine_state_info();
	allStateMessage.release_engine_message();

	ctrlChargeMessege.release_charge_state_info();
	allStateMessage.release_charge_message();

	ctrlWindowsMessege.clear_windows_state_info();
	allStateMessage.release_windows_message();

	
	ctrlTopWindowsMessage.release_top_windows_state_info();
	allStateMessage.release_top_windows_message();
	
	ctrlPrivateInformationMessage.release_private_info_state_info();
	allStateMessage.release_private_information_message();
	
	allStateMessage.release_lighting_state();

	allStateMessage.release_battery_state();

	allStateMessage.release_mileage_state();

	allStateMessage.release_common_param();
}

void ack_engine_start(){
	if(control_type == engine_start_type){
		std::string msg;
		int sock = sock_cli;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;
		CtrlEngineMessage ctrlEngineMessege;
		CtrlCommonInfo ctrlCommonInfo;
		CtrlCommonParamTbox ctrlCommonParamTbox;

		ctrlCommonInfo.set_valid(0x1);
		if(get_extend_vechicle_param()->engine_status == 0xff){
			ctrlCommonInfo.set_result(0x1);
			ctrlCommonInfo.set_state(0x1);
		}else{
			ctrlCommonInfo.set_result(0x0);
			int status = get_extend_vechicle_param()->engine_status;
			ctrlCommonInfo.set_state(status);
		}
		
		ctrlCommonParamTbox.set_request_id(engine_start_request_id);
		ctrlEngineMessege.set_allocated_engine_ctrl_info(&ctrlCommonInfo);
		ctrlEngineMessege.set_allocated_common_param(&ctrlCommonParamTbox);
		ctrlEngineMessege.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x85;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"engine net ack");
		tcp_heart_send(sock,data,offset);

		ctrlEngineMessege.release_common_param();
		ctrlEngineMessege.release_engine_ctrl_info();
		control_type = no_control_type;
	}
}
/*
void ack_charge_start(){
	if(control_type == charge_start_type){
		std::string msg;
		int sock = sock_cli;
		unsigned char *buf = statusBuf;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;

		
		CtrlChargeMessage ctrlChargeMessege;
		ctrlChargeMessege.set_state(get_extend_vechicle_param()->charge_status);
		ctrlChargeMessege.set_request_id(charge_start_request_id);
		ctrlChargeMessege.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x86;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"charge net ack");
		tcp_heart_send(sock,data,offset);
		
		control_type = no_control_type;	
	}
}
*/
void ack_remote_ctrl_enable(){
	if(control_type == remote_ctrl_enable_type){
		std::string msg;
		int sock = sock_cli;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;
		CtrlEkeyMessage ctrlEkeyMessege;
		CtrlCommonInfo ctrlCommonInfo;
		CtrlCommonParamTbox ctrlCommonParamTbox;

		ctrlCommonInfo.set_valid(0x1);
		ctrlCommonInfo.set_result(0x0);
		
		ctrlCommonParamTbox.set_request_id(remote_ctrl_enable_request_id);
		ctrlEkeyMessege.set_allocated_ekey_ctrl_info(&ctrlCommonInfo);
		ctrlEkeyMessege.set_allocated_common_param(&ctrlCommonParamTbox);
		ctrlEkeyMessege.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x87;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"remote enable net ack");
		tcp_heart_send(sock,data,offset);

		ctrlEkeyMessege.release_ekey_ctrl_info();
		ctrlEkeyMessege.release_common_param();
		control_type = no_control_type;	
	}
}

void ack_find_car(){
	if(control_type == find_car_type){
		std::string msg;
		int sock = sock_cli;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;
		CtrlLightHornMessage ctrlLightHornMessage;
		CtrlCommonInfo ctrlCommonInfo;
		CtrlCommonParamTbox ctrlCommonParamTbox;

		ctrlCommonInfo.set_valid(0x1);
		ctrlCommonInfo.set_result(0x0);
		
		ctrlCommonParamTbox.set_request_id(find_car_request_id);
		ctrlLightHornMessage.set_allocated_lighthorn_ctrl_info(&ctrlCommonInfo);
		ctrlLightHornMessage.set_allocated_common_param(&ctrlCommonParamTbox);
		ctrlLightHornMessage.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x82;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"remote enable net ack");
		tcp_heart_send(sock,data,offset);

		ctrlLightHornMessage.release_lighthorn_ctrl_info();
		ctrlLightHornMessage.release_common_param();
		control_type = no_control_type;	
	}	
}
/*
void ack_window_ctrl(){
	if(control_type == windows_ctrl_type){
		std::string msg;
		int sock = sock_cli;
		unsigned char *buf = statusBuf;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;
		CtrlWindowsMessage ctrlWindowsMessege;

		for(int i=0;i<4;++i){
			WindowsInfo *windowsInfos = ctrlWindowsMessege.add_windows_info();
			windowsInfos->set_window_type((WindowType)(i+1));
			windowsInfos->set_window_status((WindowStatus)((get_extend_vechicle_param()->window_status>>i) & 0x1));
		}
		TopWindowsInfo topWindowsInfo;
		topWindowsInfo.set_sunroof_glass_status(get_extend_vechicle_param()->sun_Glass_Status);
		ctrlWindowsMessege.set_allocated_top_wndows_info(&topWindowsInfo);
		ctrlWindowsMessege.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x88;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"electromotorMessage enable net ack");
		tcp_heart_send(sock,data,offset);
		
		control_type = no_control_type;
	}
}
*/

void ack_air_ctrl(){
	if(control_type == air_ctrl_type){
		std::string msg;
		int sock = sock_cli;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;
		CtrlAirMessage ctrlAirMessage;
		CtrlCommonInfo ctrlCommonInfo;
		CtrlCommonParamTbox ctrlCommonParamTbox;
		
		ctrlCommonInfo.set_valid(air_control_valide);
		ctrlCommonInfo.set_result(0);
		ctrlCommonParamTbox.set_request_id(air_ctrl_request_id);
		ctrlAirMessage.set_allocated_air_ctrl_info(&ctrlCommonInfo);
		ctrlAirMessage.set_allocated_common_param(&ctrlCommonParamTbox);
		ctrlAirMessage.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x89;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"air net ack");
		tcp_heart_send(sock,data,offset);

		ctrlAirMessage.release_air_ctrl_info();
		ctrlAirMessage.release_common_param();
		control_type = no_control_type;
	}	
}

void ack_air_temperture_ctrl(){
	if(control_type == air_temperature_ctr_type){
		std::string msg;
		int sock = sock_cli;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;
		CtrlAirTemperatureMessage ctrlAirTemperatureMessage;
		AirTemperatureCtrlInfo airTemperatureCtrlInfo;
		CtrlCommonParamTbox ctrlCommonParamTbox;
		
		airTemperatureCtrlInfo.set_set_temperature((get_air_conditon(ALL_VEHICLE)->temperature)*5);
		airTemperatureCtrlInfo.set_result(0);
		ctrlCommonParamTbox.set_request_id(air_temperature_ctrl_request_id);
		ctrlAirTemperatureMessage.set_allocated_air_temperature_ctrl_info(&airTemperatureCtrlInfo);
		ctrlAirTemperatureMessage.set_allocated_common_param(&ctrlCommonParamTbox);
		ctrlAirTemperatureMessage.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x8F;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"air net ack");
		tcp_heart_send(sock,data,offset);

		ctrlAirTemperatureMessage.release_air_temperature_ctrl_info();
		ctrlAirTemperatureMessage.release_common_param();
		control_type = no_control_type;
	}	
}

void ack_char_start(){
	
}

void ack_door_start(){
	if(control_type == door_lock_type){
		std::string msg;
		int sock = sock_cli;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;

		CtrlDoorsMessage ctrlDoorsMessage;
		CtrlCommonInfo ctrlCommonInfo;
		CtrlCommonParamTbox ctrlCommonParamTbox;

		ctrlCommonInfo.set_valid(door_control_valide);
		if(door_control_valide & get_extend_vechicle_param()->door_lock_exception_status){
			ctrlCommonInfo.set_result(door_control_valide);
		}else{
			ctrlCommonInfo.set_result(0x0);
		}

		ctrlCommonInfo.set_state(get_extend_vechicle_param()->door_lock_status&0x0f);	
		ctrlCommonParamTbox.set_request_id(door_lock_request_id);
		ctrlDoorsMessage.set_allocated_doors_ctrl_info(&ctrlCommonInfo);
		ctrlDoorsMessage.set_allocated_common_param(&ctrlCommonParamTbox);
		ctrlDoorsMessage.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x8B;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"door net ack");
		tcp_heart_send(sock,data,offset);

		ctrlDoorsMessage.release_doors_ctrl_info();
		ctrlDoorsMessage.release_common_param();
		control_type = no_control_type;
	}
}

void ack_truck_lock_ctrl(){
	if(control_type == truck_door_lock_type){
		std::string msg;
		int sock = sock_cli;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;

		CtrlTrunkDoorsMessage ctrlTrunkDoorsMessage;
		CtrlCommonInfo ctrlCommonInfo;
		CtrlCommonParamTbox ctrlCommonParamTbox;

		ctrlCommonInfo.set_valid(0x1);
		if(0x10 & get_extend_vechicle_param()->door_lock_exception_status){
			ctrlCommonInfo.set_result(0x1);
		}else{
			ctrlCommonInfo.set_result(0x0);
			if(0x10 & get_extend_vechicle_param()->door_lock_status){
				ctrlCommonInfo.set_state(0x1);
			}else{			
				ctrlCommonInfo.set_state(0x0);
			}
		}

		ctrlCommonParamTbox.set_request_id(truck_door_lock_request_id);
		ctrlTrunkDoorsMessage.set_allocated_trunk_doors_ctrl_info(&ctrlCommonInfo);
		ctrlTrunkDoorsMessage.set_allocated_common_param(&ctrlCommonParamTbox);
		ctrlTrunkDoorsMessage.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x90;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"door net ack");
		tcp_heart_send(sock,data,offset);

		ctrlTrunkDoorsMessage.release_common_param();
		ctrlTrunkDoorsMessage.release_trunk_doors_ctrl_info();
		control_type = no_control_type;
	}	
}
/*
void ack_panel_protecion_ctrl(){
	if(control_type == panel_protection_type){
		std::string msg;
		int sock = sock_cli;
		unsigned char *buf = statusBuf;
		date_time date;
		unsigned char data[1024] = {0};
		int offset = 0;

		CtrlPrivateInformationMessage ctrlPrivateInformationMessage;
		ctrlPrivateInformationMessage.set_on_off(get_extend_vechicle_param()->control_panel_protection_status);
		ctrlPrivateInformationMessage.set_operation(0x1);
		ctrlPrivateInformationMessage.set_state_exception(get_extend_vechicle_param()->control_panen_protection_exceprtion_status);

		ctrlPrivateInformationMessage.set_request_id(panel_protection_request_id);
		ctrlPrivateInformationMessage.SerializeToString(&msg);
		offset = 0;
		get_current_date(&date);
		head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
		date_pack(data,date,&offset);
		data[offset++] = 0x8D;
		memcpy(data+offset,msg.data(),msg.length());
		for(size_t i=0;i<msg.length();++i){
			Log_print("[%x] ",data[offset+i]);
		}
		Log_print("\n");
		offset += msg.length();
		Log_print(__FUNCTION__,"door net ack");
		tcp_heart_send(sock,data,offset);
		
		control_type = no_control_type;
	}
}
*/
int find_car(unsigned char *buf)
{
	int ret;
	
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	CtrlLightHornMessage ctrlLightHornMessege;
	CtrlCommonParamTbox ctrlCommonParamTbox;
	CtrlCommonInfo ctrlCommonInfo;
	int delayTime = 0;
	ret = ctrlLightHornMessege.ParseFromArray(buf+31,len-7);	
	Log(__FUNCTION__,"ret = %d\n",ret);
	if(ctrlLightHornMessege.has_common_param() && ctrlLightHornMessege.has_lighthorn_ctrl_info()){
		ctrlCommonParamTbox = ctrlLightHornMessege.common_param();
		ctrlCommonInfo = ctrlLightHornMessege.lighthorn_ctrl_info();
		if (ctrlCommonInfo.has_valid() && ctrlCommonInfo.has_state())
		{
			if(ctrlCommonInfo.valid() && ctrlCommonInfo.state())
			{
				find_car_cmd();	
			}
		}

		if(ctrlCommonParamTbox.has_request_id()){
			find_car_request_id = ctrlCommonParamTbox.request_id();
		}

		if(ctrlCommonParamTbox.has_delay_time()){
			delayTime = ctrlCommonParamTbox.delay_time();
		}
	}	
	
	control_type = find_car_type;
	return delayTime;
}

void ack_control()
{
	ack_engine_start();
	ack_door_start();
	ack_air_ctrl();
	ack_remote_ctrl_enable();
	ack_find_car();
	ack_truck_lock_ctrl();
	ack_air_temperture_ctrl();
}

int engine_start(unsigned char *buf)
{
	int ret;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	CtrlEngineMessage ctrlEngineMessege;
	CtrlCommonParamTbox ctrlCommonParamTbox;
	CtrlCommonInfo ctrlCommonInfo;
	int delayTime = 0;
	ret = ctrlEngineMessege.ParseFromArray(buf+31,len-7);	
	Log(__FUNCTION__,"ret = %d\n",ret);
	if(ctrlEngineMessege.has_common_param() && ctrlEngineMessege.has_engine_ctrl_info()){
		ctrlCommonParamTbox = ctrlEngineMessege.common_param();
		ctrlCommonInfo = ctrlEngineMessege.engine_ctrl_info();
		if (ctrlCommonInfo.has_valid() && ctrlCommonInfo.has_state())
		{
			engine_start_cmd(ctrlCommonInfo.state());	
		}

		if(ctrlCommonParamTbox.has_request_id()){
			engine_start_request_id = ctrlCommonParamTbox.request_id();
		}

		if(ctrlCommonParamTbox.has_delay_time()){
			delayTime = ctrlCommonParamTbox.delay_time();
		}
	}	
	
	control_type = engine_start_type;
	return delayTime;
}

/*
void charge_start(int sock,unsigned char *buf)
{
	std::string msg;
	date_time date;
	unsigned char data[1024] = {0};
	int offset = 0;
	bool ret;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];

	CtrlChargeMessage ctrlChargeMessege,ackCharegeMessege;
	ret = ctrlChargeMessege.ParseFromArray(buf+31,len-7);				
	Log(__FUNCTION__,"ret = %d\n",ret);
	if (ctrlChargeMessege.has_state())
	{
		chargeStatus = ctrlChargeMessege.state();
		
		Log(__FUNCTION__,"chare star = %x\n",chargeStatus);
		if(chargeStatus){
			charge_start_cmd(0x01);
		}
		else
		{
			charge_start_cmd(0x02);
		}
	}
	charge_start_request_id = ctrlChargeMessege.request_id();
	
	Log(__FUNCTION__,"chare star id = %d\n",charge_start_request_id);
	control_type = charge_start_type;

}
*/
int remote_ctrl_enable(unsigned char *buf)
{
	int ret;
	
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	CtrlEkeyMessage ctrlEkeyMessage;
	CtrlCommonParamTbox ctrlCommonParamTbox;
	CtrlCommonInfo ctrlCommonInfo;
	int delayTime = 0;
	ret = ctrlEkeyMessage.ParseFromArray(buf+31,len-7);	
	Log(__FUNCTION__,"remote ctrl enable ret = %d\n",ret);
	if(ctrlEkeyMessage.has_common_param() && ctrlEkeyMessage.has_ekey_ctrl_info()){		
		ctrlCommonParamTbox = ctrlEkeyMessage.common_param();
		ctrlCommonInfo = ctrlEkeyMessage.ekey_ctrl_info();
		
		if (ctrlCommonInfo.has_valid() && ctrlCommonInfo.has_state())
		{		
			Log(__FUNCTION__,"remote ctrl enable %d %d\n",ctrlCommonInfo.valid(),ctrlCommonInfo.state());
			if(ctrlCommonInfo.state()){
				remote_ctrl_enable_cmd(1);	
			}else{			
				remote_ctrl_enable_cmd(2);	
			}
		}

		if(ctrlCommonParamTbox.has_request_id()){
			remote_ctrl_enable_request_id = ctrlCommonParamTbox.request_id();
		}

		if(ctrlCommonParamTbox.has_delay_time()){
			delayTime = ctrlCommonParamTbox.delay_time();
		}
	}	
	
	control_type = remote_ctrl_enable_type;
	return delayTime;


}
/*
void windows_ctrl(int sock,unsigned char *buf)
{
	std::string msg;
	date_time date;
	unsigned char data[1024] = {0};
	int offset = 0;
	bool ret;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];

	CtrlWindowsMessage ctrlWindowsMessege,ackWindowsMessege;
	ret = ctrlWindowsMessege.ParseFromArray(buf+31,len-7);				
	Log(__FUNCTION__,"ret = %d\n",ret);
	if (ctrlWindowsMessege.windows_info_size() ==4)
	{
		WindowsInfo windowsInfos[4];
		for(int i=0;i<4;i++){
			windowsInfos[i] = ctrlWindowsMessege.windows_info(i);
		}
		if(windowsInfos[0].window_opened_value()==0 &&\
			windowsInfos[1].window_opened_value()==0 &&\
			windowsInfos[2].window_opened_value()==0 &&\
			windowsInfos[3].window_opened_value()==0 ){
				windows_ctrl_cmd(0xf,4);
		}
		if(windowsInfos[0].window_opened_value()==1 &&\
			windowsInfos[1].window_opened_value()==1 &&\
			windowsInfos[2].window_opened_value()==1 &&\
			windowsInfos[3].window_opened_value()==1){
				windows_ctrl_cmd(0xf,3);
		}
			
	}

	if(ctrlWindowsMessege.has_top_wndows_info()){
		TopWindowsInfo topWindowsInfo = ctrlWindowsMessege.top_wndows_info();
		if(topWindowsInfo.has_sunroof_glass_status()){
			
			Log(__FUNCTION__,"sunroof = %d",topWindowsInfo.sunroof_glass_status());
			if(topWindowsInfo.sunroof_glass_status()){
				windows_ctrl_cmd(0x10,1);
			}else{			
				windows_ctrl_cmd(0x10,2);
			}
		}
	}

	
	windows_ctrl_request_id = ctrlWindowsMessege.request_id();	
	control_type = windows_ctrl_type;

}
*/
int air_ctrl(unsigned char *buf)
{
	int ret,delayTime;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	CtrlAirMessage ctrlAirMessage;
	CtrlCommonParamTbox ctrlCommonParamTbox;
	CtrlCommonInfo ctrlCommonInfo;
	ret = ctrlAirMessage.ParseFromArray(buf+31,len - 7);
	Log_print(__FUNCTION__,"ret = %d\n",ret);
	
	if(ctrlAirMessage.has_common_param() && ctrlAirMessage.has_air_ctrl_info()){
		ctrlCommonParamTbox = ctrlAirMessage.common_param();
		ctrlCommonInfo = ctrlAirMessage.air_ctrl_info();

		if(ctrlCommonInfo.has_valid() && ctrlCommonInfo.has_state()){
			long long valid = ctrlCommonInfo.valid();
			long long state = ctrlCommonInfo.state();
		
			Log(__FUNCTION__,"air ctrl %d %d\n",ctrlCommonInfo.valid(),ctrlCommonInfo.state());

			if(valid & state & 0x1 ){
				air_a_key_defrost();
			}

			if(valid & state & 0x2 ){
				air_ac_max();
			}

			if(valid & state & 0x4){
				air_heat_max();
			}

			if(valid & state & 0x8){
				air_anion();
			}

			if(valid & state & 0x10){
				air_cabin_clean();
			}

			if(valid & state & 0x20){
				air_auto();
			}

			if(valid & 0x40){
				if(state & 0x40){
					air_purify_start();
				}else{			
					air_purify_stop();
				}
			}
			
			if(valid & 0x80){
				if(!(state & 0x80)){
					air_stop();
				}
			}
		}

		if(ctrlCommonParamTbox.has_request_id()){
			air_ctrl_request_id = ctrlCommonParamTbox.request_id();
		}

		if(ctrlCommonParamTbox.has_delay_time()){
			delayTime = ctrlCommonParamTbox.delay_time();
		}
	}
	
	control_type = air_ctrl_type;
	return delayTime;
}

int air_temperature_ctrl(unsigned char *buf){
	int ret,delayTime;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	CtrlAirTemperatureMessage ctrlAirTemperatureMessage;
	CtrlCommonParamTbox ctrlCommonParamTbox;
	AirTemperatureCtrlInfo airTemperatureCtrlInfo;
	ret = ctrlAirTemperatureMessage.ParseFromArray(buf+31,len - 7);
	Log_print(__FUNCTION__,"ret = %d\n",ret);
	
	if(ctrlAirTemperatureMessage.has_common_param() && ctrlAirTemperatureMessage.has_air_temperature_ctrl_info()){
		ctrlCommonParamTbox = ctrlAirTemperatureMessage.common_param();
		airTemperatureCtrlInfo = ctrlAirTemperatureMessage.air_temperature_ctrl_info();

		if(airTemperatureCtrlInfo.has_set_temperature()){
			Log(__FUNCTION__,"air temperature %d \n",airTemperatureCtrlInfo.set_temperature());
			if(airTemperatureCtrlInfo.set_temperature()!=0xffff){
				air_set_temperature(airTemperatureCtrlInfo.set_temperature()/5-36);
			}
		}

		if(ctrlCommonParamTbox.has_request_id()){
			air_temperature_ctrl_request_id = ctrlCommonParamTbox.request_id();
		}

		if(ctrlCommonParamTbox.has_delay_time()){
			delayTime = ctrlCommonParamTbox.delay_time();
		}
	}

	control_type = air_temperature_ctr_type;
	return delayTime;
}

/*
void char_ctrl(int sock,unsigned char *buf)
{
	std::string msg;
	date_time date;
	unsigned char data[1024] = {0};
	int offset = 0;
	bool ret;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];

	CtrlChairMessage ctrlChairMessege,ackChairMessege;
	ret = ctrlChairMessege.ParseFromArray(buf+31,len-7);				
	Log_print(__FUNCTION__,"ret = %d\n",ret);
	if (ctrlChairMessege.has_on_off() && ctrlChairMessege.has_operation())
	{
		chairOperature = ctrlChairMessege.operation();
		chairOnOff = ctrlChairMessege.on_off();
		
		Log_print(__FUNCTION__,"chair= %d %d\n",chairOperature,chairOnOff);
		chair_ctrl_cmd(chairOnOff,chargeStatus);
	}
	char_ctrl_request_id = ctrlChairMessege.request_id();	
	control_type = char_ctrl_type;
}

void electromotor_ctrl(int sock,unsigned char *buf){
	std::string msg;
	date_time date;
	unsigned char data[1024] = {0};
	int offset = 0;
	bool ret;
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];

	CtrlElectromotorMessage ctrlElectromotorMessage;
	ret = ctrlElectromotorMessage.ParseFromArray(buf+31,len-7);				
	Log_print(__FUNCTION__,"ret = %d\n",ret);
	if (ctrlElectromotorMessage.has_state())
	{
		electromotorStatus = ctrlElectromotorMessage.state();	
		Log_print(__FUNCTION__,"electromotorStatus= %d \n",electromotorStatus);
		electromotor_ctrl_cmd(electromotorStatus);
	}
	electromotor_ctrl_request_id = ctrlElectromotorMessage.request_id();
	
	control_type = electromotor_type;	
}
*/
int door_lock_ctrl(unsigned char *buf){
	int ret;
	
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	CtrlDoorsMessage ctrlDoorsMessege;
	CtrlCommonParamTbox ctrlCommonParamTbox;
	CtrlCommonInfo ctrlCommonInfo;
	int delayTime = 0;
	ret = ctrlDoorsMessege.ParseFromArray(buf+31,len-7);		
	Log(__FUNCTION__,"ret = %d\n",ret);
	if (ctrlDoorsMessege.has_common_param() && ctrlDoorsMessege.has_doors_ctrl_info())
	{
		ctrlCommonParamTbox = ctrlDoorsMessege.common_param();
		ctrlCommonInfo = ctrlDoorsMessege.doors_ctrl_info();

		if(ctrlCommonInfo.has_valid() && ctrlCommonInfo.has_state()){
			if(ctrlCommonInfo.valid() == 0xf && ctrlCommonInfo.state() == 0xf){			
				doors_ctrl_cmd(0x0f,0x01);
			}else{
				doors_ctrl_cmd(0x0f,0x02);
			}

			door_control_valide = ctrlCommonInfo.valid();
		}
	
		if(ctrlCommonParamTbox.has_request_id()){
			door_lock_request_id = ctrlCommonParamTbox.request_id();
		}

		if(ctrlCommonParamTbox.has_delay_time()){
			delayTime = ctrlCommonParamTbox.delay_time();
		}
	}

	control_type = door_lock_type;
	return delayTime;
}

int trunk_lock_ctrl(unsigned char *buf){
	int ret,delayTime;
	
	int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];	
	CtrlTrunkDoorsMessage ctrlTrunkDoorsMessage;
	CtrlCommonParamTbox ctrlCommonParamTbox;
	CtrlCommonInfo ctrlCommonInfo;
	ret = ctrlTrunkDoorsMessage.ParseFromArray(buf+31,len-7);
	Log(__FUNCTION__,"ret = %d\n",ret);
	if (ctrlTrunkDoorsMessage.has_common_param() && ctrlTrunkDoorsMessage.has_trunk_doors_ctrl_info()){
		ctrlCommonParamTbox = ctrlTrunkDoorsMessage.common_param();
		ctrlCommonInfo = ctrlTrunkDoorsMessage.trunk_doors_ctrl_info();

		if(ctrlCommonInfo.valid()&& ctrlCommonInfo.state()){
			doors_ctrl_cmd(0x10,0x03);
		}
		
		if(ctrlCommonParamTbox.has_request_id()){
			truck_door_lock_request_id = ctrlCommonParamTbox.request_id();
		}

		if(ctrlCommonParamTbox.has_delay_time()){
			delayTime = ctrlCommonParamTbox.delay_time();
		}	
	}

	control_type = truck_door_lock_type;
	return delayTime;
}

void *wait_ack_control(void *paramTemp){
	int *delayTime = (int *)paramTemp;
	sleep(*delayTime);	
	Log(__FUNCTION__,"ack wait %d\n",control_type);
	ack_control();

	return (void*)0;
}


static void deal_control(int sock,unsigned char *buf)
{
	std::string msg;
	date_time date;
	unsigned char data[1024] = {0};
	int offset = 0;
	int delayTime = 0;
	//bool ret;
	//struct mcu_msg_stru msg;
	//int len = (buf[DATA_LENGTH_ADDR]<<8)+buf[DATA_LENGTH_ADDR + 1];
	real_wakeup = true;
	switch(buf[CHECK_CMD_NUM_ADDR])
	{
		case 0x01://update
			deal_upgrade(sock,buf);	
			break;
		case 0x02:	
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
			get_current_date(&date);
			head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
			date_pack(data,date,&offset);
			data[offset++] = buf[CHECK_CMD_NUM_ADDR];
			tcp_heart_send(sock,data,offset);	
			break;
		case 0x07://start platform link
			if(!platform_link_on)
			{				
				platform_link_on = true;	
			}
			get_current_date(&date);
			head_pack(data,&offset,DATA_CMD_CONTROL,DATA_SUCCESS,NO_ENCRYPT);
			date_pack(data,date,&offset);
			data[offset++] = buf[CHECK_CMD_NUM_ADDR];
			tcp_heart_send(sock,data,offset);	
			break;		
		case 0x82://寻车
			delayTime = find_car(buf);
			break;
		case 0x85://发动机远程启动-停止
			delayTime = engine_start(buf);
			break;
		case 0x86://充电启动-停止
			//charge_start(sock,buf);
			break;
		case 0x87://远程控制使能
			delayTime = remote_ctrl_enable(buf);
			break;
		case 0x88://车窗控制
			//windows_ctrl(sock,buf);
			break;
		case 0x89://空调控制
			delayTime = air_ctrl(buf);
			break;
		case 0x8A:
			//electromotor_ctrl(sock,buf);
			break;
		case 0x8B:
			delayTime = door_lock_ctrl(buf);
			break;
		case 0x8C://座椅控制
			//char_ctrl(sock,buf);
			break;
		case 0x8D:
			//panel_protection_ctrl(sock,buf);
			break;
		case 0xA0://全部状态查询
			get_all_status();
			break;
		case 0x8F:
			delayTime = air_temperature_ctrl(buf);
			break;
		case 0x90://全部状态查询
			delayTime = trunk_lock_ctrl(buf);
			break;
	}

	if(buf[CHECK_CMD_NUM_ADDR]!=06 || buf[CHECK_CMD_NUM_ADDR]!=07 ||buf[CHECK_CMD_NUM_ADDR]!=0xa0){
		pthread_t control_ack_t;
		int ret = pthread_create(&control_ack_t,NULL,wait_ack_control,(void*)&delayTime);
		if(ret != 0)
		{
			Log(__FUNCTION__,"ack control thread failed!\n");
		}
	}
}

static void deal_cmd(unsigned char *buf,int clt_sock)
{
	char recv_date[20] = {0};

	if(buf[0] == 0x23 && buf[1] == 0x23)
	{
		Log_print(__FUNCTION__,"cmd = %X\n",buf[CMD_FLAG_ADDR]);
		if((buf[CMD_FLAG_ADDR] == DATA_CAR_MSG_SEND || buf[CMD_FLAG_ADDR] == DATA_CAR_MSG_REP) 
			&& buf[ACK_FLAG_ADDR] == DATA_ERROR)
		{
			sprintf((char*)recv_date,"%02d-%02d-%02d %02d:%02d:%02d",
					buf[24],buf[25],buf[26],buf[27],buf[28],buf[29]);
			Log_print(__FUNCTION__,"rcv date = %s",recv_date);
			//sql_mark_send_failed(recv_date);
		}
		else if(buf[CMD_FLAG_ADDR] == DATA_CMD_HEART)
		{
			heart_beat_ack = true;
		}
		else if(buf[CMD_FLAG_ADDR] == DATA_CMD_TIME)
		{
			char set_date[48] = {0};
			sprintf(set_date,"date -s \"20%02d-%02d-%02d %02d:%02d:%02d\"",
				    buf[24],buf[25],buf[26],buf[27],buf[28],buf[29]);
			system((const char*)set_date);
		}
		else if(buf[CMD_FLAG_ADDR] == DATA_CMD_CHECK)
		{
			deal_net_check(clt_sock);
		}
		else if(buf[CMD_FLAG_ADDR] == DATA_CMD_SET)
		{
			deal_net_set(clt_sock,buf);
		}
		else if(buf[CMD_FLAG_ADDR] == DATA_GET_VERSION)
		{
			deal_get_version(clt_sock,buf);
		}
		else if(buf[CMD_FLAG_ADDR] == DATA_CMD_CONTROL)
		{	
			real_wakeup = true;
			deal_control(clt_sock,buf);
		}
	}
}


bool tcp_send(int sock,unsigned char* buf,int len)
{
	unsigned char send_buf[1024] = {0};
	int i,ret;
	memcpy(send_buf,buf,len);

	send_buf[len] = check_sum(send_buf,len);
//	Log(__FUNCTION__,"len = %d\n",len);
	pthread_mutex_lock(&tcp_mutex);
    if(param.decrypt)
    {
    	ret = SSL_write(ssl,send_buf,len+1);
    }
	else
	{
		ret = send(sock, send_buf,len + 1,MSG_NOSIGNAL);
	}

	if(-1 == ret)
	{
		pthread_mutex_unlock(&tcp_mutex);	
		perror("write failed");
		return false;
	}
	else
	{
		pthread_mutex_unlock(&tcp_mutex);	
		Log_print(__FUNCTION__,"tcp send:");
		for(i = 0;i < len;i++)
		{
			Log_print("%X ",buf[i]&0xFF);
			if((i+1)%10 == 0)
			{
				Log_print("\n");
			}
		}
		Log_print("\n");
		return true;
	}
}

bool tcp_heart_send(int sock,unsigned char* buf,int len)
{
	if(!connect_ok)
	{
		return false;
	}
	int i,ret;
#if 1
	for(i = 0;i < len;i++)
	{
		Log_print("%X ",buf[i]);
		if((i+1 % 10) == 0)
		{
			Log_print("\n");
		}
	}	
	Log_print("\n");
#endif
	end_pack(buf,len);
	unsigned char send_buf[1024] = {0};
	memcpy(send_buf,buf,len);

	send_buf[len] = check_sum(send_buf,len);
#if 0
	for(i = 0;i < len+1;i++)
	{
		Log_print("%X ",send_buf[i]);
		if((i+1 % 10) == 0)
		{
			Log_print("\n");
		}
	}
#endif
	pthread_mutex_lock(&tcp_mutex);
    if(param.decrypt == 1)
    {
    	ret = SSL_write(ssl,send_buf,len+1);
    }
	else
	{
		ret = send(sock, send_buf,len + 1,MSG_NOSIGNAL);
	}

	if(-1 == ret)
	{
		pthread_mutex_unlock(&tcp_mutex);	
		perror("write failed");
		return false;
	}
	else
	{
		pthread_mutex_unlock(&tcp_mutex);	
		Log_print(__FUNCTION__,"tcp heart send:");
		return true;
	}
}



bool connect_to_server(int* clt_sock,char* url)
{
	Log(__FUNCTION__,"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    *clt_sock = socket(AF_INET, SOCK_STREAM, 0);   
	
    if(*clt_sock < 0)  
    {    
        Log(__FUNCTION__,"socket failed\n");  
		close(*clt_sock);
		sleep(1);
        return false;  
    }    
    struct sockaddr_in addr;   
    addr.sin_family = AF_INET;   
    addr.sin_port = htons(param.port);   
	
	if(inet_addr(url) == INADDR_NONE)
	{
		struct hostent *nlp_host;
		
		if((nlp_host=gethostbyname(url))==0)
		{
			Log(__FUNCTION__,"Resolve Error!\n");
			return false;
		}
		
		addr.sin_addr.s_addr=((struct in_addr *)(nlp_host->h_addr))->s_addr;
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(url);
	}

    socklen_t addr_len = sizeof(addr);  
	
	Log(__FUNCTION__,"connectting\n");  
    int connect_fd = connect(*clt_sock, (struct sockaddr*)&addr, addr_len);  
    if(connect_fd < 0)  
    {  
        Log(__FUNCTION__,"connect error\n");  
		close(*clt_sock);
        return false;  
    }
	Log(__FUNCTION__,"connect to server success\n");
	return true;
}

int ssl_read_long(SSL *ssl,unsigned char *recvbuf)
{
	int recvTemp,recvLen=0;
	recvTemp = SSL_read(ssl,recvbuf,24);
	if(recvTemp == -1){
		return -1;
	}else{
		recvLen += recvTemp;
		int needRecv = recvbuf[22];  
		needRecv <<= 8;
		needRecv += recvbuf[23];
		needRecv +=1; //crc
		recvTemp = SSL_read(ssl,recvbuf+24,needRecv);
		if(recvTemp == -1){
			return -1;
		}else{
			return recvTemp+recvLen;
		}
	}
}

int login(int clt_sock,short serial_num)
{
	unsigned char data[512] = {0};
	unsigned char rcv_buf[512] = {0};
	int offset;
	date_time date;
	get_current_date(&date);

	head_pack(data,&offset,DATA_CAR_LOGIN,DATA_CMD,NO_ENCRYPT);
	date_pack(data,date,&offset);
	login_pack(data,serial_num,&offset);
	end_pack(data,offset);
	
	tcp_send(clt_sock,data,offset);
	int recvd;
	if(param.decrypt == 1)
	{
		//recvd = SSL_read(ssl,rcv_buf,1024);  
		recvd = ssl_read_long(ssl, rcv_buf);
	}
	else
	{
		recvd = recv(clt_sock,rcv_buf,1024,0);
	}
	
    if(recvd==-1&&errno==EAGAIN)
    {
        Log(__FUNCTION__,"timeout\n");
		connect_ok = false;
		return -1;
    }
	else
	{
		Log(__FUNCTION__,"authentication_user\n");
		int i,len;
		len = str_to_i(rcv_buf+22,2);
		Log(__FUNCTION__,"len = %d\n",len);

		for(i = 1;i < recvd + 1;i++)
		{
			Log_print("%X ",rcv_buf[i - 1]);
			if((i % 10) == 0)
			{
				Log_print("\n");
			}
		}
		Log_print("\n");
		if(rcv_buf[0] == 0x23 && rcv_buf[1] == 0x23 && rcv_buf[2] == 0x01 && rcv_buf[3] == 0x01)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int logout(int clt_sock,short serial_num)
{
	Log(__FUNCTION__,"******************");
	unsigned char data[512] = {0};
	int offset;
	date_time date;
	get_current_date(&date);

	head_pack(data,&offset,DATA_CAR_LOGOUT,DATA_CMD,NO_ENCRYPT);
	date_pack(data,date,&offset);

	data[offset++] = (serial_num >> 8) & 0xFF;
	data[offset++] = serial_num & 0xFF;

	end_pack(data,offset);
	
	tcp_send(clt_sock,data,offset);
	return 0;
}

void check_time(int clt_sock)
{
	unsigned char data[512] = {0};
	int offset;
	date_time date;
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_TIME,DATA_CMD,NO_ENCRYPT);
	end_pack(data,offset);	
	tcp_send(clt_sock,data,offset);
}

void collision_alert()
{
	unsigned char data[512] = {0};
	int offset;
	date_time date;
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_CONTROL,DATA_CMD,NO_ENCRYPT);	
	date_pack(data,date,&offset);
	data[offset++] = 0xA2;
	end_pack(data,offset);	
	tcp_send(sock_cli,data,offset);
}

void lilegal_encroachment()
{
	unsigned char data[512] = {0};
	int offset;
	date_time date;
	get_current_date(&date);
	head_pack(data,&offset,DATA_CMD_CONTROL,DATA_CMD,NO_ENCRYPT);	
	date_pack(data,date,&offset);
	data[offset++] = 0xA3;
	end_pack(data,offset);	
	tcp_heart_send(sock_cli,data,offset);
}

//------------------------------------------------------------------------------
/*
 *  Receive data from socket, and output data through stdout device
 */
//------------------------------------------------------------------------------
void SocketRecv_Proc(int socketFd)
{
    unsigned char recvbuf[BUFFER_SIZE];
	short empty_time = 0;
	int timeout = param.tcp_rcv_timeout;

    while (!network_thread_cancel)
    {
		memset(recvbuf, 0, sizeof(recvbuf));  
		int recvd;
		if(param.decrypt == 1)
		{
			//recvd = SSL_read(ssl,recvbuf,1024);  			
			recvd = ssl_read_long(ssl, recvbuf);
		}
		else
		{
			recvd = recv(socketFd,recvbuf,1024,0);
		}

		
		//Log(__FUNCTION__,"recvd = %d  timeout = %d",recvd,timeout);
		if(recvd == 0){
			connect_ok = false;
			//module_init_thread_restart = true;		
			Log(__FUNCTION__,"socket be closed\n");
			break;
		}
	
		if(recvd==-1&&errno==EAGAIN)
		{
			if(timeout-- <= 0)
			{
				Log(__FUNCTION__,"timeout\n");
				connect_ok = false;
				//module_init_thread_restart = true;
				break;
			}
		}
		else
		{
			if((recvbuf[0] == 0)&&(recvbuf[1] == 0)&&(recvbuf[2] == 0))
			{
				if(empty_time++ > 100)
				{
					break;
				}
			}
			else
			{
				empty_time = 0;
			}
			int i;
			timeout = param.tcp_rcv_timeout;
			if(recvbuf[recvd - 1] == check_sum(recvbuf,recvd - 1))
			{
				Log_print(__FUNCTION__,"tcp rcv:");
				for(i = 1;i < recvd + 1;i++)
				{
					Log_print("%X ",recvbuf[i - 1]);
					if((i % 10) == 0)
					{
						Log_print("\n");
					}
				}
				Log_print("\n");
				deal_cmd(recvbuf,socketFd);
			}
			else
			{
				Log(__FUNCTION__,"check sum failed");
			}

		}

    }
}

void* TCP_Program(void* tcp_param)
{    
	sleep(1);

	if(tcp_param == NULL)
	{
		return 0;
	}
	network_thread_cancel = false;
	while(!network_thread_cancel)
	{
		int connect_timeout = 0;
		int step = connect_step;
		int ret;
		while((step != finish_step)&&(!network_thread_cancel))
		{
			if(!connected)
			{
				sleep(5);
				step = connect_step;
				if(connect_timeout++ < 60)
				{
					Log(__FUNCTION__,"network not ready");
					sleep(1);			
				}
				else
				{
					system("reboot");
				}
				continue;
			}
			switch(step)
			{
			case connect_step:
				if(connect_to_server(&sock_cli,(char*)tcp_param))
				{
					struct timeval timeout={2,0};//2s
					setsockopt(sock_cli,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
					setsockopt(sock_cli,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));	
					if(param.decrypt == 1)
					{
						step = ssl_init_step;//check_rsa_step;	
					}
					else
					{
						step = auth_step;
					}
					connect_timeout = 0;
				}
				else
				{
					Log(__FUNCTION__,"connect to server failed\n");
					sleep(1);
				}
				break;
			case ssl_init_step:
				ret = ssl_init(sock_cli);
				if(ret == true)
				{
					step = auth_step;
				}
				else
				{
					close(sock_cli);
					step = connect_step;
					sleep(5);
				}
				break;
			case auth_step:
				//authentication  
				ret = login(sock_cli,param.login_serial_num);
				if(ret == 1)
				{
					step = finish_step;
					connect_timeout = 0;
					device_check_param.tsp_check_status =2;
					
					Log(__FUNCTION__,"authentication success\n");
				}
				else if(ret == 0)
				{
					Log(__FUNCTION__,"authentication failed\n");
					if(connect_timeout++ > 3)
					{
						step = connect_step;		
					}
					sleep(1);
				}				
				else if(ret == -1)
				{
					Log(__FUNCTION__,"timeout");
					step = connect_step;
					connect_timeout = 0;
				}
				break;
			}
		}
		connect_ok = true;
		check_time(sock_cli);

		get_upgrade();
		if(upgrade_param.pc_upgrade_status){
			int ret = down_load_verify((char*)upgrade_param.pc_upgrade_sign,(char*)upgrade_param.pc_upgrade_path);
			if(ret){			
				if(mcu_upgrade == false)
				{
					PC_Power_cmd(PC_UPGRADE);
					Ql_SLP_WakeLock_Destroy(wakelock_fd);
					upgrade_param.pc_upgrade_status = 0;				
					init_save_upgrade();
					addShellUpdateNum(backupShellType);
					system("sync");
					system("/home/root/update.sh &");
				}
			}		
		}
#if 0

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
#endif		
	    struct timeval heart_timeout={1,0};//30s
		setsockopt(sock_cli,SOL_SOCKET,SO_RCVTIMEO,(const char*)&heart_timeout,sizeof(heart_timeout));
		setsockopt(sock_cli,SOL_SOCKET,SO_SNDTIMEO,(const char*)&heart_timeout,sizeof(heart_timeout));
	    SocketRecv_Proc(sock_cli);
		
		Log(__FUNCTION__,"network error outpu recv proc\n");
		connect_ok = false;
		//logout(sock_cli,param.login_serial_num);
		param.login_serial_num++;	
		init_save_param(PARAM_PATH);
		init_save_param(BAK_PARAM_PATH);
		close(sock_cli);
		if(param.decrypt == 1)
		{
			ssl_close();
		}
		if((param.GB32690_type != e_off) && (param.GB32690_type != e_normal))
		{
			sleep(param.offline_time);
			param.GB32690_type = e_normal;
		}
	}     
	return 0;
}


unsigned char* UnicodeToUTF8( int unicode, unsigned char *p)
{
    unsigned char *e = NULL;
    if((e = p))
    {
        if(unicode < 0x80)
        {
            *e++ = unicode;
        }
        else if(unicode < 0x800)
        {
            *e++ = ((unicode >> 6) & 0x1f)|0xc0;
            *e++ = (unicode & 0x3f)|0x80;
        }
        else if(unicode < 0x10000)
        {
            *e++ = ((unicode >> 12) & 0x0f)|0xe0;
            *e++ = ((unicode >> 6) & 0x3f)|0x80;
            *e++ = (unicode & 0x3f)|0x80;
        }
        else if(unicode < 0x200000)
        {
            *e++ = ((unicode >> 18) & 0x07)|0xf0;
            *e++ = ((unicode >> 12) & 0x3f)|0x80;
            *e++ = ((unicode >> 6) & 0x3f)|0x80;
            *e++ = (unicode & 0x3f)|0x80;
        }
        else if(unicode < 0x4000000)
        {
            *e++ = ((unicode >> 24) & 0x03)|0xf8 ;
            *e++ = ((unicode >> 18) & 0x3f)|0x80;
            *e++ = ((unicode >> 12) & 0x3f)|0x80;
            *e++ = ((unicode >> 6) & 0x3f)|0x80;
            *e++ = (unicode & 0x3f)|0x80;
        }
        else
        {
            *e++ = ((unicode >> 30) & 0x01)|0xfc;
            *e++ = ((unicode >> 24) & 0x3f)|0x80;
            *e++ = ((unicode >> 18) & 0x3f)|0x80;
            *e++ = ((unicode >> 12) & 0x3f)|0x80;
            *e++ = ((unicode >> 6) & 0x3f)|0x80;
            *e++ = (unicode & 0x3f)|0x80;
        }
    }

    return e;
}


unsigned char* UnicodeStrToUTF8Str (unsigned short *unicode_str,
                                    unsigned char   *utf8_str,
                                    int             utf8_str_size)
{
    int unicode = 0;
    unsigned char *e = NULL, *s = NULL;
    unsigned char utf8_ch[MAX_CHARACTER_SIZE];
    s = utf8_str;
    if ((unicode_str) && (s))
    {
        while ((unicode = (int) (*unicode_str++)))
        {
            #if 1//LE
                unicode = ((unicode&0xFF)<<8) | ((unicode&0xFF00) >> 8);  //error
            #else//BE
                unicode = unicode;  
            #endif
            memset (utf8_ch, 0, sizeof (utf8_ch));

            if ((e = UnicodeToUTF8 (unicode, utf8_ch)) > utf8_ch)
            {
                *e = '\0';

                if ((s - utf8_str + strlen ((const char *) utf8_ch)) >= (unsigned int)utf8_str_size)
                {
                    return s;
                }
                else
                {
                    memcpy (s, utf8_ch, strlen ((const char *) utf8_ch));
                    s += strlen ((const char *) utf8_ch);
                    *s = '\0';
                }
            }
            else
            {
                return s;
            }
        }
    }

    return s;
}

static void ql_sms_cb_func( QL_SMS_MsgRef   msgRef,
                            void*               contextPtr)
{
	real_wakeup = true;
    int i;
    if(msgRef->e_storage != E_QL_SMS_STORAGE_TYPE_NONE)
    {
        const char *msg_format[]  = {"CDMA",  "GW"};
        const char *storage_type[]= {"UIM",   "NV"};
        QL_USER_LOG("###You've got one new %s message, stored to %s index=%d\n",
                    msg_format[msgRef->e_mode & 1],
                    storage_type[msgRef->e_storage & 1],
                    msgRef->storage_index);
    }
    else if(msgRef->format == E_QL_SMS_FORMAT_UCS2)
    {
        unsigned char smsbuf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};
        UnicodeStrToUTF8Str((unsigned short*)(&msgRef->sms_data[0]),
                              smsbuf,
                              QL_SMS_MAX_MT_MSG_LENGTH) ;

        QL_USER_LOG("\n###You've got one new UCS2 msg from %s at %s, len=%d, content=%s\n",
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data_len,
                    smsbuf);
        Log(__FUNCTION__,"Received UCS raw data:");
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            Log(__FUNCTION__,"%.2X ", msgRef->sms_data[i]);
        }

        Log(__FUNCTION__,"\nAfter convert to UTF8, len=%d, data:", strlen((const char*)smsbuf));
        for(i=0; (unsigned int)i<strlen((const char*)smsbuf); i++)
        {
            Log_print("%X ", smsbuf[i]);
        }
        Log_print("\n");
    }
    else if(msgRef->format == E_QL_SMS_FORMAT_BINARY_DATA)
    {
        QL_USER_LOG("###You've got one new BINARY msg from %s at %s , len=%d, content=",
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data_len);
		for(i=0; i<msgRef->sms_data_len; i++)
		{
			Log_print("%X ", msgRef->sms_data[i]);
		}
		Log_print("\n");
	}
	else //default is GSM-7
	{
		QL_USER_LOG("###You've got one new GSM-7 msg from %s at %s content=%s\n",
					msgRef->src_addr,
					msgRef->timestamp,
					msgRef->sms_data);
		for(i=0; i<msgRef->sms_data_len; i++)
		{
			Log_print("%X ", msgRef->sms_data[i]);
		}
		Log_print("\n\n");
	}
}

#define BUF_SIZE 128
void ql_qcmapclient_nw_status_cb(ql_wwan_nw_staus_e_type status)
{
	char ICCID[BUF_SIZE] = {0};
	char IMSI[BUF_SIZE] = {0};
	char IMEI[BUF_SIZE] = {0};
    switch (status) 
	{
    case QL_WWAN_NW_CONNECTING:
		if(param.GB32690_type <= 0)
		{
			QL_SIM_GetICCID(E_QL_SIM_EXTERNAL_SLOT_1,ICCID, BUF_SIZE);     
			Log(__FUNCTION__,"ICCID: %s\n",ICCID);	
			if(strcmp(ICCID,param.ICCID))
			{
				memcpy(param.ICCID,ICCID,sizeof(param.ICCID)-1);
				init_save_param(PARAM_PATH);
				init_save_param(BAK_PARAM_PATH);
			}
			/****************
			if(!strcmp("00000000000000000",param.VIN)){
				memcpy(param.VIN,&ICCID[3],17);			
				Log(__FUNCTION__,"VIN: %s\n",param.VIN);
				init_save_param(PARAM_PATH);
				init_save_param(BAK_PARAM_PATH);
			}
			***************/			
			QL_SIM_GetIMSI(E_QL_SIM_EXTERNAL_SLOT_1,IMSI, BUF_SIZE);
			Log(__FUNCTION__,"IMSI: %s\n",IMSI);
			if(strcmp(IMSI,param.IMSI))
			{
				memcpy(param.IMSI,IMSI,sizeof(param.IMSI)-1);
				init_save_param(PARAM_PATH);
				init_save_param(BAK_PARAM_PATH);
			}
			QL_DEV_GetImei(IMEI, BUF_SIZE);
			Log(__FUNCTION__,"IMEI: %s\n",IMEI);
			if(strcmp(IMEI,param.IMEI))
			{
				memcpy(param.IMEI,IMEI,sizeof(param.IMEI)-1);
				init_save_param(PARAM_PATH);
				init_save_param(BAK_PARAM_PATH);
			}
		}
		else
		{
			Log(__FUNCTION__,"ICCID: %s\n",param.ICCID);
		}

        Log(__FUNCTION__,"APPS connecting wwan...\n\r");
        break;

    case QL_WWAN_NW_CONNECTED:
		Log(__FUNCTION__,"APPS connect wwan done\n\r"); 
        connected = TRUE;
        break;
    case QL_WWAN_NW_CONNECTING_TIMEOUT:
        Log(__FUNCTION__,"APPS connect wwan timeout\n\r");
    case QL_WWAN_NW_DISCONNECTED:
        Log(__FUNCTION__,"APPS disconnect wwan\n\r");
        connected = FALSE;
        break;
    default:
        Log(__FUNCTION__,"APPS connect wwan unkonwn evt\n\r");
        break;
    }

    return;
}

void* module_init_thread(void* module_param)
{
	int ret;
	static sms_client_handle_type	   h_sms;
	ret = QL_SMS_Client_Init(&h_sms);
	Log(__FUNCTION__,"QL_SMS_Client_Init ret=%d with h_sms=0x%x\n", ret, h_sms);
	ret = QL_SMS_AddRxMsgHandler(ql_sms_cb_func, (void*)h_sms);
	Log(__FUNCTION__,"QL_SMS_AddRxMsgHandler ret=%d \n", ret);
	ql_wifi_enable();
	while(1)
	{
	    Ql_NW_MobileAP_Init();
	    Ql_NW_MoblieAP_AutoConnect_Set(TRUE);

	    Ql_NW_MobileAP_Enable(10, ql_qcmapclient_nw_status_cb);
		module_init_thread_restart = false;
	    while (!module_init_thread_restart) 
		{
	        sleep(1);
	    }
	    Ql_NW_MobileAP_Disable();
	}
	
}


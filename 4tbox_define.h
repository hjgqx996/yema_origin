#ifndef __4TBOX_DEFINE
#define __4TBOX_DEFINE
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
#include <sys/msg.h>
#include <unistd.h>
#include "ql_gpio.h"
#include "ql_sleep_wakeup.h"
#include "ql_tts_client_api.h"

#define CERT_PATH "/home/root/client-cert.crt"//
#define KEY_PATH  "/home/root/client-key.pem"//
#define TEMP_KEY_PATH "/home/root/temp-key.pem"
#define CAFILE "/home/root/ca-cert.crt"//
#define EN_PATH	"/home/root/en.pem"

#define SAVE_LOG
#define PRINT_LOG

#define SQL_OPEN

#define SLEEP_HEART_TIME	3// min –›√ﬂ«Èøˆœ¬ºÏ≤‚Õ¯¬Á–ƒÃ¯ ±º‰
#define SWITCH_MSG_WAKEUP_MOD	160 // º∏¥ŒŒﬁÕ¯¬ÁªΩ–—‘Ú«–ªªµΩ∂Ã–≈ªΩ–—

#define TBOX_TYPE_ID				0x03

#define MAX_FIL_LOOP			3
//#define ENCRYPT

#define SQL_FILE_PATH 	"/media/sdcard/record.db"
#define SQL_PLAT_FILE_PATH "/media/sdcard/plat_record.db"


#define _AT_PORT	"/dev/smd8"
#define _NMEA_PORT	"/dev/smd7"
#define SAVE_MAX_DAY		7

#define UPGRADE_CMD			'4'
#define CHECK_VERSION_CMD	'5'

#define RSA_FILE_NAME	"/home/root/RSA_pub.key"

#define AES_BLOCK_SIZE	16


#define GPIO_MUTE		PINNAME_GPIO1
#define GPIO_GPS_PWR	PINNAME_GPIO2
#define GPIO_BIG_VOICE	PINNAME_GPIO4
#define GPIO_MCU_PWR	PINNAME_GPIO5
#define GPIO_CODEC_PWR	PINNAME_SPI_CLK

#define CMD_GET_RIGHT	1
#define CMD_PULL_CHARGE 2
#define CMD_PUSH_RED 3
#define CMD_OVER_SPEED 4
#define CMD_NEED_PARK 5
#define CMD_ENGIN_START 6
#define CMD_WECHAT_PAY	7
#define CMD_OUT_OF_RANGE 9


#define UNICODE_OUT_OF_RANGE "6C7D8F665DF28D8551FA56ED533A830356F4"
#define UNICODE_GET_RIGHT	"8BF775285FAE4FE1626B63CF4E8C7EF4780183B753D675288F6667439650"//"8BF783B753D675288F6667439650"
#define UNICODE_PULL_CHARGE "8BF762D463895145753563D25934"
#define UNICODE_PULL_RED "8BF762C98D777EA282727D27602565AD75355F005173"
#define UNICODE_OVER_SPEED "8F66901F8FC79AD8"
#define UNICODE_NEED_PARK "73AF8F66524D8BF762C98D77624B5239"
#define UNICODE_ENGIN_START "8BF763094E0B542F505C952E"
#define UNICODE_WECHAT_PAY "8BF775285FAE4FE173AF8F66"


enum GB32960_type_e{
	e_off = 0,
	e_login,
	e_rep,
	e_alert,
	e_normal
};
enum upgrade_type{
	arm_app = 3,
	mcu_app
};


enum audio_type{
	audio_local=1,
	audio_tts
};

enum ssl_step{
	connect_step=0,
	ssl_init_step,
	auth_step,
	finish_step
};

enum sound_ID{
	need_right_sound=1,
	unplug_line_sound,
	pull_emergent_sound,
	park_car_sound,
	out_range_sound
};


typedef struct{
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
	unsigned int msecond;
	time_t tick;
}date_time;

#define ERROR_CHARGE		0xFE
#define INVALID 			0xFF

#define CAR_STATUS_RUN		0x01
#define CAR_STATUS_OFF		0x02
#define CAR_STATUS_OTHER	0x03
#define PARK_CHARGE			0x01
#define RUN_CHARGE			0x02
#define NO_CHARGE			0x03
#define FINISH_CHARGE		0x04
#define ELECTRIC_VEHICLE	0x01
#define HYBRID_VEHICLE		0x02
#define PETROL_VEHICLE		0x03
#define DC_ON				0x01
#define DC_OFF				0x02
typedef struct{
	unsigned char car_status;
	unsigned char charge_status;
	unsigned char run_mode;  
	float car_speed;//0.1km/h
	float mileage;//0.1km
	float voltage;//0.1V
	float current;//0.1A      -1000A
	unsigned char SOC;//State of Charge 1%
	unsigned char DC;
	unsigned char gear;
	unsigned int insulation_resistance;//k¶∏
	unsigned char acce_val;
	unsigned char brake_val;
}vehicle_t;

#define MOTOR_USE_ELEC		0x01
#define MOTOR_CREATE_ELEC	0x02
#define MOTOR_STOP			0x03
#define MOTOR_STANDBY		0x04
typedef struct{
	unsigned char serial_num;
	unsigned char status;
	unsigned char driver_control_temperature;// 1°Ê-40°Ê
	unsigned int rpm;// 1r/min      -20000r/min
	float torsion;//0.1N*m		-2000N*m
	char driver_temperature;// 1°Ê-40°Ê
	float driver_control_vol;//0.1V
	float driver_control_current;//0.1A     -1000A
}driver_motor_detail_t;

typedef struct{
	unsigned char motor_num;// 4
	driver_motor_detail_t driver_data[4];
}driver_motor_t;

typedef struct{
	float cell_vol;// 0.1V
	float cell_current;//0.1A
	float fuel_consumption;//0.01kg/100km
	unsigned int temperature_probe_num;// 4
	int temp_val[4];// 1°Ê-40°Ê
	float H_max_tem;//0.1°Ê-40°Ê
	unsigned char H_max_temp_num;
	unsigned int H_max_con;// H concentration 1mg/kg
	unsigned char H_max_con_num;
	float H_max_press;//0.1Mpa
	unsigned char H_max_press_num;
	unsigned char DC;
}fuel_cell_t;

typedef struct{
	unsigned char engine_status;
	unsigned int rpm;// 1r/min
	float fuel_consumption;//0.01L/100km
}engine_t;

typedef struct{
	// 1230 0000 
	//[1:0 invalid 1 valid] 
	//[2:0 north 1 south]
	//[3:0 east 1 west]
	unsigned char status;
	double longitude;// *1000000
	double latitude;// *1000000
	double direction;
	double speed;
}location_t;

typedef struct{
	unsigned char max_vol_sys_num;
	unsigned char max_vol_bat_num;
	float max_bat_vol;//0.001V
	unsigned char min_vol_sys_num;
	unsigned char min_vol_bat_num;
	float min_bat_vol;//0.001V
	unsigned char max_temp_sys_num;
	unsigned char max_temp_prob_num;
	float max_temp;// 1°Ê-40°Ê
	unsigned char min_temp_sys_num;
	unsigned char min_temp_prob_num;
	float min_temp;// 1°Ê-40°Ê
}extreme_t;

typedef struct{
	unsigned char alert_level;
	unsigned char general_flag[4];
	unsigned char battery_error_num;
	unsigned int battery_error[4];
	unsigned char driver_error_num;
	unsigned int driver_error[4];
	unsigned char engine_error_num;
	unsigned int engine_error[4];
	unsigned char other_error_num;
	unsigned int other_error[4];
}alert_t;

typedef struct{
	unsigned char sys_num;
	float sys_vol;//0.1V
	float sys_current;//0.1A    -1000A
	unsigned int sum_btry_num;
	unsigned int start_num;
	unsigned char this_btry_num;
	float battery_vol[4];//0.001V
}battery_vol_detail_t;

typedef struct{
	unsigned char battery_sys_num;
	battery_vol_detail_t data[4];
}battery_vol_t;

typedef struct{
	unsigned char sys_num;
	unsigned int prob_num;
	unsigned char prob_val[4];// 1°Ê
}battery_temperature_detail_t;

typedef struct{
	unsigned char num;
	battery_temperature_detail_t data[4];
}battery_temperature_t;

typedef struct{
	unsigned int length;
	unsigned char can_data[255];
}custom_can_t;
typedef struct{
	date_time date;
	driver_motor_t driver_motor;
	vehicle_t vehicle;
	fuel_cell_t fuel_cell;
	engine_t engine;
	location_t location;
	extreme_t extreme;
	alert_t alert;
	battery_vol_t battery_vol;
	battery_temperature_t battery_temperature;
	custom_can_t custom_can;
}car_data_t;

typedef struct{
	unsigned char data[1024];
	int len;
}gl_data_t;
#define VIN_LENGTH			17
//package offset
#define DATA_HEAD_ADDR		0
#define CMD_FLAG_ADDR		2
#define ACK_FLAG_ADDR		3
#define VIN_ADDR			4
#define ENCRYPT_TYPE_ADDR	21
#define DATA_LENGTH_ADDR	22	
#define DATA_ADDR			24

#define DATA_YEAR_ADDR		24
#define DATA_MONTH_ADDR		25
#define DATA_DAY_ADDR		26
#define DATA_HOUR_ADDR		27
#define DATA_MIN_ADDR		28
#define DATA_SEC_ADDR		29

//head
#define DATA_HEAD 			0x23
//command flag
#define DATA_CAR_LOGIN 		0x01
#define DATA_CAR_MSG_SEND	0x02
#define DATA_CAR_MSG_REP	0x03
#define DATA_CAR_LOGOUT		0x04
#define DATA_PLAT_LOGIN		0x05
#define DATA_PLAT_LOGOUT	0x06
#define DATA_CMD_HEART		0x07
#define DATA_CMD_TIME		0x08
#define DATA_CMD_CHECK		0x80
#define DATA_CMD_SET		0x81
#define DATA_CMD_CONTROL	0x82
#define DATA_GET_RSA		0xC0
#define DATA_SEND_AES		0xC1
#define DATA_GET_VERSION	0xC2
#define DATA_RES_UPLOAD		0xC3

//ack flag
#define DATA_SUCCESS		0x01
#define DATA_ERROR			0x02
#define DATA_SAME_VIN		0x03
#define DATA_CMD			0xFE
//encrypt type
#define NO_ENCRYPT			0x01
#define RSA_TYPE			0x02
#define AES_TYPE			0x03
#define ENCRYPT_ERROR		0xFE
#define ENCRYPT_INVALID		0xFF
//message type
#define VEHICLE_DATA		0x01
#define DRIVER_MOTOR		0x02
#define FUEL_CELL			0x03
#define ENGINE_DATA			0x04
#define LOCATION_DATA		0x05
#define EXTREME_DATA		0x06
#define ALERT_DATA			0x07
#define BATTERY_VOL_DATA	0x08
#define BATTERY_TEMP_DATA	0x09
#define VOICE_CMD			0x0A
#define EXTEND_DATA			0x80
#define CUSTOM_CAN			0x90

//rcv cmd addr
#define CHECK_CMD_NUM_ADDR	30

//rcv param cmd
#define SET_FREQ_CMD		0x02
#define PLAT_URL_LEN		0x0D
#define PLAT_URL_SET		0x0E
#define PLAT_PORT			0x0F
#define ALLOW_IGNITION		0x80
#define ALLOW_FLAMEOUT		0x81




#define MSG_ID_NET_TO_MCU	1202
#define MSG_ID_AUDIO		1234



struct net_msg_stru
{
	long type;
	int len;
	unsigned char data[128];
};

struct mcu_msg_stru
{
	long type;
	unsigned char data[128];
};


typedef struct{
	 bool GPS_error;
     date_time D;
     char status;       
     double latitude;   
     double longitude;   
     char NS;             
     char EW;           
     double speed;        
     int high;   
	 int satelite_num;
	 int GPS_signal;
	 double direction;
}GPS_INFO;


typedef struct{
	const char* keyword;
	int type;
	void* value;
}set_param_t;

typedef struct{
	int heart_period;//s
	int data_period;//s
	int alert_period;
	char ICCID[21];
	char IMEI[16];//Õ®≥£15Œª
	char IMSI[16];//Õ®≥£15Œª
	char VIN[18];
	unsigned char btry_sys_num;// 1
	unsigned char sys_num_length;
	unsigned int btry_num;
	unsigned int temp_p_num;
	
	char url[128];
	int port;
	char platform_url[128];
	int platform_port;
	int save_period;//local sql save preiod
	int tcp_rcv_timeout;
	int next_login_time;
	int tbox_timeout;//no use just for MX platform
	int login_serial_num;
	
	int GB32690_type;
	char GB32690_type_note[64];
	int before_offline_time;
	int offline_time;
	int before_alert_time;
	int alert_time;

	int mcu_debug;
	int decrypt;
	int usbnet;
}param_t;

typedef struct{
	int ac_status;                   //AC
	int forward_defront_status;   //Èô§Èúú
	int behind_defront_status;  //ÂêéÈô§Èú?	
	int heating_status;   //Âà∂ÁÉ≠
	int air_circulation_mode;   //Á©∫Ê∞îÂæ™ÁéØ
	int blow_model; //ÂêπÈ£éÊñπÂêë
	int temperature;         //Ê∏©Â∫¶
	int windLevel;   //È£éÂäõ
}air_condition_param_t;

typedef struct{
	int ekey_status; //Èí•Âåô‰ΩøËÉΩ
	int electrify_status;  	//‰∏äÁîµ
	int charge_status;        //ÂÖÖÁîµ
	int engine_status;//ÂèëÂä®Êú∫Áä∂ÊÄ?	
	int control_panel_protection_status;//‰∏≠ÊéßÂ±è‰øùÊä?	
	int door_lock_status;//Èó®ÈîÅ bit  0-Â∑¶Ââç;1-Âè≥Ââç;2-Â∑¶Âêé;3-Âè≥Âêé;4-ÂêéÂ§áÁÆ±Ôºõ5-ÂâçËΩ¶Âé?	
	int door_status;//ËΩ¶Èó®Áä∂ÊÄÅ
	int door_lock_exception_status; //ËΩ¶ÈîÅÊó†ÊïàÁä∂ÊÄÅ
	int door_exceprtion_status;	//ËΩ¶Èó®Êó†ÊïàÁä∂ÊÄÅ
	int vehicle_body_defence_status; //ËΩ¶Ë∫´ËÆæÈò≤Áä∂ÊÄÅ
	int window_status[4];//ËΩ¶Á™ó   bit  0-Â∑¶Ââç;1-Âè≥Ââç;2-Â∑¶Âêé;3-Âè≥Âêé;
	int window_position[4];
	int sunroof_curtain_status;
	int sunroof_glass_status;
	int sunroof_glass_position;
	int high_beam_status;
	int low_beam_status;
	int fog_light_status;
	int left_cornering_lamp;
	int right_cornering_lamp;
	int markLamp; //ËΩÆÂªìÁÅØ
	int rechargeMileage; //Áª≠Ëà™ÈáåÁ®ã
	int accumulatedMileage;//Á¥ØËÆ°ÈáåÁ®ã
	int soc;//Ââ©‰ΩôÁîµÈáè
	int averageConsumption;//Âπ≥ÂùáËÉΩËÄó
}extend_vehicle_param_t;

typedef struct{
	unsigned char emmc_check_status;
	unsigned char tsp_check_status;
	unsigned char gps_check_status;
}device_check_param_t;

typedef struct{
	unsigned int pc_upgrade_status;
	unsigned int mcu_upgrade_status;
	unsigned int vcu_upgrade_status;
	unsigned int mcu_mcu_upgrade_status;
	unsigned int bms_upgrade_status;
	
	unsigned char pc_upgrade_path[512];
	unsigned char mcu_upgrade_path[512];
	unsigned char vcu_upgrade_path[512];
	unsigned char mcu_mcu_upgrade_path[512];	
	unsigned char bms_upgrade_path[512];
	
	unsigned char pc_upgrade_sign[512];
	unsigned char mcu_upgrade_sign[512];
	unsigned char vcu_upgrade_sign[512];
	unsigned char mcu_mcu_upgrade_sign[512];
	unsigned char bms_upgrade_sign[512];
}upgrade_param_t;

typedef struct{
    int rechargeMileage;
	int vehicleDirection;       
	int brakePedalPosition;     
	int brakePedalStatusValidity; 
	int brakePedalPositionValidity; 
	int accelerationPedalPositionValidity;
}tbox_extend_t;

#define CMD_HEAD	0xFF
#define CMD_NOACK	0xBB
#define CMD_POWER	0x05

#define PC_READY	0x04
#define PC_SLEEP	0x06
#define PC_UPGRADE	0x0A

#define CMD_EXTRA_CAR_STATE 0x4C
#define CMD_TO_MCU	0x4B
#define GEAR_INFO	0x0D
#define CMD_CAR_ODD3 0x2D
#define CMD_TBOX_CAR_INFO 0x58

#define KEY_PARAM 0x6
#define VEHICLE_PARAM 0x7
#define MCU_BASIC_INFOMATION 0x04
#define PC_BASIC_INFOMATION 0x84
#define MCU_AIR_CONDITION 0x01
#define PC_AIR_CONDITION 0x81
#define MCU_CAD 0x02
#define PC_CAD 0x82
#define MCU_ALERT 0x03

#define STATUS_DATA 0x1
#define CHECK_DATA 0x2
#define CONTROL_DATA 0x3

#define REAL_TIME_SPEED	0x0A
#define CMD_WAKE_SOURCE 0x86
#define WAKE_BY_RTC		0x01
#define DOOR_CONTROL	0x02
#define DOOR_LOCK		0x01
#define DOOR_UNLOCK		0x00
#define WINDOW_CONTROL	0x03
#define WINDOW_UP		0x01
#define WINDOW_STOP		0x00
#define WINDOW_DOWN		0x02
#define GPS_TIME		0x04

#define IGNITION_CMD_TO_MCU 0x01
#define FIND_CAR_CMD_TO_MCU 0x02

#define ERROR_REPLACEMENT_TIME 35		//replacement data in 30s when error appear

#define HARDWARE_VERSION "EC20_YEMA_TBOX_V0.1"
#define FIREWARE_VERSION "EC20CEFAGR06A01M4G_OCPU"
//SLEEP_SRC_CTRL_BIT mcu_ctrl_bit;
//SLEEP_SRC_CTRL_BIT sql_ctrl_bit;
//SLEEP_SRC_CTRL_BIT net_ctrl_bit;


#endif

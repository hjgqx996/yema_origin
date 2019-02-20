#ifndef __GENENRAL_H
#define __GENENRAL_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "network.h"
#include "mcu.h"
#include "4tbox_define.h"
#include "sql.h"
#include "gps.h"
#include <sys/stat.h>
#include "audio.h"
#include "ql_gpio.h"
#include "ql_oe.h"
#include "ql_eint.h"
#include "gpio_eint.h"
#include "http.h"

#define LOG_PATH	"/media/sdcard/log/tbox_%04d-%02d-%02d.log"
#define PARAM_PATH	"/home/root/param.json"
#define BAK_PARAM_PATH	"/usrdata/param.json"
#define UPGRADE_PATH "/home/root/upgrade.json"
#define FIERWARE_PATH "/etc/quectel-project-version"

#define ALL_VEHICLE 0
#define FRONT_VEHICLE 1
#define REAR_VEHICLE 2;

void get_param();
void init_save_param(const char* path);
void Log(const char *fun,const char *fmt, ...);
void Log_print(const char *fmt, ...);
unsigned char check_sum(unsigned char * buf,int len);
air_condition_param_t *get_air_conditon(int positon);
extend_vehicle_param_t *get_extend_vechicle_param();
device_check_param_t *get_device_check_param();
void init_get_device_check_param();

void init_save_upgrade();
bool get_json_upgrade(char *jsonStr);
void get_upgrade();
bool getFirewareVersion(char *firewareVersion);


#endif




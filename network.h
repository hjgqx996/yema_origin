#ifndef __NETWORK_H
#define __NETWORK_H
#include <stdbool.h>

void* TCP_Program(void* tcp_param);

void* module_init_thread(void* module_param);

bool down_load_verify(char* sign_base64,const char* download_path);


bool tcp_send(int clt_sock,unsigned char* buf,int len);
bool tcp_heart_send(int sock,unsigned char* buf,int len);

int login(int clt_sock,short serial_num);
int logout(int clt_sock,short serial_num);

void collision_alert();
void lilegal_encroachment();

void ack_engine_start();
void ack_door_start();
void ack_remote_ctrl_enable();
void ack_charge_start();
void ack_panel_protecion_ctrl();
void ack_air_ctrl();
void ack_truck_lock_ctrl();


#endif

#ifndef __MCU_H
#define __MCU_H

void* uart_thread(void* uart_param);
int OpenDev(char *Dev);
void set_speed(int fd, int speed);
int set_Parity(int fd,int databits,int stopbits,int parity);
int mcu_send(unsigned char* buf,int len);
int str_to_i(unsigned char *buf,int length);

void find_car_cmd();
void engine_start_cmd(unsigned char value);
void charge_start_cmd(unsigned char value);
void remote_ctrl_enable_cmd(unsigned char value);
void electromotor_ctrl_cmd(bool status);
void windows_ctrl_cmd(unsigned char place,unsigned char value);
void doors_ctrl_cmd(unsigned char place,unsigned char value);
void chair_ctrl_cmd(unsigned char place,unsigned char value);
void mcu_queue_send(unsigned char* buf,int len);
void PC_Power_cmd(unsigned char cmd);
void check_vehicle_status();
void panel_protection_cmd(unsigned char value);
void air_a_key_defrost();
void air_ac_max();
void air_heat_max();
void air_anion();
void air_cabin_clean();
void air_auto();
void air_purify_start();
void air_purify_stop();
void air_stop();
void air_set_temperature(unsigned char temperature);


#endif

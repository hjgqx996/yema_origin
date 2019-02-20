#ifndef __GPS_H
#define __GPS_H

void* gps_thread(void* gps_param);
int Ql_SendAT(char* atCmd, char* finalRsp, long timeout_ms);
void correctGps();


#endif

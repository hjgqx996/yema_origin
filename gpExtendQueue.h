#ifndef _GPEXTENDQUEUE_H
#define _GPEXTENDQUEUE_H

#include <iostream>
#include <string>
#include <queue>
#include "vehicle_info_extend.pb.h"
#include "4tbox_define.h"

using namespace std;

#define QUEUE_NUM 20

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
}location_queue;
	
class gpExtendQueue{
public:
        gpExtendQueue();
        ~gpExtendQueue(){};
        bool pushNewData(location_t *gpsData);
        bool packetGPSOffset(unsigned char *data,int *offsetbool,bool nowGPSValidStatus);
        bool clearGPSQueue();
private:
        queue<location_queue*> leisureGPSQueue;
        queue<location_queue*> usingGPSQueue;
        location_queue *gpses;
};
#endif


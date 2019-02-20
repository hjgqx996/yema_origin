#include "gpExtendQueue.h"

tbox_extend_t tbox_extend = {
    .rechargeMileage = 0xFFFF,
	.vehicleDirection = 0xFFFF,      
	.brakePedalPosition = 0xFF,     
	.brakePedalStatusValidity = 0xFF,
	.brakePedalPositionValidity = 0xFF, 
	.accelerationPedalPositionValidity = 0xFF,
};

gpExtendQueue::gpExtendQueue(){
	gpses = (location_queue*)malloc(sizeof(location_queue)*QUEUE_NUM);
	for(int i=0;i<QUEUE_NUM;++i){
		leisureGPSQueue.push(&gpses[i]);
	}
}

bool gpExtendQueue::pushNewData( location_t *gpsData){
	location_queue *temp;
	if(leisureGPSQueue.empty()){
		temp = usingGPSQueue.front();
		usingGPSQueue.pop();
	}else{
		temp = leisureGPSQueue.front();
		leisureGPSQueue.pop();
	}

	temp->status = gpsData->status;
	temp->longitude = gpsData ->longitude;
	temp->latitude = gpsData->latitude;
	temp->direction = gpsData ->direction;
	temp->speed = gpsData->speed;

	usingGPSQueue.push(temp);
	return true;
}

bool gpExtendQueue::packetGPSOffset(unsigned char *data,int *offset,bool nowGPSValidStatus){
	string msg;
	location_queue *preGps,*forwardGps;	
	VehicleInfoExtendMessage vehicleInfoExtendMessage;	
	LocationOffsetInfo locationOffsetInfo;
	unsigned int queueResidueNum = 2;
	if(!nowGPSValidStatus){
		queueResidueNum = 1;
	} 

	vehicleInfoExtendMessage.set_rechargemileage(tbox_extend.rechargeMileage);
	vehicleInfoExtendMessage.set_vehicledirection(tbox_extend.vehicleDirection);
	//vehicleInfoExtendMessage.set_brakepedalposition(tbox_extend.brakePedalPosition);
	vehicleInfoExtendMessage.set_brakepedalstatusvalidity(tbox_extend.brakePedalStatusValidity);
	vehicleInfoExtendMessage.set_brakepedalpositionvalidity(tbox_extend.brakePedalPositionValidity);
	vehicleInfoExtendMessage.set_accelerationpedalpositionvalidity(tbox_extend.accelerationPedalPositionValidity);
	
	while(usingGPSQueue.size() > queueResidueNum ){
		preGps = usingGPSQueue.front();
		usingGPSQueue.pop();
		forwardGps = usingGPSQueue.front();		
		LocationOffsetData *locationOffsetData = locationOffsetInfo.add_locationoffsetdata();
		//printf("%f %f  \n  %f %f \n %f %f \n %f %f\n",forwardGps->longitude ,preGps->longitude,forwardGps->latitude,preGps->latitude,forwardGps->direction , preGps->direction,forwardGps->speed, preGps->speed);
		//printf("%f %f %f %f\n",forwardGps->longitude - preGps->longitude,forwardGps->latitude - preGps->latitude,forwardGps->direction - preGps->direction,forwardGps->speed - preGps->speed);
		int lonOffset = (forwardGps->longitude - preGps->longitude)*1000000;
		int latOffset = (forwardGps->latitude - preGps->latitude)*1000000;
		int dirOffset = forwardGps->direction - preGps->direction;
		int speedOff = (forwardGps->speed - preGps->speed)*10;
		locationOffsetData->set_offsetlon(lonOffset);
		locationOffsetData->set_offsetlat(latOffset);
		locationOffsetData->set_offsetdir(dirOffset);
		locationOffsetData->set_offsetspeed(speedOff);
		leisureGPSQueue.push(preGps);
	}

	if(nowGPSValidStatus){
		preGps = usingGPSQueue.front();
		usingGPSQueue.pop();
		leisureGPSQueue.push(preGps);
	}
	
	vehicleInfoExtendMessage.set_allocated_locationoffsetinfo(&locationOffsetInfo);
	vehicleInfoExtendMessage.SerializeToString(&msg);
	
	data[(*offset)++] = EXTEND_DATA;
	data[(*offset)++] = (msg.length() >> 8) & 0xFF;
	data[(*offset)++] = msg.length() & 0xFF;
	memcpy(data+(*offset),msg.data(),msg.length());
	*offset += msg.length();
	
	vehicleInfoExtendMessage.release_locationoffsetinfo();
	return true;
}

bool gpExtendQueue::clearGPSQueue(){
	location_queue *temp;	
	while(usingGPSQueue.size()>0){
		temp = usingGPSQueue.front();
		usingGPSQueue.pop();
		leisureGPSQueue.push(temp);		
	}

	return true;
}


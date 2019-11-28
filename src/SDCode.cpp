#include <arduino.h>
#include <Wire.h>

#include "SDCode.h"
#include "fifoArray.h"

#define ARRAY_SIZE	128

void requestEvent_SDCode(void);
void receiveEvent_SDCode(int byteNum);
static void (*receiveHandler)(uint8_t *data, int len);
static bool busy;
static bool is_ctlPacket;
static uint8_t noPkt[11];
static uint8_t ctlPkt[20];
static uint8_t cmdPkt[6];

int cnt1, cnt2, cnt3;
long t1;
static FIFOArray fifo(ARRAY_SIZE);

SDCode::SDCode(void (*func)(uint8_t *data, int len))
{
	busy = 0;
	receiveHandler = func;
	noPkt[0] = 8;
	memcpy(&noPkt[1], "@!$#^%*&", 8); 
	ctlPkt[0] = 0x26;
	ctlPkt[1] = 0xA8;
	ctlPkt[2] = 0x14;
	ctlPkt[3] = 0xB1;
	ctlPkt[4] = 0x14;
	ctlPkt[5] = 0xF7;
	ctlPkt[14] = 0x2F;
	ctlPkt[16] = 0x64;
	ctlPkt[18] = 0x64;
	cmdPkt[0] = 0x26;
	cmdPkt[1] = 0xA8;
	cmdPkt[2] = 0x14;
	cmdPkt[3] = 0xB2;
}

void SDCode::Start(void)
{
	Wire.begin(I2C_DEVICE_ADDRESS);
    Wire.onRequest(requestEvent_SDCode);
    Wire.onReceive(receiveEvent_SDCode);
}

int SDCode::getCount()
{
	return fifo.count();
}

static void controlPush(void)
{
//	static unsigned long oldTime;


	//if(20 > (ARRAY_SIZE-fifo.count()))
	if(fifo.count() > 0)
		return;

	if(busy)
		return;
	busy = 1;

//	if(micros() > (oldTime+20000))
	{
	//	oldTime = micros();
		for(int n=0; n<20; n++)
			fifo.push(ctlPkt[n]);
	}
	busy = 0;
}


static uint8_t _checksum(uint8_t *data, int len)
{
	int checksum = 0;
	for(int n=0; n<len; n++)
        checksum += data[n];
    return (checksum&0xFF);
}

uint8_t SDCode::checksum(uint8_t *data)
{
	int len = data[4];
	if(len > 50)
		return 0;
	return _checksum(&data[6], len);
}


void SDCode::setData(uint8_t *data, int len)
{
	is_ctlPacket = 0;

	if(len > 40)
		return;


	if(len > (ARRAY_SIZE-fifo.count()))
		return;

	if(busy)
		return;
	busy = 1;

	for(int n=0; n<len; n++)
		fifo.push(data[n]);
	busy=0;
}

void SDCode::control(int16_t roll, int16_t pitch, int16_t yaw, int16_t throttle, int16_t option)
{
	int16_t *tmp = (int16_t *)&ctlPkt[6];

	is_ctlPacket = 1;
	tmp[0] = roll;
	tmp[1] = pitch;
	tmp[2] = yaw;
	tmp[3] = throttle;
	tmp[4] = option;

	ctlPkt[5] = _checksum(&ctlPkt[6], 14);
	cnt3 += 1;
}


void SDCode::control(int16_t roll, int16_t pitch, int16_t yaw, int16_t throttle, int16_t option, int16_t posvel, int16_t yawvel)
{
	int16_t *tmp = (int16_t *)&ctlPkt[6];

	is_ctlPacket = 1;
	tmp[0] = roll;
	tmp[1] = pitch;
	tmp[2] = yaw;
	tmp[3] = throttle;
	tmp[4] = option;
	tmp[5] = posvel;
	tmp[6] = yawvel;

	ctlPkt[5] = _checksum(&ctlPkt[6], 14);
	cnt3 += 1;
}


void SDCode::command(uint8_t *data, int len)
{

	if((len+6) > (ARRAY_SIZE-fifo.count()))
		return;

	if(busy)
		return;
	busy = 1;

	cmdPkt[4] = len+6;
	cmdPkt[5] = _checksum(data, len);
	for(int n=0; n<6; n++)
		fifo.push(cmdPkt[n]);
	for(int n=0; n<len; n++)
		fifo.push(data[n]);
	busy=0;
}



void requestEvent_SDCode(void)
{
	uint8_t cmd[21];
	cnt1 += 1;
	if(is_ctlPacket)
		controlPush();

	if(fifo.count()==0){
		Wire.write(noPkt, 11); 
		return;
	}

	if(busy){
		
		Wire.write(noPkt, 11); 
	}
	else{
		busy = 1;
		int len = fifo.count();
		len = (len>10)? 10 : len;
		cmd[0] = len;
		for(int n=1; n<=len; n++)
			cmd[n] = fifo.pop();
		Wire.write(cmd, 11); 
		busy = 0;

		return;
		if(micros() > (t1+1000000)){
			t1 = micros();
			Serial.print(cnt1);
			Serial.print(" ");
			Serial.print(cnt2);
			Serial.print(" ");
			Serial.println(cnt3);
			cnt1 = cnt2 = cnt3 = 0;
		}
	}
}


void receiveEvent_SDCode(int byteNum)
{
	static uint8_t rcvData[60];
	uint8_t data[8];
	static int offset = -1;
	cnt2 += 1;
	if(byteNum == 0)
		return;

    for(int n=0; n<byteNum; n++)
      rcvData[n] = Wire.read();

	if(receiveHandler)
		receiveHandler(rcvData, byteNum);

	return;
}
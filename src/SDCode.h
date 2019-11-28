/*
  SDCode.h

  Copyright (c) 2015, SteamMaker Inc.
*/

#ifndef __SDCODE_h__
#define __SDCODE_h__

#define I2C_DEVICE_ADDRESS	0x26

typedef enum{
        STATUS = 0x01,
        ATTPID,
        ALTPID,
        POSPID,
        TRIM,
        CALIBRATION,
        MISCEL,
        DEVICEINFO,
        SENSORS,
        SIGNAL,
		SUBORD,
        QC = 0xCC

} COMMAND_TYPE;

typedef struct 
{
	int size;
	int head;
	int tail;
	uint8_t *buf;
} array;

class SDCode
{
private:
	

public:
	void Start(void);
	int getCount();
	uint8_t checksum(uint8_t *cmd);
	SDCode(void (*func)(uint8_t *cmd, int len));
	void setData(uint8_t *data, int len);
	void control(int16_t roll, int16_t pitch, int16_t yaw, int16_t throttle, int16_t option);
	void SDCode::control(int16_t roll, int16_t pitch, int16_t yaw, int16_t throttle, int16_t option, int16_t posvel, int16_t yawvel);
	void command(uint8_t *data, int len);

};

#endif

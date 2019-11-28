
#include <SDCode.h>

void receiveHandler(uint8_t *cmd, int len);
SDCode sdcode(receiveHandler);

uint8_t buf[2];

int isSensorsRead;

void setup() {
  sdcode.Start();
  Serial.begin(115200);
  Serial.setTimeout(10);
  buf[0] = SENSORS;
  buf[1] = 1;
  sdcode.command(buf, 2);
}


void loop() {
    if(!isSensorsRead){
        sdcode.command(buf, 2);
        delay(100);
    }
}


void receiveHandler(uint8_t *cmd, int len)
{
    uint8_t checksum = sdcode.checksum(cmd);
    if(checksum == cmd[5]){
      if(cmd[7]&0x20){
        Serial.print("GyroX : ");
        Serial.println((int8_t)cmd[11]);
        Serial.print("GyroY : ");
        Serial.println((int8_t)cmd[12]);
        Serial.print("Alt : ");
        Serial.println(cmd[13]);
        isSensorsRead = 1;
      }
    }
}

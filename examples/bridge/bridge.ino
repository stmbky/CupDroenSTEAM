
#include <SDCode.h>

void receiveHandler(uint8_t *cmd, int len);
SDCode sdcode(receiveHandler);


void setup() {
  sdcode.Start();
  Serial.begin(115200);
  Serial.setTimeout(10);
  DDRD |= 0x20;
}


void loop() {
    if(Serial.available()){
          uint8_t rcvData[40];
          uint8_t ch = Serial.readBytes(rcvData, 40);
          sdcode.setData(rcvData, ch);
          PORTD ^= 0x20;
    }
}


void receiveHandler(uint8_t *cmd, int len)
{
    Serial.write(cmd, len);
    
}

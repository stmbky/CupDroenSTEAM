
#include <SDCode.h>

void receiveHandler(uint8_t *cmd, int len);
SDCode sdcode(receiveHandler);


int is_ready;

void setup() {
  sdcode.Start();
  Serial.begin(115200);
  Serial.setTimeout(10);  
}


void loop() {
    sdcode.control(0, 0, 0, 0, 0x8001, 0x02, 0);    
}


void receiveHandler(uint8_t *cmd, int len)
{
    uint8_t checksum = sdcode.checksum(cmd);
    if(checksum == cmd[5]){
      is_ready = !(cmd[7]&0x03);
    }
}

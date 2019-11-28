
#include <SDCode.h>

void receiveHandler(uint8_t *cmd, int len);
SDCode sdcode(receiveHandler);


int is_ready;

void setup() {
  sdcode.Start();
  Serial.begin(115200);
  Serial.setTimeout(10);
  sdcode.control(0, 0, 0, 0, 0x2F); 
}


void loop() {
    if(!is_ready)
      return;
    delay(5000);
    sdcode.control(0, 0, 0, 100, 0x2F);    
    delay(10000);
    sdcode.control(0, 0, 0, 0, 0x2F);  
    delay(10000);
}


void receiveHandler(uint8_t *cmd, int len)
{
    uint8_t checksum = sdcode.checksum(cmd);
    /*
    Serial.print(checksum);
    Serial.print(' ');
    Serial.print(cmd[5]);
    Serial.print(' ');
    Serial.print(is_ready);
    Serial.print(' ');
    Serial.println(cmd[7]);
    */
    if(checksum == cmd[5]){
      is_ready = !(cmd[7]&0x03);
    }
}

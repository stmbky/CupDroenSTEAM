
#include <SDCode.h>

void receiveHandler(uint8_t *cmd, int len);
SDCode sdcode(receiveHandler);

int trimX = 32767;
int trimY = 32767;
int _trimX;
int _trimY;
uint8_t buf[10];

void setup() {
  sdcode.Start();
  Serial.begin(115200);
  Serial.setTimeout(10);
}


void loop() {

    if(Serial.available()){
        char ch = Serial.read();
        if(ch == '1')
            trimX += 10;
        if(ch == '2')
            trimX -= 10;
        if(ch == '3')
            trimY += 10;
        if(ch == '4')
            trimY -= 10;            
    }
    if((_trimX != trimX) || (_trimY != trimY)){
        buf[0] = TRIM;
        int16_t *tmp = (int16_t *)&buf[1];
        tmp[0] = trimX;
        tmp[1] = trimY;
        sdcode.command(buf, 5);
    }
    delay(50);
}


void receiveHandler(uint8_t *cmd, int len)
{
    uint8_t checksum = sdcode.checksum(cmd);
    if(checksum == cmd[5]){
        if((cmd[8]&0x08) && (cmd[10]==TRIM)){
            int16_t *tmp = (int16_t *)&cmd[11];
            _trimX = tmp[0];
            _trimY = tmp[1];
            trimX = (trimX==32767)? _trimX : trimX;
            trimY = (trimY==32767)? _trimY : trimY;
            Serial.print(_trimX);
            Serial.print(" ");
            Serial.println(_trimY);
        }
    }
}

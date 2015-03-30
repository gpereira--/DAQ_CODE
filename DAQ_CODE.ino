
#include <SPI.h>
#include <mcp_can.h>
#include <i2cmaster.h>
#include "I2Cbus.h"
#include "AMT203.h"

//#define CS 3 //Chip or Slave select 

I2Cbus I2C;
MCP_CAN CAN(10);
AMT203 CUI(3);

void setup()
{
  /*
  Serial.begin(115200);
   Serial.println("Setup...");
   */

  I2C.setup();
  CUI.setup();

  if(CAN_OK == CAN.begin(CAN_500KBPS))
  {
    Serial.println("CAN BUS initialized.");
  }
  else
  {
    Serial.println("CAN BUS failed to initialize.");
    Serial.println("Trying again.");
    delay(100);
  }

  //ADC.setup(); 

}

unsigned char stmp[8] = {
  0, 1, 2, 3, 4, 5, 6, 7};

void loop()
{
  float temperature1 = I2C.readDevice(0x51);   // addresses from the scanner
  float temperature2 = I2C.readDevice(0x52);  

  float deg = CUI.read();

  // Print Data To Screen...
  /*
  Serial.println("Temperature Readings (*c):");
   Serial.print("> A1(51): ");
   Serial.println(temperature1);
   Serial.print("> A2(52): ");
   Serial.println(temperature2);
   */

  // send data:  id = 0x00, standrad flame, data len = 8, stmp: data buf
  CAN.sendMsgBuf(0x00, 0, 8, stmp);
  delay(100);                       // send data per 100ms

  delay(2000); 
}


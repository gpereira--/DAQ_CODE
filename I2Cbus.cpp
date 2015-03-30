#include <arduino.h>
#include <i2cmaster.h>
#include "I2Cbus.h"

I2Cbus::I2Cbus(void){

}

void I2Cbus::setup()
{
	i2c_init(); //Initialise then,
	PORTC = (1 << PORTC4) | (1 << PORTC5); //enable pullups
}

float I2Cbus::readDevice(int _address)
{
  int dev = _address << 1;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;

  // RAW READ
  i2c_start_wait(dev + I2C_WRITE);

  i2c_write(0x07);

  i2c_rep_start(dev + I2C_READ);

  data_low = i2c_readAck(); //Read 1 byte and then send ack
  data_high = i2c_readAck(); //Read 1 byte and then send ack
  pec = i2c_readNak();
  i2c_stop();

  //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
  double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
  double tempData = 0x0000; // zero out the data
  int frac; // data past the decimal point

  // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor)-0.01;

  //Process tempData
  float objTemp = tempData - 273.15;
  delay(500);
  return objTemp;

}

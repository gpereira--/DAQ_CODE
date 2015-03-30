#include <arduino.h>
#include <SPI.h>
#include "AMT203.h"

AMT203::AMT203(int CS) {
  pinMode(CS, OUTPUT);
  _CS = CS;
}

void AMT203::setup()
{
  pinMode(_CS, OUTPUT); //Slave Select
  digitalWrite(_CS, HIGH);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  delay(2000);
  SPI.end();
}

uint8_t AMT203::cmd(uint8_t _MSG, int _CS)    //Repetive SPI transmit sequence
{
  uint8_t temp = 0;            //vairable to hold recieved data
  digitalWrite(_CS, LOW);      //select spi device
  temp = SPI.transfer(_MSG);    //send and recieve
  digitalWrite(_CS, HIGH);     //deselect spi device
  return temp;                //return recieved byte
}

float AMT203::read()
{
  uint16_t ABSposition = 0;
  uint16_t ABSposition_last = 0;
  uint8_t temp[2];
  float deg = 0.00;

  uint8_t recieved = 0xA5;    //just a temp vairable
  ABSposition = 0;    //reset position vairable

  SPI.begin();    //start transmition
  digitalWrite(_CS, LOW);

  cmd(0x10, _CS);   //issue read command

  recieved = cmd(0x00, _CS);    //issue NOP to check if encoder is ready to send

  while (recieved != 0x10)    //loop while encoder is not ready to send
  {
    recieved = cmd(0x00, _CS);    //cleck again if encoder is still working
    delay(2);    //wait a bit
  }

  temp[0] = cmd(0x00, _CS);    //Recieve MSB
  temp[1] = cmd(0x00, _CS);    // recieve LSB

  digitalWrite(_CS, HIGH);  //just to make sure
  SPI.end();    //end transmition

  temp[0] &= ~ 0xF0;   //mask out the first 4 bits

  ABSposition = temp[0] << 8;    //shift MSB to correct ABSposition in ABSposition message
  ABSposition += temp[1];    // add LSB to ABSposition message to complete message

  deg = ABSposition;
  deg = deg * 0.08789;    // aprox 360/4096
  //Serial.println(deg);     //send position in degrees

  //delay(10);    //wait a bit till next check
  return deg;
}


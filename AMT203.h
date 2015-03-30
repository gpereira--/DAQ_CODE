#ifndef AMT203_h
#define AMT203_h

#include <arduino.h>
#include <SPI.h>

class AMT203
{
  public:
    AMT203(int CS);
    void setup(void);
    float read();
    uint8_t cmd(uint8_t _MSG, int _CS);
  private:
    int _CS;
    uint8_t _MSG;
};

#endif

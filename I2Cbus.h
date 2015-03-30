#ifndef I2Cbus_h
#define I2Cbus_h

#include <arduino.h>
#include <i2cmaster.h>

class I2Cbus
{
	public:
		I2Cbus(void);
		void setup(void);
		float readDevice(int _address);
        private:
		int _address;
};

#endif

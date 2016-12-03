#ifndef VEML6070_H
#define VEML6070_H

#include <Arduino.h>
#include <Wire.h>

#define VEML6070_ADDRESS		0x38
#define VEML6070_LSB			0x38
#define VEML6070_MSB			0x39

#define VEML6070_DEFAULT		0x02
#define VEML6070_ACK			(1 << 5)
#define VEML6070_ACK_TH			(1 << 4)
#define VEML6070_IT				(1 << 2)
#define VEML6070_SD 			(1 << 0)

typedef enum integration:byte{
	VEML6070_HALF,
	VEML6070_SINGLE,
	VEML6070_DOUBLE,
	VEML6070_QUADRUPLE,
};

class VEML6070{

public:

	VEML6070();
	VEML6070(byte);
	~VEML6070();


	void setSensivity(byte);
	void setAck(bool);
	void setAckTh(bool);
	void setShutdown(bool);

	int read();

protected:

	void _set(bool, bool, byte, bool);

private:
	int value;

	byte sensivity;
	bool ack;
	bool ack_th;
	bool shutdown;

};

#endif
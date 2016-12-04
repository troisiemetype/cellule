//VEML6070
/*
 * This library is used to communicate with the VEML6070 uv ligth sensor
 * Copyright (C) 2016  Pierre-Loup Martin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VEML6070_H
#define VEML6070_H

#include "Arduino.h"
#include "Wire.h"

//Address defines
//The data are read from two diffrent addresses on this sensor
#define VEML6070_ADDRESS		0x38
#define VEML6070_LSB			0x38
#define VEML6070_MSB			0x39

//sensor flags for sensor setup
#define VEML6070_DEFAULT		0x02
#define VEML6070_ACK			(1 << 5)
#define VEML6070_ACK_TH			(1 << 4)
#define VEML6070_IT				(1 << 2)
#define VEML6070_SD 			(1 << 0)

//Used to simplify implementation of different intergration time from user program
typedef enum VEML6070_integration{
	VEML6070_HALF,
	VEML6070_SINGLE,
	VEML6070_DOUBLE,
	VEML6070_QUADRUPLE,
} integration_time;

class VEML6070{

public:

	VEML6070();
	~VEML6070();

	void init(byte _sensivity = VEML6070_SINGLE);

	void setSensivity(byte);
	void setAck(bool);
	void setAckTh(bool);
	void setShutdown(bool);

	unsigned int read();

protected:

	void _set(bool, bool, byte, bool);

private:
	unsigned int value;

	byte sensivity;
	bool ack;
	bool ack_th;
	bool shutdown;

};

#endif
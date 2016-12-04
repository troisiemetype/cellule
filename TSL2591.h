//TSL2591
/*
 * This library is used to communicate with the TSL2591 visible/IR ligth sensor
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

#ifndef TSL2591_H
#define TSL2591_H

#include <Arduino.h>
#include <Wire.h>

//Addresses for the sensor control registers
#define TSL2591_ADDR_ENABLE					0x00
#define TSL2591_ADDR_CONFIG					0x01
#define TSL2591_ADDR_STATUS					0x13
#define TSL2591_ADDR_CH0_LSB 				0x14
#define TSL2591_ADDR_CH0_MSB				0x15
#define TSL2591_ADDR_CH1_LSB				0x16
#define TSL2591_ADDR_CH1_MSB				0x17

//Enable register fields
#define TSL2591_ENABLE_NPIEN				(1 << 7)		//No persist interrupt
#define TSL2591_ENABLE_SAI					(1 << 6)		//Sleep after interrupt
#define TSL2591_ENABLE_AIEN					(1 << 4)		//ALS interrupt enable
#define TSL2591_ENABLE_AEN					(1 << 1)		//ALS enable
#define TSL2591_ENABLE_PON					(1 << 0)		//Power on

//Control register fields
#define TSL2591_CONTROL_SRESET				(1 << 7)		//System reset
#define TSL2591_CONTROL_AGAIN				(1 << 4)		//Mesuring gain setting
#define TSL2591_CONTROL_ATIME				(1 << 0)		//Mesuring time setting

//control register gain values
#define TSL2591_CONTROL_GAIN_LOW			0x00			//Values for gain setting above
#define TSL2591_CONTROL_GAIN_MEDIUM			0x01
#define TSL2591_CONTROL_GAIN_HIGH			0x02
#define TSL2591_CONTROL_GAIN_MAX			0x03

//control register time values
#define TSL2591_CONTROL_TIME_100			0x00			//Values for time setting above
#define TSL2591_CONTROL_TIME_200			0x01
#define TSL2591_CONTROL_TIME_300			0x02
#define TSL2591_CONTROL_TIME_400			0x03
#define TSL2591_CONTROL_TIME_500			0x04
#define TSL2591_CONTROL_TIME_600			0x05






class TSL2591{
public:

	TSL2591();
	~TSL2591();

	void setTiming(byte);
	void setGain(byte);

	void readFull();
	void readLight();
	void readIr();


protected:

private:

};

#endif
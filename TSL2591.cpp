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

#include "TSL2591.h"

TSL2591::TSL2591():gain(TSL2591_CONTROL_GAIN_MEDIUM), time(TSL2591_CONTROL_TIME_200), ir(0), visible(0)
{
	//Connect to I2C
	Wire.begin();

}

TSL2591::~TSL2591(){

}

void TSL2591::init(){
	//Open I2C
	Wire.begin();

	//Enable the sensor
	enable();
	//Set timing and gain (two functions for convenience, but they do the same)
	setTiming(time);

}

//Enable the sensor
void TSL2591::enable(){
	//Set power on, set ALS
	write(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_ENABLE, TSL2591_ENABLE_AEN | TSL2591_ENABLE_PON);
}

//Disable the sensor
void TSL2591::disable(){
	write(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_ENABLE, 0);
}

void TSL2591::setTiming(byte _timing){
	time = _timing;
	write(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_CONFIG, gain | time);
}

void TSL2591::setGain(byte _gain){
	gain = _gain;
	write(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_CONFIG, gain | time);
}

unsigned int TSL2591::readFull(){
	unsigned int lsb = read(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_CH0_LSB);

}

unsigned int TSL2591::readLight(){

}

unsigned int TSL2591::readIr(){

}

void TSL2591::write(byte reg){
	Wire.beginTransmission(TSL2591_ADDRESS);
	Wire.write(reg);
	Wire.endTransmission();
}

void TSL2591::write(byte reg, byte value){
	Wire.beginTransmission(TSL2591_ADDRESS);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}

byte TSL2591::read(byte reg){

	byte lsb;
	write(reg);

	Wire.requestFrom(TSL2591_ADDRESS, 1);
	lsb = Wire.read();
	Wire.endTransmission();

	return lsb;

}

unsigned int TSL2591::readInt(byte reg){

	byte lsb = 0;
	byte msb = 0;

	write(reg);

	Wire.requestFrom(TSL2591_ADDRESS, 2);
	lsb = Wire.read();
	msb = Wire.read();
	Wire.endTransmission();

	return lsb | (msb << 8);

}
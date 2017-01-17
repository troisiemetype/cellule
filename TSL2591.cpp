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

TSL2591::TSL2591():
gain(1),
time(TSL2591_CONTROL_TIME_200),
ir(0),
visible(0),
full(0),
ev(0),
gainCoef{1, 24, 384},
gainReg{TSL2591_CONTROL_GAIN_LOW, TSL2591_CONTROL_GAIN_MEDIUM, TSL2591_CONTROL_GAIN_HIGH}
{
	//Connect to I2C
	Wire.begin();
}

TSL2591::~TSL2591(){
	disable();
}

void TSL2591::init(){
	//Open I2C
	Wire.begin();

	//Enable the sensor
	enable();
	//Set timing and gain (two functions for convenience, but they do the same)
	setTime(time);
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

//Set integration time
void TSL2591::setTime(byte _time){
	time = _time;
	write(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_CONFIG, gainReg[gain] | time);
	updateCpl();
}

//Set integration gain
void TSL2591::setGain(byte _gain){
	gain = _gain;
	write(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_CONFIG, gainReg[gain] | time);
	updateCpl();
}

//Update reading. Signal overflow
int TSL2591::update(){
	full = readFull();
	ir = readIr();

	//Test for over/underflow. Change gain if needed
	if((gain < 2) && (full < 500)){
		gain++;
		setGain(gain);
//		delay(250);
	} else if((gain > 0) && (full > 63000)){
		gain--;
		setGain(gain);
//		delay(250);
	}

/*
	Serial.print("gain: ");
	Serial.println(gain);
	
	Serial.print("full: ");
	Serial.println(readFull());

	Serial.print("light: ");
	Serial.println(readLight());

	Serial.print("IR: ");
	Serial.println(readIr());

	Serial.println();
*/
	return gain;
}

//Get a read of channel 0 (visible + IR)
unsigned int TSL2591::readFull(){
	full = readInt(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_CH0_LSB);
	return full;
}

//Get the difference between visible and IR
unsigned int TSL2591::readLight(){
	readFull();
	readIr();
	visible = full - ir;
	return visible;
}

//Get a read of channel 1 (IR only)
unsigned int TSL2591::readIr(){
	ir = readInt(TSL2591_COMMAND_NORMAL | TSL2591_ADDR_CH1_LSB);
	return ir;
}

//Get the lux value of channel 0 only (full)
unsigned long TSL2591::getLuxFull(){
	luxFull = ((float)full)/cpl;
	return luxFull;

}

//Get the lux value, with full and IR pondered
unsigned long TSL2591::getLux(){
	float lux1;
	float lux2;

	lux1 = ((float)full - TSL2591_LUX_CH1_CF1 * (float)ir)/cpl;
	lux2 = ((float)full * TSL2591_LUX_CH0_CF2 - (float)ir * TSL2591_LUX_CH1_CF2)/cpl;

//	lux1 = ((float)full - TSL2591_COEFB * (float)ir) * cpl;
//	lux2 = (TSL2591_COEFC * (float)full - TSL2591_COEFD * (float)ir) / cpl;

	luxLight = lux1 > lux2 ? lux1 : lux2;
/*
	Serial.print("lux 1: ");
	Serial.println(lux1);
	Serial.print("lux 2: ");
	Serial.println(lux2);
*/
	return luxLight;

}

//Get the ratio between IR and full
float TSL2591::getRatio(){
	return (float)ir / (float)full;
}

//Update the Count Per Lux ratio
void TSL2591::updateCpl(){
	cpl = (float)((time + 1) * 100) / (TSL2591_COUNT_PER_LUX * TSL2591_W_PER_LUMEN);
	cpl *= gainCoef[gain];
//	cpl = ((time + 1) * 100) * gainCoef[gain] / TSL2591_DF;
/*
	Serial.print("cpl: ");
	Serial.println(cpl);
	Serial.print("time: ");
	Serial.println((time + 1) * 100);
	Serial.print("gain: ");
	Serial.println(gainCoef[gain]);
	*/
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
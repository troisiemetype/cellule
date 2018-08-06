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

#include "VEML6070.h"

//Defaut constructor of the class
VEML6070::VEML6070():sensivity(1), ack(false), ack_th(false), shutdown(false), value(0)
{
	//Set the default sensivity. The datasheet advise to run first at 1, and change if needed
}

//class destructor
VEML6070::~VEML6070(){
	setShutdown(true);
}

//Constructor with sensivity passed as arg.
void VEML6070::init(byte _sensivity){

	//copy sensivity set in function call
	sensivity = _sensivity;

	//Open wire
	Wire.begin();
	//Send init command
	_set(ack, ack_th, sensivity, shutdown);

	//wait a bit for the sensor to init
	delay(200);
}

void VEML6070::enable(){
	setShutdown(false);
}

void VEML6070::disable(){
	setShutdown(true);
}

//send a init command
void VEML6070::_set(bool _ack, bool _ack_th, byte _sens, bool _shutdown) const{
	byte init_byte = VEML6070_DEFAULT;

	init_byte |= _ack << VEML6070_ACK;
	init_byte |= _ack_th << VEML6070_ACK_TH;
	init_byte |= _sens << VEML6070_IT;
	init_byte |= _shutdown << VEML6070_SD;

	Wire.beginTransmission(VEML6070_ADDRESS);
	Wire.write(init_byte);
	Wire.endTransmission();


}

//Convenience function to change only sensivity
void VEML6070::setSensivity(byte value){
	_set(ack, ack_th, value, shutdown);
	sensivity = value;
}

//Convenience function to change only Ack
void VEML6070::setAck(bool value){
	_set(value, ack_th, sensivity, shutdown);
	ack = value;
}

//Convenience function to change only Ack threshold
void VEML6070::setAckTh(bool value){
	_set(ack, value, sensivity, shutdown);
	ack_th = value;
}

//Convenience function to change only shutdown
void VEML6070::setShutdown(bool value){
	_set(ack, ack_th, sensivity, value);
	shutdown = value;
}

//Read value from the sensor
unsigned int VEML6070::read(){

	Wire.requestFrom(VEML6070_MSB, 1);

	value = Wire.read();
	value = value << 8;


	Wire.requestFrom(VEML6070_LSB, 1);
	value |= Wire.read();

	return value;
}

//Get the last value read
unsigned int VEML6070::lastReading() const{
	return value;
}


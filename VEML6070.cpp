#include "VEML6070.h"

//Defaut constructor of the class
VEML6070::VEML6070(){
	//Set the default sensivity
	sensivity = 1;

	VEML6070(sensivity);
}

//Constructor with sensivity passed as arg.
VEML6070::VEML6070(byte _sensivity){
	//init the state of the sensor
	ack = false;
	ack_th = false;
	shutdown = false;

	//copy sensivity set in function call
	sensivity = _sensivity;

	//Open wire
	Wire.begin();
	//Send init command
	_set(ack, ack_th, sensivity, shutdown);

	//wait a bit for the sensor to init
	delay(200);
}

//class destructor
VEML6070::~VEML6070(){

}

//send a init command
void VEML6070::_set(bool _ack, bool _ack_th, byte _sens, bool _shutdown){
	byte init_byte = VEML6070_DEFAULT;

	init_byte |= _ack && VEML6070_ACK;
	init_byte |= _ack_th && VEML6070_ACK_TH;
	init_byte |= _sens << VEML6070_IT;
	init_byte |= _shutdown && VEML6070_SD;


	Wire.beginTransmission(VEML6070_ADDRESS);
	Wire.write(init_byte);
	Wire.endTransmission();	

}

//Convenience function to change only sensivity
void VEML6070::setSensivity(byte value){
	_set(ack, ack_th, value, shutdown);
}

//Convenience function to change only Ack
void VEML6070::setAck(bool value){
	_set(value, ack_th, sensivity, shutdown);
}

//Convenience function to change only Ack threshold
void VEML6070::setAckTh(bool value){
	_set(ack, value, sensivity, shutdown);
}

//Convenience function to change only shutdown
void VEML6070::setShutdown(bool value){
	_set(ack, ack_th, sensivity, value);
}

//Read value from the sensor
int VEML6070::read(){
	value = 0;

	Wire.requestFrom(VEML6070_MSB, 1);

	value = Wire.read();
	value == value << 8;

	Wire.requestFrom(VEML6070_LSB, 1);
	value |= Wire.read();

	return value;

}


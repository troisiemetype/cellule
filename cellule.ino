//Cellule
/*
 * This program is a lightmeter for photography
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
#include <EEPROM.h>
#include <avr/sleep.h>

#include "TSL2591.h"
#include "VEML6070.h"

#include "Display.h"

//This is used to correct the lux value and get the correct aperture and speed
#define DEVICE_FACTOR 		280

#define SLEEP_DELAY 		5000

#define MODE_PHOTO 			0
#define MODE_COLLODION 		1
#define MODE_UV 			2

//tables for valid apertures, sensivities and speeds
float aperture[20] = {0.7, 1, 1.4, 2, 2.8, 4, 5.6, 8, 11, 16, 22, 32, 44, 64, 88, 128, 196, 256, 392, 512};
float sensivity[14] = {0.7, 1.5, 3, 6, 12, 25, 50, 100, 200, 400, 800, 1600, 3200, 6400};
int speeds[13] = {2, 4, 8, 15, 30, 60, 125, 250, 500, 1000, 2000, 4000, 8000};

//indexes for aperture and sensivity
//This are read from eeprom, look above
//unsigned char aIndex = 4;
//unsigned char sIndex = 7;

//default speed value, and speed mode
float speed = 1;
unsigned char speedMode = 0;

//vars for lus, ir and uv.
float lux = 0;
float ir = 0;
float uv = 0;

//var for ev.
float ev = 0;

//values set to eeprom, and their adresses
unsigned char aIndexAddr = 0;
unsigned char sIndexAddr = 1;
unsigned char modeAddr = 2;

unsigned char aIndex = EEPROM.read(aIndexAddr);
unsigned char sIndex = EEPROM.read(sIndexAddr);
unsigned char mode = EEPROM.read(modeAddr);


//Timer used to update readings
long prevMillis = millis();
int mesureDelay = 0;
long sleepDelay = millis();

TSL2591 light = TSL2591();
//VEML6070 blue = VEML6070();

Display display = Display();

void setup(){

	pinMode(2, INPUT_PULLUP);
	pinMode(3, INPUT_PULLUP);

	Serial.begin(115200);

	initDevices();

	display.drawLogo(0, 0, 10);
	display.update();

	//verify the values from eeprom. If not correct, set it to default
	if(aIndex > (sizeof(aperture)/sizeof(float) - 1)){
		aIndex = 4;
	}
	if(sIndex > (sizeof(sensivity)/sizeof(float) - 1)){
		sIndex = 7;
	}
	if(mode > MODE_UV){
		mode = MODE_PHOTO;
	}

	//Wait enough for a reading to be made
	while((millis() - prevMillis) < 1000);


}

void initDevices(){

	display.begin();

	light.init();
}

void loop(){
//	Serial.print("blue: ");
//	Serial.println(blue.read());

//	Serial.print("Gain: ");
//	Serial.println(light.update());
/*
	Serial.print("ratio: ");
	Serial.println(light.getRatio());
	
	Serial.print("full: ");
	Serial.println(light.readFull());

	Serial.print("light: ");
	Serial.println(light.readLight());

	Serial.print("IR: ");
	Serial.println(light.readIr());

	Serial.print("lux full: ");
	Serial.println(light.getLuxFull());

	Serial.println("");
*/
	//update reading only if a long enough time (250ms) has elapsed since last reading
	mesureDelay = millis() - prevMillis;
	if(mesureDelay > 250){
		prevMillis = millis();
		light.update();
		lux = light.getLux();
	}
	
	//Compute speed and ev.
	speed = pow(aperture[aIndex], 2) * DEVICE_FACTOR / (lux * sensivity[sIndex]);
	ev = log(pow(aperture[aIndex], 2))/log(2) + log(1/speed)/log(2);

	//format the speed value, for displaying in minutes, seconds or fractionnal form
	if(speed >= 1){
		speedMode = SPEED_SECONDS;
	} else if(speed >= 60){
		speedMode = SPEED_MINUTES;
	} else {
		speedMode = SPEED_NORMAL;
		for (unsigned char i = 0; i < 13; i++){
			if ((1 / speed) <= speeds[i]){
				speed = speeds[i];
				break;
			}
		}
	}

	//Display the updated values.
	//Todo: update only when the button has been pushed, or the increment wheel turned.
	display.updateIso(sensivity[sIndex]);

	display.updateAperture(aperture[aIndex]);
	display.updateSpeed(speed, speedMode);

//	display.updateGain(light.update());

	display.updateEV(ev);
	display.updateMode(mode);
	display.updateBarEV(ev);

	display.update();

	//Go to sleep if nothing has been done for some time
	if((millis() - sleepDelay) > SLEEP_DELAY){

//		display.clear();
//		display.update();
		goToSleep();

		sleepDelay = millis();
	}
}

void saveEeprom(){
	EEPROM.update(aIndexAddr, aIndex);
	EEPROM.update(sIndexAddr, sIndex);
	EEPROM.update(modeAddr, mode);
}

void goToSleep(){
	//enable sleep, set power down mode
	SMCR |= (1 << SE);
	SMCR |= (1 << SM1);

	//disable watchdog timer
	WDTCSR = 0;
	//disable Brown Out detector
	MCUCR |= (1 << BODSE);
	MCUCR |= (1 << BODS);

	//Set an interrupt ont INT0 to wakeup.
	EICRA = 0;
	EIMSK = B1;

	light.disable();
	display.off();
	char sregTemp = SREG;

//	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();

	//disable sleep
	SMCR &= ~(1 << SE);

	//Restore status register
	SREG = sregTemp;

	//reinit devices
	initDevices();
}

ISR(INT0_vect){
	//clear the interrupts, are they are only for sleep wakeup.
	EIMSK = 0;

}

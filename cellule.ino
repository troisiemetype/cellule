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

#include "Encoder.h"
#include "PushButton.h"

//This is used to correct the lux value and get the correct aperture and speed
#define DEVICE_FACTOR 		280

#define SLEEP_DELAY 		30000

#define MODE_PHOTO 			0
#define MODE_COLLODION 		1
#define MODE_UV 			2

#define RATIO_UV			60

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

//vars for lux, ir and uv.
float lux = 0;
float ir = 0;
float uv = 0;

//var for ev.
float ev = 0;
float lv = 0;

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
VEML6070 blue = VEML6070();

Display display = Display();

Encoder wheel = Encoder();

PushButton btnIso = PushButton();
PushButton btnMenu = PushButton();
PushButton btnHold = PushButton();

void setup(){

	Serial.begin(115200);

	mode = MODE_COLLODION;
	sIndex = 0;
	aIndex = 7;

	// Init encoder
	wheel.begin(4, 5, Encoder::QUAD_STEP);
	wheel.setDebounceDelay(10);
	wheel.reverse();

	// Init buttons.
	btnIso.begin(6, INPUT_PULLUP);
	btnMenu.begin(7, INPUT_PULLUP);
	btnHold.begin(8, INPUT_PULLUP);


	initDevices();

	display.drawLogo(0, 0, 25);
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

//	Serial.println("started");


}

void initDevices(){

	display.begin();

	light.init();

	blue.init(VEML6070::SINGLE);
//	blue.enable();

	Wire.setClock(400000L);

}

void loop(){

	uint8_t update = 0;

	//update reading only if a long enough time (250ms) has elapsed since last reading
	mesureDelay = millis() - prevMillis;
	if(mesureDelay > 300){
		update = 1;

		if((mode == MODE_PHOTO) || (mode == MODE_COLLODION)){
			light.update();
			lux = light.getLux();
		}

		if((mode == MODE_COLLODION) || (mode == MODE_UV)){
			uv = blue.read();
		}
		prevMillis = millis();
	}

	btnIso.update();
	btnMenu.update();
	btnHold.update();

	if(wheel.update()){
		update = 1;
		int8_t step = wheel.getStep();
		if(btnIso.isPressed()){
			// Changing iso
			if(step < 0 && sIndex > 0) --sIndex;
			if(step > 0 && sIndex < 13) ++sIndex;
		} else if(btnMenu.isPressed()){
			// Changing mode
			if(step < 0 && mode > 0) --mode;
			if(step > 0 && mode < 2) ++mode;			
		} else {
			// Changing aperture value
			if(step < 0 && aIndex > 0) --aIndex;
			if(step > 0 && aIndex < 19) ++aIndex;
		}
	}

	if(update){

		float fullLV = 0;

		// Compute speed and ev.
		// lv = log(2)(lux) - 1.32
		// 1.32 is given by the correspondance table between LV and lux.
		// This way of computing gives a max difference of 0.2 LV from the one using device factor
		lv = (log(lux) / log(2)) - 1.32;
		fullLV = lv;

		if(mode == MODE_COLLODION){
			fullLV = (lv * (100 - RATIO_UV) + uv * RATIO_UV) / 100;
		}

		// compute EV from LV
		ev = fullLV + (log(sensivity[sIndex] / 100) / log(2));

		// Compute speed given aperture and EV
		// ev = log(2)(A square / S) = log(2)(A square) - log(2)(S)
		// log(2)(S) = log(2)(A square) - ev
		// S = exp(2)(log(2)(A square) - ev)
		speed = pow(2, ((log(aperture[aIndex]) / log(2)) - ev));

/*
		// First version, before lv pre-compute

		// speed = aperture square * device factor / (lux * sensivity)
		speed = pow(aperture[aIndex], 2) * DEVICE_FACTOR / (lux * sensivity[sIndex]);

		// EV = log(2)(aperture square) + log(2)(1/speed)
		ev = log(pow(aperture[aIndex], 2))/log(2) + log(1/speed)/log(2);
*/
		//format the speed value, for displaying in minutes, seconds or fractionnal form
		if(speed >= 300){
			speedMode = SPEED_MINUTES;
			speed /= 60;
		} else if(speed >= 1){
			speedMode = SPEED_SECONDS;
		} else {
			speedMode = SPEED_NORMAL;
			for (unsigned char i = 0; i < 13; i++){
				if ((1 / speed) <= speeds[i]){
					speed = speeds[i];
					break;
				}
			}
		}

		updateDisplay();
	}

	return;

	//Go to sleep if nothing has been done for some time
	if((millis() - sleepDelay) > SLEEP_DELAY){

//		display.clear();
//		display.update();
		goToSleep();

		sleepDelay = millis();
	}
}

void updateDisplay(){
	//Display the updated values.
	//Todo: update only when the button has been pushed, or the increment wheel turned.
	display.clear();
	display.updateIso(sensivity[sIndex]);

	display.updateMode(mode);

	display.updateAperture(aperture[aIndex]);

	display.updateSpeed(speed, speedMode);

//	display.updateGain(light.update());

	display.updateLV(lv);
	if((mode == MODE_PHOTO) || (mode == MODE_COLLODION)) display.updateEV(ev);
//	display.updateIR(ir);
	if((mode == MODE_COLLODION) || (mode == MODE_UV)) display.updateUV(uv);
//	display.updateBarEV(ev);

//	display.showBars(0);

	display.update();


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
//	MCUCR |= (1 << BODSE);
//	MCUCR |= (1 << BODS);

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

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

#include "TSL2591.h"
#include "VEML6070.h"

#include "Display.h"

float aperture[20] = {0.7, 1, 1.4, 2, 2.8, 4, 5.6, 8, 11, 16, 22, 32, 44, 64, 88, 128, 196, 256, 392, 512};
unsigned char aIndex = 4;

float sensivity[14] = {0.7, 1.5, 3, 6, 12, 25, 50, 100, 200, 400, 800, 1600, 3200, 6400};
unsigned char sIndex = 7;

int speeds[13] = {2, 4, 8, 15, 30, 60, 125, 250, 500, 1000, 2000, 4000, 8000};

float speed = 1;
unsigned char speedMode = 0;

float lux = 0;
float ir = 0;
float uv = 0;

float ev = 0;

TSL2591 light = TSL2591();
//VEML6070 blue = VEML6070();

Display display = Display();

void setup(){

	Serial.begin(115200);

	display.begin();

//	display.drawLogo(0, 0, 10);

	light.init();

//	delay(1000);

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
	light.update();

	lux = light.getLux();
	ev = log(lux) / log(2);
	speed = pow(aperture[aIndex], 2) * 300 / (lux * sensivity[sIndex]);

	if(speed >= 1){
		speedMode = 1;
	} else if(speed >= 60){
		speedMode = 2;
	} else {
		speedMode = 0;
		for (unsigned char i = 0; i < 13; i++){
			if ((1 / speed) < speeds[i]){
				speed = speeds[i];
				break;
			}
		}
	}

	display.updateAperture(aperture[aIndex]);
	display.updateSpeed(speed, speedMode);

//	display.updateGain(light.update());

	display.updateEV(ev);
	display.updateIso(sensivity[sIndex]);
	display.updateMode(1);

	display.update();

	delay(500);
}

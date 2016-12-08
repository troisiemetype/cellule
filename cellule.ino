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

TSL2591 light = TSL2591();
//VEML6070 blue = VEML6070();

int visible = 0;
int gain = 1;

void setup(){
	Serial.begin(115200);

	Serial.println("started");
	Serial.println("");

	light.init();
	light.setGain(gain);
	light.setTime(TSL2591_CONTROL_TIME_200);

	delay(500);
}

void loop(){
//	Serial.print("blue: ");
//	Serial.println(blue.read());

	light.update();
/*	
	Serial.print("full: ");
	Serial.println(light.readFull());

	Serial.print("light: ");
	Serial.println(light.readLight());

	Serial.print("IR: ");
	Serial.println(light.readIr());

	Serial.println("");
	*/
	delay(500);


}


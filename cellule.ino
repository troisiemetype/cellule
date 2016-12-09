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

void setup(){
	Serial.begin(115200);

	Serial.println("started");
	Serial.println("");

	light.init();

	delay(500);

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

	Serial.print("lux light: ");
	Serial.println(light.getLuxLight());

	Serial.println("");
*/

	light.update();

	Serial.println(ev());



	delay(500);
}

float ev(){
	return log(light.getLux()) / log(2);
}


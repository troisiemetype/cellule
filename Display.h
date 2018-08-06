//Display
/*
 * This library is intended to manage diplay of the cellule
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

/*
 * This class manages the setup of the display.
 * It is used to init the display to blank at power-up.
 * It provides conveniences functions to display and update values to be shown.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MODE_PHOTO			0
#define MODE_COLLODION		1
#define MODE_UV 			2

#define SPEED_NORMAL 		0
#define SPEED_SECONDS 		1
#define SPEED_MINUTES 		2

#define LINE_OFFSET			2
#define FONT_OFFSET 		16


class Display{
public:
	Display();

	void begin();

	void update();
	void clear();
	void off();

	void updateAperture(float value);
	void updateSpeed(int value, byte mode = 0);
	void updateIso(float value);
	void updateEV(float value);
	void updateIR(int value);
	void updateUV(int value);
	void updateMode(byte mode);

	void showBars(bool value);

	void updateBarEV(float value);
	void updateBarIR(float value);
	void updateBarUV(float value);

	void drawLogo(byte x, byte y, byte size);

	void updateGain(int gain);

	void ln();
	void setPosition(byte x, byte y);

private:
	Adafruit_SSD1306 _display;
	byte _cursor, _posAperture, _posSpeed, _posIso, _posEV, _posMode;
};

#endif
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

#include "Display.h"

//the added font is called here, as it is easyest to have just one file to modify if needed.
#include <Fonts/FreeSerifItalic9pt7b.h>

//Constructor of the class. All attributes initialized.
Display::Display(){
	_posAperture = 19;
	_posSpeed = 48;
	_posIso = 64;
	_posEV = 75;
	_posMode = 86;

}

//To be called during setup()
void Display::begin(){
	//Clear the display buffer, set rotation, set standard color.
	_display.begin();
	_display.clearDisplay();
	_display.setTextColor(WHITE);
	_display.setRotation(0);
	_display.dim(0);

	//Debugging purpose: set a rule on the left border, used to count rows
/*	
	for (byte i = 0; i < 64; i++){
		_display.drawPixel(63, 2 * i, WHITE);
	}
	for (byte i = 0; i < 13; i++){
		_display.drawPixel(62, 10 * i, WHITE);
	}
*/
	//Display a rule on the right side, to mesure ev.
	//
/*
	for (byte i = 0; i < 22; i++){
		_display.drawPixel(62, 127 - (6 * i), WHITE);
		if(i == 2 || i == 12 || i == 18){
			_display.drawPixel(63, 127 - (6 * i), WHITE);
		}
	}
*/
//	showBars(true);


}

void Display::off(){
	_display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void Display::update(){
	_display.display();
	_cursor = 1;
	_display.setCursor(0, _cursor);
}

void Display::clear(){
	_display.clearDisplay();
}

//Update the aperture value on display
//Set font to be used, cursor position, erase previous value by drawing a filled rectangle.
//Print "f/", then value passed to method.
void Display::updateAperture(float value){
	_display.setFont(&FreeSerifItalic9pt7b);
	_cursor += FONT_OFFSET;
	_display.setCursor(0, 25);
//	_display.fillRect(0, _cursor - FONT_OFFSET, 62, 21, BLACK);
	_display.print("f/");
	if(value == 0.7 || value == 1.4 || value == 2.8 || value == 5.6){
		_display.print(value, 1);
	} else {
		_display.print(value, 0);
	}
	_cursor = _display.getCursorY() - FONT_OFFSET;
//	_display.display();

}

void Display::updateSpeed(int value, byte mode){
	_display.setFont(&FreeSerifItalic9pt7b);
	_cursor += FONT_OFFSET;
	_display.setCursor(64, 25);
//	_display.fillRect(0, _cursor - FONT_OFFSET, 62, 21, BLACK);
	if(mode == SPEED_NORMAL){
		_display.print("1/");
	}
	_display.print(value);
	if(mode == SPEED_SECONDS){
		_display.print("s");
	} else if(mode == SPEED_MINUTES){
		_display.print("m");
	}
//	_display.println();

	_cursor = _display.getCursorY() - FONT_OFFSET;
//	_display.display();

}

void Display::updateSpeedCol(int value, byte mode){
	_display.setFont(&FreeSerifItalic9pt7b);
	_display.setCursor(64, 45);
	if(mode == SPEED_NORMAL){
		_display.print("1/");
	}
	_display.print(value);
	if(mode == SPEED_SECONDS){
		_display.print("s");
	} else if(mode == SPEED_MINUTES){
		_display.print("m");
	}
}

void Display::updateIso(float value){
	_display.setFont();
	_display.setCursor(0, 0);
//	_display.fillRect(0, _cursor - LINE_OFFSET, 62, 10 + LINE_OFFSET, BLACK);
	_display.print("iso ");

	if((value == 0.7) || (value == 1.5)){
		_display.print(value, 1);
	} else {
		_display.print(value, 0);
	}
//	_display.println();
	_cursor = _display.getCursorY() + LINE_OFFSET;
//	_display.display();

}

void Display::updateLV(float value){
	_display.setFont();
	_display.setCursor(0, 35);
	_display.print("LV ");
	_display.println(value, 1);

	_cursor = _display.getCursorY() + LINE_OFFSET;

}

void Display::updateEV(float value){
	_display.setFont();
	_display.setCursor(0, 45);
//	_display.fillRect(0, _cursor - LINE_OFFSET, 62, 10 + LINE_OFFSET, BLACK);
	_display.print("EV ");
	_display.println(value, 1);

	_cursor = _display.getCursorY() + LINE_OFFSET;

//	_display.display();

}

void Display::updateIR(int value){
	_display.setFont();
	_display.setCursor(0, 45);
//	_display.fillRect(0, _cursor - LINE_OFFSET, 62, 10 + LINE_OFFSET, BLACK);
	_display.print("IR ");
	_display.print(value);

	_cursor = _display.getCursorY() + LINE_OFFSET;

}

void Display::updateFLV(float value){
	_display.setFont();
	_display.setCursor(0, 55);
//	_display.fillRect(0, _cursor - LINE_OFFSET, 62, 10 + LINE_OFFSET, BLACK);
	_display.print("full ");
	_display.print(value);

	_cursor = _display.getCursorY() + LINE_OFFSET;

}

void Display::updateUV(int16_t value){
	_display.setFont();
	_display.setCursor(0, 45);
//	_display.fillRect(0, _cursor - LINE_OFFSET, 62, 10 + LINE_OFFSET, BLACK);
	_display.print("UV ");
	_display.print(value);

	_cursor = _display.getCursorY() + LINE_OFFSET;

}

void Display::updateMode(byte mode){
	_display.setFont();
	_display.setCursor(64, 0);
//	_display.fillRect(0, _cursor - LINE_OFFSET, 62, 10 + LINE_OFFSET, BLACK);
	if(mode == 0){
		_display.print("photo");
	} else if(mode == 1){
		_display.print("collodion");
	} else {
		_display.print("uv print");

	}

	_cursor = _display.getCursorY() + LINE_OFFSET;

//	_display.display();

}

void Display::showBars(bool value){
	unsigned char tempCursor = _display.getCursorY();

	_display.setCursor(30, 98);
	_display.println("UV");
	_display.println("EV");
	_display.println("IR");

	_display.drawFastHLine(40, 101, 16, WHITE);
	_display.drawFastHLine(40, 110, 19, WHITE);
	_display.drawFastHLine(40, 118, 22, WHITE);

	_display.drawFastVLine(55, 95, 6, WHITE);
	_display.drawFastVLine(58, 95, 15, WHITE);
	_display.drawFastVLine(61, 95, 24, WHITE);

}

void Display::showHold(){
	_display.setFont();
	_display.setCursor(104, 55);
	_display.print("hold");

}

void Display::updateBarEV(float value){
	_display.fillRect(60, 0, 2, 128, BLACK);
	byte height = (value + 2) * 6;
	_display.fillRect(60, 128 - height, 2, height, WHITE);
}

void Display::updateBarIR(float value){
	
}

void Display::updateBarUV(float value){
	
}

void Display::drawLogo(byte x, byte y, byte size){
	_display.drawCircle(x + size, y + size, size, WHITE);
	_display.drawCircle(x + 2.6 * size, y + (3 * size / 2) - 0.1 * size, size / 2, WHITE);
	_display.drawCircle(x + 3.8 * size, y + (3 * size / 2) - 0.1 * size, size / 2, WHITE);
//	_display.display();
}

void Display::updateGain(int gain){
	_display.setFont();
	_display.setCursor(64, 55);
	_display.print("gain ");
	_display.print(gain);
}

void Display::ln(){
	_display.println();
}

void Display::setPosition(byte x, byte y){
	_display.setCursor(x, y);
}
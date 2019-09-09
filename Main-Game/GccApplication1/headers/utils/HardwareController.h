#ifndef ARCADESHOOTER_HARDWAREINTERFACE_H
#define ARCADESHOOTER_HARDWAREINTERFACE_H

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

#include "LED.h"
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#include "LCD.h"
#include "Constants.h"



#define delay 100


class HardwareController {

public:
	static void displayBoardMatrix(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
		
		int rows = BOARD_HEIGHT, cols = BOARD_WIDTH;

		for (int i = 0; i < rows; i++) {
			PORTB = 0xFF;			//Turning the columns off
			PORTA = (1 << i);		//Enabling the ith row

			unsigned char temp = 0xFF;
			for (int j = 0; j < cols; j++) {
				if (board[i][j] == 1 || board[i][j] == 2 || board[i][j] == 3) {
					temp &= ~(1 << j); //Putting 0 to the corresponding column
				}
			}

			PORTB = temp;
			_delay_us(10);
		}
	}

	static int getGyroReading() {
		
		char rawInput = PINA; 
		char inputLSB = rawInput & 0b00000011 ;

		switch (inputLSB) {
			case 0: return GYRO_CALIBRATING;
			case 1: return GYRO_MOVE_RIGHT;
			case 2: return GYRO_MOVE_LEFT;
			case 3: return GYRO_STAY_STILL;
			default: return GYRO_INVALID;
		}
	}

	static void hardWareSetup()
	{
		//LED MATRIX OUTPUT
		DDRA = 0b11111000;
		DDRB = 255;
		
		//LCD DISPLAY
		DDRD = 0xFF;
		DDRC = 0xFF;
		
		Lcd4_Init();
		displayLcd("Score: 0","Health Full");
	}


	//point by point scan
	static void displayBoardMatrixBackup(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
		
		int rows = BOARD_HEIGHT, cols = BOARD_WIDTH;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (board[i][j]) {
					printLedMatrixUsingOneDecoder(j,i);
					_delay_us(delay);
				}
			}
		}
	}

	static void displayLcd(char * upper,char * lower) {
		Lcd4_Clear();
		Lcd4_Set_Cursor(1, 0);
		Lcd4_Write_String(upper);
		Lcd4_Set_Cursor(2, 0);
		Lcd4_Write_String(lower);
	}

	static bool isShootButtonPressed() {
		char res = PINA;
		res = PINA & 0b00000100;

		return res; 
	}

	static void enableBuzzer() {
		PORTA = PORTA | (1 << 3);
	}

	static void disableBuzzer() {
		PORTA = PORTA & (~(1 << 3));
	}
	
	// 00 = calibratomg ,01 = right, 10 = left, 11 = nothing
};

#endif //ARCADESHOOTER_HARDWAREINTERFACE_H
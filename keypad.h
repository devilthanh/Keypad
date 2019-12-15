#ifndef KEYPAD_H
#define KEYPAD_H
#include "Arduino.h"

#define LONG_INTERVAL 1000
#define SHORT_INTERVAL 100
#define NUMBER_LENGTH	6
#define KEY_MODE 		0
#define TEXT_MODE 		1
#define NUMBER_MODE 	2
#define TXT_CLEAR 		11

#define KEY_1			0
#define KEY_2			1
#define KEY_3			2
#define KEY_4			4
#define KEY_5			5
#define KEY_6			6
#define KEY_7			8
#define KEY_8			9
#define KEY_9			10
#define KEY_0			13
#define KEY_STAR		12
#define KEY_HASH		14
#define KEY_A			3
#define KEY_B			7
#define KEY_C			11
#define KEY_D			15

const char keys[16] 	PROGMEM= {"123A456B789C*0#D"};

const char keys_1[] 	PROGMEM= {",.?!:;-+#*()'\"_@&$%/<>|=[]1"};
const char keys_2[] 	PROGMEM= {"abc2"};
const char keys_3[] 	PROGMEM= {"def3"};
const char keys_4[] 	PROGMEM= {"ghi4"};
const char keys_5[] 	PROGMEM= {"jkl5"};
const char keys_6[] 	PROGMEM= {"mno6"};
const char keys_7[] 	PROGMEM= {"pqrs7"};
const char keys_8[] 	PROGMEM= {"tuv8"};
const char keys_9[] 	PROGMEM= {"wxyz9"};
const char keys_star[] 	PROGMEM= {"+*"};
const char keys_0[] 	PROGMEM= {" 0"};
const char keys_hash[] 	PROGMEM= {"#"};

const char *const keyText[12] PROGMEM = {keys_1, keys_2, keys_3, keys_4, keys_5, keys_6, keys_7, keys_8, keys_9, keys_star, keys_0, keys_hash};

class Keypad {
	public:		
		Keypad(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
		void update();
		void clearText();
		void popText();
		String getText();
		bool textAvailable();
		bool isKeyPressed(uint8_t key);
		void setMode(uint8_t mode);
		uint8_t getMode();
		void setCaplock();
		void setTextLength(uint8_t l);
		char getKey();
	
	private:
		uint8_t row[4], col[4];
		uint8_t rowPressed = 0, colPressed = 0, mode = KEY_MODE;
		uint8_t TEXT_LENGTH = 25;
		unsigned long startTimer = 0;
		char keyPressed = 0;
		uint8_t ptr = 0;
		uint8_t lastRow = 0;
		uint8_t lastCol = 0;
		uint8_t caplock = 0;
		bool newTxt = false;
		bool isPressed = false;
		bool locked = false;
		String txt = "";
};

#endif
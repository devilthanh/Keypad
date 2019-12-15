#include <keypad.h>
#include <Arduino.h>

char buffer[30];

Keypad::Keypad(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4){
	row[0] = r1;
	row[1] = r2;
	row[2] = r3;
	row[3] = r4;
	col[0] = c1;
	col[1] = c2;
	col[2] = c3;
	col[3] = c4;
	
	pinMode(r1, INPUT_PULLUP);
	pinMode(r2, INPUT_PULLUP);
	pinMode(r3, INPUT_PULLUP);
	pinMode(r4, INPUT_PULLUP);
	pinMode(c1, OUTPUT);
	pinMode(c2, OUTPUT);
	pinMode(c3, OUTPUT);
	pinMode(c4, OUTPUT);
	digitalWrite(c1, HIGH);
	digitalWrite(c2, HIGH);
	digitalWrite(c3, HIGH);
	digitalWrite(c4, HIGH);
}

void Keypad::update(){
	if(!isPressed){
		for(uint8_t c=0; c<4; c++){
			for(uint8_t i = 0; i<4; i++)
				if(i==c) digitalWrite(col[i], LOW);
				else digitalWrite(col[i], HIGH);
			
			for(uint8_t r=0; r<4; r++){
				if(digitalRead(row[r])==LOW){
					if((unsigned long)(millis() - startTimer) > SHORT_INTERVAL){
						
						startTimer = millis();
						rowPressed = r;
						colPressed = c;
						isPressed = true;
						keyPressed = pgm_read_byte_near(keys+r*4+c);

						if(mode == TEXT_MODE){
							if(c<=2){
								if(c==2 && r==3){
									setCaplock();
								}else{
																	
									if(lastRow != r || lastCol != c){
										locked = false;
									}
									lastRow = r;
									lastCol = c;
									
									if(!locked){
										if(txt.length() < TEXT_LENGTH){
											//new char
											ptr = 0;
											strcpy_P(buffer, (char *)pgm_read_word(&(keyText[r*3+c])));
											char ch = buffer[ptr];
											if(caplock && ch >=97 && ch <=122){
												ch -= 32;
												txt += ch;
												if(caplock==1) caplock = 0;
											}else txt += ch;
											
											locked = true;
											newTxt = true;
										}
									}else{
										//change char
										ptr++;
										strcpy_P(buffer, (char *)pgm_read_word(&(keyText[r*3+c])));										
										
										if(ptr == strlen(buffer))
											ptr = 0;
										
										if(txt.charAt(txt.length()-1) >=65 && txt.charAt(txt.length()-1)<=90 && caplock == 0)
											caplock = 1;
										
										char ch = buffer[ptr];
										if(caplock && ch >=97 && ch <=122){
											ch -= 32;
											txt.setCharAt(txt.length()-1, ch);
											if(caplock==1) caplock = 0;
										}else txt.setCharAt(txt.length()-1, ch);
										
										newTxt = true;
									}
								}
							}else{
								uint8_t key = r*4+c;
								if(key == TXT_CLEAR){
									popText();
								}
							}
						}else if(mode == NUMBER_MODE){
							uint8_t key = r*4+c;
							if((colPressed <=2 && rowPressed <=2) || (rowPressed == 3 && colPressed == 1)){
								if(txt.length() < NUMBER_LENGTH){
									txt += String(keyPressed);
									newTxt = true;
								}
							}else{
								if(key == TXT_CLEAR){
									popText();
								}
							}
						}
						return;
					}
				}
			}
		}
		
		if(mode == TEXT_MODE)
			if((unsigned long)(millis() - startTimer) > LONG_INTERVAL){
				locked = false;
			}
	}else{
		keyPressed = 0;	
		for(uint8_t i = 0; i<4; i++)
			if(i==colPressed) digitalWrite(col[i], LOW);
			else digitalWrite(col[i], HIGH);
			
		if(digitalRead(row[rowPressed]) == LOW){
			if((unsigned long)(millis() - startTimer) > LONG_INTERVAL){
				if(mode == TEXT_MODE){
					if(colPressed<= 2 && locked){
						txt.setCharAt(txt.length()-1, pgm_read_byte_near(keys + rowPressed*4 + colPressed));
						newTxt = true;
						locked = false;	
						if(caplock==1) caplock = 0;
					}else{
						uint8_t key = rowPressed*4+colPressed;
						if(key == TXT_CLEAR){
							clearText();
						}
					}
				}else if(mode == NUMBER_MODE){
					uint8_t key = rowPressed*4+colPressed;
					if(key == TXT_CLEAR){
						clearText();
					}
				}
			}
		}else{
			rowPressed = 0;
			colPressed = 0;
			startTimer = millis();
			isPressed = false;
		}
		digitalWrite(col[colPressed], HIGH);		
	}
}

bool Keypad::isKeyPressed(uint8_t key){
	uint8_t r = key/4, c = key%4;
	for(uint8_t i = 0; i<4; i++)
		if(i==c) digitalWrite(col[i], LOW);
		else digitalWrite(col[i], HIGH);
		
	if(digitalRead(row[r]) == LOW){
		return true;
	}
	return false;
}

void Keypad::clearText(){
	if(txt.length() != 0) {
		txt = "";
		newTxt = true;
		locked = false;
	}
}

void Keypad::popText(){
	if(txt.length() != 0) {
		txt.remove(txt.length()-1,1);
		newTxt = true;
		locked = false;
	}
}

void Keypad::setMode(uint8_t m){
	if(m && m != mode) txt = "";
	mode = m;
}

uint8_t Keypad::getMode(){
	return mode;
}

void Keypad::setCaplock(){
	caplock++;
	if(caplock == 3) caplock = 0;
	locked = false;
}

bool Keypad::textAvailable(){
	return newTxt;
}

String Keypad::getText(){
	if(newTxt == true){
		newTxt = false;
		return txt;
	}else{
		return "";
	}
}

char Keypad::getKey(){
	return keyPressed;
}

void Keypad::setTextLength(uint8_t l){
	TEXT_LENGTH = l;
}
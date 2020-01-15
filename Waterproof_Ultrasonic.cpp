#include <Waterproof_Ultrasonic.h>


Waterproof_Ultrasonic::Waterproof_Ultrasonic(uint8_t rxPin, uint8_t txPin, uint8_t mode){
	this->txPin = txPin;
	this->rxPin = rxPin;
	this->mode = mode;
	
	if(mode == 1){
		 pinMode(txPin, OUTPUT);
		 pinMode(rxPin, INPUT);
	}else {
		if(uart) delete uart;
		uart = new SoftwareSerial(rxPin, txPin);
		uart->begin(9600);	
		lastTime = 	millis();
	}
}

String Waterproof_Ultrasonic::getR27(){
	if(mode==1){
		return "Empty";
	}else if(mode==2){
		return "47 kOhm";
	}else if(mode==3){
		return "120 kOhm";
	}
}

uint8_t Waterproof_Ultrasonic::getMode(){
	return mode;
}

uint16_t Waterproof_Ultrasonic::read(){
	if(mode == 1){
		digitalWrite(txPin, LOW);
		delayMicroseconds(5);
		digitalWrite(txPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(txPin, LOW);
		duration = pulseIn(rxPin, HIGH);
		return duration*0.034/2;
	}else if(mode == 2){
		if(uart->available()){
			uint8_t buffer[4];
			uart->readBytes(buffer, 4);
			uint16_t sum = buffer[0] + buffer[1] + buffer[2];
			if((uint8_t)sum == buffer[3] && buffer[0] == 0xff){
				uint16_t value = (buffer[1]<<8)+buffer[2];
				return value/10;
			}
		}
	}else if(mode == 3){
		uart->write(0x55);
		uint8_t buffer[4];
		uart->readBytes(buffer, 4);
		uint16_t sum = buffer[0] + buffer[1] + buffer[2];
		if((uint8_t)sum == buffer[3] && buffer[0] == 0xff){
			uint16_t value = (buffer[1]<<8)+buffer[2];
			return value/10;
		}
	}
	return 0;
}

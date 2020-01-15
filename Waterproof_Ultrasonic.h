#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

class Waterproof_Ultrasonic 
{
	public:
		Waterproof_Ultrasonic(uint8_t rxPin, uint8_t txPin, uint8_t mode);
		uint16_t read();
		uint8_t getMode();
		String getR27();
	private:
		uint32_t lastTime = 0;
		uint8_t mode = 1;
		uint8_t rxPin = 0;
		uint8_t txPin = 0;
		long duration;
		SoftwareSerial * uart;
};
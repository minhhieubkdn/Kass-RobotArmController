/*
 Name:		Receiver.ino
 Created:	12/30/2018 1:04:42 PM
 Author:	Mr. H
*/


#include <Servo.h>
#include "RobotArm.h"
#include <SPI.h>
#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>

#define LED_PIN 8

#define INVERT_DIRECTION_1 true
#define INVERT_DIRECTION_2 true
#define INVERT_DIRECTION_3 false
#define INVERT_DIRECTION_4 false

const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 Receiver(9, 10);

struct Data {
	byte Buttons[2] = { 0,0 };
	uint16_t velocity;
	uint16_t alpha;
	int omega;
}data;

struct Point
{
	float x;
	float y;
};

int Velocities[4] = { 0,0,0,0 };   //mm/s

int mAbs(int _num) {
	if (_num < 0) {
		return -_num;
	}
	return _num;
}

//class mMotor {
//public:
//	mMotor() {
//		this->invertedDirection = false;
//		this->vel = 0;
//		this->index = 0;
//	};
//	void setVel(float _vel) {
//		this->vel = _vel;
//		Serial.print("<s");
//		Serial.print(index, DEC);
//		Serial.print(">");
//		Serial.print(this->invertedDirection ? (-this->vel) : (this->vel));
//		Serial.print("</s");
//		Serial.print(index, DEC);
//		Serial.print(">\n");
//	}
//	float getVel() {
//		return this->vel;
//	}
//private:
//	uint8_t index;
//	float vel;
//	bool invertedDirection;
//};

class mRoad {
private:
	boolean isStarting, isMoving, isFinished;
	uint16_t velocity;
	uint16_t distance;
public:
	mRoad() {
		this->isFinished = this->isMoving = this->isStarting = false;
		this->velocity = 0;
		this->distance = 0;
	}
};

void InitRFReceiver()
{
	Receiver.begin();
	Receiver.setAutoAck(1);
	Receiver.setDataRate(RF24_1MBPS);
	Receiver.setChannel(10);
	Receiver.openReadingPipe(1, pipe);
	Receiver.startListening();
}

void CaculateVelocity(uint16_t _vel, uint16_t _alp, int _omega)
{
	int v1, v2, v3, v4;
	double radianAlpha = _alp * PI / 180;

	v1 = _vel * sin(radianAlpha + PI / 4) + _omega;
	v2 = _vel * cos(radianAlpha + PI / 4) + _omega;
	v3 = _vel * cos(radianAlpha + PI / 4) - _omega;
	v4 = _vel * sin(radianAlpha + PI / 4) - _omega;

	Velocities[0] = (INVERT_DIRECTION_1) ? (-v1) : (v1);
	Velocities[1] = (INVERT_DIRECTION_2) ? (-v2) : (v2);
	Velocities[2] = (INVERT_DIRECTION_3) ? (-v3) : (v3);
	Velocities[3] = (INVERT_DIRECTION_4) ? (-v4) : (v4);
}

void SetVelForMotors(int *vel)
{
	String str;

	for (int i = 0; i < 4; i++)
	{
		str = "<s";
		str.concat(i + 1);
		str.concat(">");
		str.concat(vel[i]);
		str.concat("</s");
		str.concat(i + 1);
		str.concat(">");
		Serial.println(str);
	}
}

void setup()
{
	Serial.begin(115200);
	InitRFReceiver();
}

bool zeroSet = true;
void loop() {
	static uint32_t lastReceiveMillis = 0;
	if (Receiver.available())
	{
		digitalWrite(LED_PIN, HIGH);
		Receiver.read(&data, sizeof(data));
		CaculateVelocity(data.velocity, data.alpha, data.omega);
		SetVelForMotors(&Velocities[0]);
		lastReceiveMillis = millis();
		zeroSet = false;
	}
	if(millis() > lastReceiveMillis + 50) {
		digitalWrite(LED_PIN, LOW);
	}
	if (millis() > lastReceiveMillis + 1000 && zeroSet == false) {
		zeroSet = true;
		for (int i = 0; i < 4; i++)
		{
			Velocities[i] = 0;
		}
		SetVelForMotors(&Velocities[0]);
	}
	//debug();
	/*CheckButtonStates();
	CheckButtonState2();*/
}

void debug() {
	if (data.velocity != 0)
	{
		String _str;
		_str = String(data.velocity);
		_str.concat(",");
		_str.concat(data.alpha);
		_str.concat(",");
		_str.concat(data.omega);
		Serial.println(_str);
	}
}

void CheckButtonStates() {
	switch (data.Buttons[0])
	{
	case 1:
		Serial.println("L2"); break;
	case 2:
		Serial.println("R2"); break;
	case 4:
		Serial.println("L1"); break;
	case 8:
		Serial.println("R1"); break;
	case 16:
		Serial.println("green_triangle"); break;
	case 32:
		Serial.println("red_circle"); break;
	case 64:
		Serial.println("blue_X"); break;
	case 128:
		Serial.println("pink_square"); break;
	}
}

void CheckButtonState2() {
	switch (data.Buttons[1])
	{
	case 1:
		Serial.println("Select"); break;
	case 2:
		Serial.println("L3"); break;
	case 4:
		Serial.println("R3"); break;
	case 8:
		Serial.println("Start"); break;
	case 16:
		Serial.println("Up Arrow"); break;
	case 32:
		Serial.println("Right Arrow"); break;
	case 64:
		Serial.println("Down Arrow"); break;
	case 128:
		Serial.println("Left Arrow"); break;
	}
}
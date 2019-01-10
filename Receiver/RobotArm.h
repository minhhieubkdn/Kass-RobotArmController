// RobotArm.h

#ifndef _ROBOTARM_h
#define _ROBOTARM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define LINK_1 140 //mm
#define LINK_2 140

#define SERVO_PIN_1 6
#define SERVO_PIN_2 7
#define SERVO_PIN_3 8

#define PICK_UP_DISTACE 10 //mm

#define HAND_SERVO
#define USED_RELAY

#define ON LOW
#define OFF HIGH

#ifdef HAND_SERVO
#define HAND_SERVO 5
#endif

#ifdef USED_RELAY
#define RELAY_1 4
#define RELAY_2 3
#endif

#define SAFE_HOMING_ANGLE 90

class RobotArmClass
{
private:
	int theta1, theta2, theta3;
	int x, y;
	Servo servo1, servo2, servo3;
	Servo handServo;

public:
	RobotArmClass();
	void init();
	void SetAngleForServos(int _angle_1, int _angle_2);
	void SetAngleForServos(int _angle_1, int _angle_2, int _angle_3);
	void GoToXY(int _x, int _y);
	void GoToXYZ(int _x, int _y, int _z);
	void RotateArm(int _angle);
	void PickUp();
	void PutDown();
	int GetCurrentX();
	int GetCurrentY();
	int GetCurrentZ();
};

extern RobotArmClass RobotArm;

#endif

//
//
//

#include "RobotArm.h"


void RobotArmClass::init()
{
	this->servo_1.attach(SERVO_PIN_1);
	this->servo_2.attach(SERVO_PIN_2);
	this->servo_3.attach(SERVO_PIN_3);

	SetAngleForServos(0, 0, 0);
}

void RobotArmClass::SetAngleForServos(int _angle_1, int _angle_2)
{
	this->servo_1.write(_angle_1);
	this->servo_2.write(_angle_2);
}

void RobotArmClass::SetAngleForServos(int _angle_1, int _angle_2, int _angle_3)
{
	this->servo_1.write(_angle_1);
	this->servo_2.write(_angle_2);
	this->servo_3.write(_angle_3);
}

void RobotArmClass::GoToXY(int _x, int _y)
{
	double K1, K2;
	int _theta1, _theta2;
	K1 = ((_x * _x + _y * _y) - (LINK_1 * LINK_1 + LINK_2 * LINK_2)) / (2 * LINK_1 * LINK_2);
	K2 = (_x * (LINK_1 + LINK_2 * K1) + (_y * LINK_2 * sqrt(1 - K1 * K1))) / (_x*_x + _y * _y);
	//K2 = (_x * (LINK_1 + LINK_2 * K1) - (_y * LINK_2 * sqrt(1 - K1 * K1))) / (_x*_x + _y * _y);
	_theta1 = acos(K1);
	_theta2 = acos(K2);

	this->theta1 = int(_theta1);
	this->theta2 = int(_theta2);

	this->x = _x;
	this->y = _y;

	SetAngleForServos(this->theta1, this->theta2);
}

void RobotArmClass::GoToXYZ(int _x, int _y, int _z)
{
	this->theta3 = _z;
	GoToXY(_x, _y);
	RotateArm(_z);
}

void RobotArmClass::RotateArm(int _angle)
{
	servo_3.write(_angle);
}

int RobotArmClass::GetCurrentX()
{
	return this->x;
}

int RobotArmClass::GetCurrentY()
{
	return this->y;
}

int RobotArmClass::GetCurrentZ()
{
	return this->theta3;
}

void RobotArmClass::PickUp()
{
	this->y += PICK_UP_DISTACE;
	GoToXY(this->x, this->y);
}

void RobotArmClass::PutDown()
{

}

RobotArmClass::RobotArmClass() {
	this->theta1 = 0;
	this->theta2 = 0;
	this->theta3 = 0;
}

RobotArmClass RobotArm;
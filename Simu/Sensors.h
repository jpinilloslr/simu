#pragma once
#include "Chasis.h"
#include "Wheel.h"
#include "Arm.h"
#include "WheelDescriptor.h"
#include <ode\ode.h>

class Sensors
{
private:
	IrrlichtDevice *device;
	Chasis *chasis;
	Wheel **wheels;
	Wheel *wheel;
	Arm   *arm;

	int wheelsDown;

	long nTickFloorContact;

	bool objectIsWheel(dGeomID o1);
	bool objectIsChasis(dGeomID o1);
	bool objectIsArm(dGeomID o1);

public:
	Sensors(IrrlichtDevice *device, Chasis *chasis, Wheel **wheels, Arm *arm);
	~Sensors();
	void setContact(dGeomID o1, dGeomID o2);
	void loop();

	bool isCarOnSide();
	bool isCarDown() ;
	bool isCarFallingRight();
	bool isCarFallingLeft();

	vector3df getGyroscope();
};


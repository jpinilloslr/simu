#pragma once
#include "CarPart.h"
#include "WheelDescriptor.h"
#include "ToolDescriptor.h"

class Chasis :	public CarPart
{
private:
	unsigned int wheelsCount;

	WheelDescriptor *wheelsDesc;
	ToolDescriptor armDesc;

	
public:
	Chasis(IrrlichtDevice *device);
	~Chasis();
	int getWheelsCount() { return wheelsCount; }
	WheelDescriptor *getWheelDescriptor(unsigned int wheelIndex);
	ToolDescriptor *getArmDescriptor();
	void load(char *path);

	dBodyID odeBody;
	dGeomID odeGeom;
	
};



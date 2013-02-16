#pragma once
#include "CarPart.h"
#include "ToolDescriptor.h"

class Arm :
	public CarPart
{
private:
	ToolDescriptor descriptor;

	float anchorX;
	float anchorY;
	float anchorZ;

	float batteryConsume;
public:
	Arm(IrrlichtDevice *device);
	~Arm(void);
	void load(char *path);
	void setToolDescriptor(ToolDescriptor *td);
	ToolDescriptor *getToolDescriptor()        { return &descriptor; }
	float getAnchorX()        { return anchorX; }
	float getAnchorY()        { return anchorY; }
	float getAnchorZ()        { return anchorZ; }

	dBodyID  odeBody;
	dGeomID  odeGeom;
	dJointID odeJoint;	

	void setToNormalPosition();
	void launch();
	bool isInPosition();

	float popBatteryConsume();
};



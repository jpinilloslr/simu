#pragma once
#include "CarPart.h"
#include "WheelDescriptor.h"

class Wheel :public CarPart
{
private:
	WheelDescriptor descriptor;
	ISceneNodeAnimator* rollAnim;

	
public:
	Wheel(IrrlichtDevice *device);
	~Wheel(void);
	void turn(float angle);
	void setSpeed(float speed);
	void setWheelDescriptor(WheelDescriptor *wd);
	WheelDescriptor *getWheelDescriptor()        { return &descriptor; }
	void load(char *path);
	
	dBodyID  odeBody;
	dGeomID  odeGeom;
	dJointID odeJoint;		


};


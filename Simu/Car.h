#pragma once
#include "Chasis.h"
#include "Wheel.h"
#include "Arm.h"
#include "Sensors.h"
#include <ode\ode.h>
#include <windows.h>

#define MAX_BATTERY_LEVEL 100
#define MIN_BATTERY_LEVEL 0

class Car
{
private:	
	IrrlichtDevice *device;

	Arm    *arm;
	Chasis *chasis;
	Wheel **wheels;	
	Sensors *sensors;	

	ITextSceneNode *txtDebug;
	
	float speed;
	bool  goBack;
	float maxSpeed;
	float direction;	
	float acceleration;
	bool  unlockBWheels;
	float batteryLevel;
	
	long nTickBalance;
	long nTickPullUp;
	
	void friction();
	void speedAndDirectionControl();
	void unlockBackWheels();
	void lockBackWheels();

	void timerBalance();
	void timePullUp();
	void updateDebugLabel();
public:
	bool showDebugLabel;
	dSpaceID odeSpace;
	Car(IrrlichtDevice *device);
	~Car(void);
	void load();
	IMeshSceneNode *getNode() { return chasis->getNode(); }

	Chasis *getChasis() { return chasis; }
	Wheel **getWheels() { return wheels; }
	Arm    *getArm()    { return arm;    }

	void loop();
	void updateMotorAndDirection();

	void speedUp();
	void speedDown();	
	void goTo(vector3df position, float distance);
	void goAwayFrom(vector3df position, float distance);	
	void avoid(vector3df position, float distance);	

	void  setDirection(float d);
	float getDirection()               { return direction; }
	float getSpeed()                   { return speed; }
	void setSpeed(float speed)         { this->speed = speed; }
	Sensors *getSensors()              { return sensors; }

	void setBatteryLevel(float level);
	float getBatteryLevel();
	void  consumeBattery(float v)      { batteryLevel -= v; if(batteryLevel < MIN_BATTERY_LEVEL) batteryLevel = MIN_BATTERY_LEVEL; }
	void  chargeBattery(float v)       { batteryLevel += v; if(batteryLevel > MAX_BATTERY_LEVEL) batteryLevel = MAX_BATTERY_LEVEL; }

};


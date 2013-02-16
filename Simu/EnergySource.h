#pragma once
#include "SimuObject.h"

#define MAX_SOURCE_BATTERY_LEVEL 100
#define MIN_SOURCE_BATTERY_LEVEL 0

class EnergySource : public SimuObject
{
private:
	ITextSceneNode *txtDebug;

	float batteryLevel;
	void updateDebugLabel();
public:
	bool showDebugLabel;
	EnergySource(IrrlichtDevice *device);
	~EnergySource();
	void load(char *path);
	void loop();

	dBodyID  odeBody;
	dGeomID  odeGeom;

	float extractBatteryPower();
	float getBatteryLevel()  { return batteryLevel; }
};


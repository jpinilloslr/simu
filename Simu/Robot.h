#pragma once
#include "car.h"
#include "EnergySource.h"
#include "PerceptionField.h"

#define MAX_PERCEPTION_ELEMENTS 10

enum Motivation { mot_chargebat, mot_none };

class Robot : public Car
{
private:
	IrrlichtDevice *device;
	
	PerceptionField *perceptionField;
	float perceptionRange;
	Motivation motivation;

	void processPerception();
	void processMotivation();

	void goToChargeBattery(EnergySource *source);

	void search();
	vector3df searchPos;
	bool searching;
public:
	Robot(IrrlichtDevice *device);
	~Robot();

	PerceptionField *getPerceptionField() { return perceptionField; }
	float getPerceptionRange()            { return perceptionRange; }

	void loop();
};


#include "Robot.h"

void Robot::processPerception()
{
	bool bCameraFound = false;
	bool bSourceFound = false;

	for(unsigned int i=0; i<perceptionField->getPerceptionsCount(); i++)
	{
		if(perceptionField->getPerception(i)->type == pt_robot)
		{
			Robot *robot = (Robot *)perceptionField->getPerception(i)->pointer;
			goAwayFrom(robot->getNode()->getPosition(), 10.0f);
		}
		else
		if(perceptionField->getPerception(i)->type == pt_camera)
		{
			if(motivation == mot_none)
			{
				searching = false;
				ICameraSceneNode *camera = (ICameraSceneNode *)perceptionField->getPerception(i)->pointer;
				goTo(camera->getPosition(), 5.0f);
				bCameraFound = true;				
			}
		}
		else
		if(perceptionField->getPerception(i)->type == pt_energysource)
		{
			if(motivation == mot_chargebat)
			{
				EnergySource *source = (EnergySource *)perceptionField->getPerception(i)->pointer;

				if(source->getBatteryLevel() > MIN_SOURCE_BATTERY_LEVEL)
				{
					searching = false;
					bSourceFound = true;

					if(source->getNode()->getPosition().getDistanceFrom(getNode()->getPosition()) > 7.0f)
					{
						goTo(source->getNode()->getPosition(), 7.0f);
					}
					else
					{
						chargeBattery(source->extractBatteryPower());
					}
				}
			}
			else
			{
				EnergySource *source = (EnergySource *)perceptionField->getPerception(i)->pointer;
				goAwayFrom(source->getNode()->getPosition(), 10.0f);
			}
		}
	}

	if((motivation == mot_none) && (!bCameraFound))
	{
		search();
	}
	if((motivation == mot_chargebat) && (!bSourceFound))
	{
		search();
	}
}

void Robot::search()
{
	if(!searching)
	{
		searching = true;
		searchPos.X = (float)rand() / (RAND_MAX + 1) * (100.0f - -100.0f) + -100.0f;
		searchPos.Z = (float)rand() / (RAND_MAX + 1) * (100.0f - -100.0f) + -100.0f; 
		searchPos.Y = 0.0f;
		
	}

	goTo(searchPos, 5.0f);
	float d = getChasis()->getDistanceTo(searchPos);
	if(d < 10.0f)
		searching = false;
}

void Robot::processMotivation()
{
	if((getBatteryLevel() < 80.0f) && (motivation != mot_chargebat))
	{
		motivation = mot_chargebat;
	}
	else
	if((getBatteryLevel() == 100.0f) && (motivation != mot_none))
	{
		motivation = mot_none;
	}
}

Robot::Robot(IrrlichtDevice *device) : Car(device)
{
	perceptionField = new PerceptionField();
	perceptionRange = 100.0f;
	searching = false;
}

Robot::~Robot()
{
	delete perceptionField;
}

void Robot::loop()
{
	processPerception();
	processMotivation();
	Car::loop();
}
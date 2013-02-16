#include "Sensors.h"


Sensors::Sensors(IrrlichtDevice *device, Chasis *chasis, Wheel **wheels, Arm *arm)
{
	this->chasis = chasis;
	this->wheels = wheels;
	this->arm = arm;
	this->device = device;

	nTickFloorContact = device->getTimer()->getRealTime();
}


Sensors::~Sensors(void)
{
}

void Sensors::setContact(dGeomID o1, dGeomID o2)
{
	if(objectIsWheel(o1) && (o2 == NULL))
	{
		wheel->setFloorContact(true);
	}

	if(objectIsChasis(o1) && (o2 == NULL))
	{
		chasis->setFloorContact(true);
	}
}

bool Sensors::objectIsWheel(dGeomID o1)
{
	int i=0;
	bool found = false;

	while((!found) && (i<chasis->getWheelsCount()))
	{
		wheel = wheels[i];
		found = (wheel->odeGeom == o1);
		i++;
	}
	return found;
}

bool Sensors::objectIsChasis(dGeomID o1)
{
	return (chasis->odeGeom == o1);
}

bool Sensors::objectIsArm(dGeomID o1)
{
	return (arm->odeGeom == o1);
}

void Sensors::loop()
{
	/*if(device->getTimer()->getRealTime() - nTickFloorContact > 1000)
	{
		wheelsDown = 0;
		for(int i=0; i<chasis->getWheelsCount(); i++)
		{
			if(wheels[i]->getFloorContact())
				wheelsDown++;
			
			wheels[i]->setFloorContact(false);
		}

		if(!arm->isInPosition())
			arm->setToNormalPosition();

		nTickFloorContact = device->getTimer()->getRealTime();
	}*/
}

bool Sensors::isCarOnSide() 
{ 
	return ((getGyroscope().Y > 0.25f) && (getGyroscope().Y <= 0.8f)); 
}

bool Sensors::isCarDown()   
{ 
	return ((getGyroscope().Y > 0.8f) && (getGyroscope().Y <= 1.0f)); 
} 

vector3df Sensors::getGyroscope()
{
	vector3df vect1, vect2, normal;
	vect1 = wheels[3]->getNode()->getPosition() - wheels[0]->getNode()->getPosition();
	vect2 = wheels[5]->getNode()->getPosition() - wheels[0]->getNode()->getPosition();

	normal = vect1.crossProduct(vect2);
	normal.rotateXZBy(chasis->getAbsoluteYAngle(), vector3df(0.0f, 0.0f, 0.0f));
	normal.normalize();
	
	return normal;
}
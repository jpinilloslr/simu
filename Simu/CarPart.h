#pragma once
#include <irrlicht.h>
#include <ode\ode.h>
#include "SimuObject.h"

class CarPart : public SimuObject
{
protected:
	bool floorContact; 
	
public:
	CarPart(IrrlichtDevice *device);
	~CarPart();

	void setFloorContact(bool contact) {  floorContact = contact;  }
	bool getFloorContact()             {  return floorContact;  }

};


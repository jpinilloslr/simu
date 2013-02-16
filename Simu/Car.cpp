#include "Car.h"


void Car::friction()
{
	if(speed > 0)
		speed -= 0.009f;
	else
	if(speed < 0)
		speed += 0.009f;

	if((speed < 0.09f) && (speed > -0.09f)) speed = 0.0f;
}

void Car::speedAndDirectionControl()
{
	if(speed > maxSpeed)
		speed = maxSpeed;
	if(speed < -maxSpeed)
		speed = -maxSpeed;

	if(direction > wheels[0]->getWheelDescriptor()->maxRotation)
		direction = wheels[0]->getWheelDescriptor()->maxRotation;
	if(direction < -wheels[0]->getWheelDescriptor()->maxRotation)
		direction = -wheels[0]->getWheelDescriptor()->maxRotation;

	if(direction > 0.0f)
		direction -= 0.1f;
	if(direction < 0.0f)
		direction += 0.1f;
}

void Car::timerBalance()
{
	if((batteryLevel > MIN_BATTERY_LEVEL) &&  (device->getTimer()->getRealTime() - nTickBalance > 500))
	{
		if((sensors->getGyroscope().Z < -0.5f) )
		{
			speed = maxSpeed * (sensors->getGyroscope().Z+0.2f);
			arm->launch();
		}
		else
		if((sensors->getGyroscope().Z > 0.5f) )
		{
			speed = maxSpeed * (sensors->getGyroscope().Z-0.2f);
		}

		if(sensors->getGyroscope().X > 0.3f)
		{
			speed = -maxSpeed * (sensors->getGyroscope().Z-0.5f);
			unlockBackWheels();
		}
		else
		if(sensors->getGyroscope().X < -0.3f)
		{
			speed = maxSpeed * (sensors->getGyroscope().Z+0.5f);
			unlockBackWheels();
		}
		else
			lockBackWheels();
		nTickBalance = device->getTimer()->getRealTime();
	}
}

void Car::timePullUp()
{
	if((batteryLevel > MIN_BATTERY_LEVEL) && (device->getTimer()->getRealTime() - nTickPullUp > 3000))
	{
		if(sensors->isCarDown()) 
		{
			if(!arm->isInPosition())
				arm->setToNormalPosition();
			else
				arm->launch();
		}
		else
		if(sensors->isCarOnSide()) 
		{
			dBodyAddTorque(chasis->odeBody, 100.0f, 0.0f, 100.0f);
			if(!arm->isInPosition())
				arm->setToNormalPosition();
		}
		else
		{
			if(!arm->isInPosition())
				arm->setToNormalPosition();
		}		


		nTickPullUp = device->getTimer()->getRealTime();
	}
}

void Car::updateDebugLabel()
{
	if(NULL != txtDebug)
	{
		vector3df pos = getNode()->getPosition();
		pos.Y = chasis->getSizeY() * 2.0f;
		txtDebug->setPosition(pos);

		irr::core::stringw text;
		irr::core::stringw value;
		text = "bat: ";
		value += batteryLevel;
		
		value = value.subString(0, value.findFirst('.')+2);
		text = text + value + "%";

		txtDebug->setText(text.c_str());
	}
}

Car::Car(IrrlichtDevice *device)
{
	this->device = device;

	speed        = 0.0f;
	direction    = 0.0f;	
	goBack       = false;
	maxSpeed     = 7.0f;
	acceleration = 0.1f;

	showDebugLabel = false;
	txtDebug       = NULL;
	chasis         = NULL;
	batteryLevel   = MAX_BATTERY_LEVEL;

	nTickBalance = nTickPullUp  = device->getTimer()->getRealTime();
}

Car::~Car(void)
{
	for(int i=0; i<chasis->getWheelsCount(); i++)
	{
		delete wheels[i];
	}

	delete[] wheels;
	delete arm;
	delete chasis;
	delete sensors;
}

void Car::load()
{
	chasis = new Chasis(device);
	chasis->load("data/robots/chasis/chasis_001/");

	wheels = new Wheel*[chasis->getWheelsCount()];			

	for(int i=0; i<chasis->getWheelsCount(); i++)
	{
		wheels[i] = new Wheel(device);
		wheels[i]->load("data/robots/wheel/wheel_001/");

		wheels[i]->setWheelDescriptor(chasis->getWheelDescriptor(i));
	}

	arm = new Arm(device);
	arm->load("data/robots/tool/tool_001/");
	arm->setToolDescriptor(chasis->getArmDescriptor());

	sensors = new Sensors(device, chasis, wheels, arm);

	if(showDebugLabel)
	{
		txtDebug = device->getSceneManager()->addTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"", SColor(255, 255, 255, 255));
	}
}

void Car::loop() 
{
	if(GetKeyState('W')!=0 && GetKeyState('W')!=1)
		speed += 0.1f;
	if(GetKeyState('S')!=0 && GetKeyState('S')!=1)
		speed -= 0.1f;
	if(GetKeyState('A')!=0 && GetKeyState('A')!=1)
		direction += 0.2f;
	if(GetKeyState('D')!=0 && GetKeyState('D')!=1)
		direction -= 0.2f;
	if(GetKeyState(VK_SPACE)!=0 && GetKeyState(VK_SPACE)!=1)
		speed = 0.0f;
	if(GetKeyState('X')!=0 && GetKeyState('X')!=1)
		dBodyAddTorque(chasis->odeBody, -20.0f, 0.0f, 0.0f);
	if(GetKeyState('Z')!=0 && GetKeyState('Z')!=1)
		dBodyAddTorque(chasis->odeBody, 0.0f, 0.0f, 20.0f);

	friction();
	
	timerBalance();
	timePullUp();
	
	speedAndDirectionControl();
	sensors->loop();
	updateMotorAndDirection();
	updateDebugLabel();
	consumeBattery(arm->popBatteryConsume());
	consumeBattery(0.001);
}

void Car::speedUp()
{
	if(getBatteryLevel() > MIN_BATTERY_LEVEL)
	{
		consumeBattery(0.001f);
		speed += acceleration;

		if((goBack) && (speed > 0))
			speed *= -1;
	}
}

void Car::speedDown()
{
	if(getBatteryLevel() > MIN_BATTERY_LEVEL)
	{
		consumeBattery(0.001f);
		speed -= acceleration;

		if((goBack) && (speed > 0))
			speed *= -1;
	}
}

void Car::setDirection(float d)
{
	if(getBatteryLevel() > MIN_BATTERY_LEVEL)
	{
		consumeBattery(0.0001f * abs(direction - d));
		direction = d;
	}
}

void Car::goTo(vector3df position, float distance)
{	
	float currentDistance = chasis->getDistanceTo(position);
	float angle = chasis->getAngleTo(position);

	if(currentDistance > distance)
	{		
		if(speed < currentDistance - distance)
			speedUp();
		else
		if(speed > currentDistance - distance)
			speedDown();

		if((angle > 2.0f) && (angle < 358.0f))
		{
			if(angle < 180.0f)
				setDirection(angle/100.0f);
			else
				setDirection(-angle/100.0f);
		}
	} 	
}

void Car::goAwayFrom(vector3df position, float distance)
{	
	float currentDistance = chasis->getDistanceTo(position);
	float angle = chasis->getAngleTo(position);

	if(currentDistance < distance)
	{
		float size = getChasis()->getSizeX();
		if(getChasis()->getSizeZ() > size)
			size = getChasis()->getSizeZ();

		speedUp();

		if(((angle < 178.0f) || (angle > 182.0f))) 
		{
			if(angle < 180.0f)
				setDirection(-angle/100.0f);
			else
				setDirection(angle/100.0f);
		}

		/*if(currentDistance <= size)
			goBack = true;*/
	} 	
}

void Car::avoid(vector3df position, float distance)
{	
	float currentDistance = chasis->getDistanceTo(position);
	float angle = chasis->getAngleTo(position);

	if((currentDistance < distance) && (speed > 0.0f))
	{		
		if((angle < 45.0f) || (angle > 315.0f))
		{
			/*if(speed > acceleration*2)
				speedDown();*/

			if(angle < 35.0f)
				setDirection(-5.0f);
			else
				setDirection(5.0f);
		}
	} 	
}

void Car::updateMotorAndDirection()
{
	for(int i=0; i< getChasis()->getWheelsCount(); i++)
	{
		if(getWheels()[i]->getWheelDescriptor()->motor)
		{
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamVel2, getSpeed());
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamFMax2, 1.0f);
		}

		if(getWheels()[i]->getWheelDescriptor()->canRotate)
		{
			dReal v = getDirection() - dJointGetHinge2Angle1(getWheels()[i]->odeJoint);
			
			if (v > 0.1) v = 0.1f;
			if (v < -0.1) v = -0.1f;
			
			v *= 5.0;

			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamVel, v);
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamFMax, 1.0f);
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamLoStop, -getWheels()[i]->getWheelDescriptor()->maxRotation);
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamHiStop, getWheels()[i]->getWheelDescriptor()->maxRotation);
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamFudgeFactor, 0.1f);
		}
	}
}

void Car::unlockBackWheels()
{
	unlockBWheels = true;
	consumeBattery(0.0001f);

	for (int i=0; i<getChasis()->getWheelsCount(); i++) 
	{
		if(!getWheels()[i]->getWheelDescriptor()->canRotate)
		{
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamLoStop, 0.0f / (180.0f/irr::core::PI));
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamHiStop, 90.0f / (180.0f/irr::core::PI));
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamVel, 1.0);
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamFMax, 1.0f);
		}
	}
}

void Car::lockBackWheels()
{
	unlockBWheels = false;
	consumeBattery(0.0001f);

	for (int i=0; i<getChasis()->getWheelsCount(); i++) 
	{
		if(!getWheels()[i]->getWheelDescriptor()->canRotate)
		{
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamLoStop, 0);
			dJointSetHinge2Param (getWheels()[i]->odeJoint, dParamHiStop, 0);
		}
	}
}

void Car::setBatteryLevel(float level)
{
	if(level > MAX_BATTERY_LEVEL)
		batteryLevel = MAX_BATTERY_LEVEL;
	else
	if(level < MIN_BATTERY_LEVEL)
		batteryLevel = MIN_BATTERY_LEVEL;
	else
		batteryLevel = level;
}

float Car::getBatteryLevel()
{
	return batteryLevel;
}
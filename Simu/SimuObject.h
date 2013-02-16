#pragma once
#include <irrlicht.h>
#include <ode\ode.h>

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;

class SimuObject
{
protected:
	IrrlichtDevice *device;
	ISceneManager  *scenemgr;
	IMeshSceneNode *node;

	float sizeX;
	float sizeY;
	float sizeZ;
	float radius;
	float weight;

	vector3df collisionBoxTranslation;
	ISceneNode *collisionBox;
	ISceneNode *collisionSphere;

	float absoluteYAngle;
public:
	SimuObject(IrrlichtDevice *device);
	~SimuObject();
	void load(char *part);
	IMeshSceneNode *getNode() { return node; }
	float getDistanceTo(vector3df position);
	float getAngleTo(vector3df position);

	float getSizeX()  { return sizeX; }
	float getSizeY()  { return sizeY; }
	float getSizeZ()  { return sizeZ; }
	float getRadius() { return radius; }
	float getWeight() { return weight; }

	void loadCollisionBox();
	void updateCollisionBox();
	void loadCollisionSphere();
	void updateCollisionSphere();


	void  setAbsoluteYAngle(float angle) {  absoluteYAngle = angle;  }
	float getAbsoluteYAngle()            {  return absoluteYAngle;  }
};


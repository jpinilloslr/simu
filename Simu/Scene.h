#pragma once

#include <irrlicht.h>
#include "physics.h"
#include "EnergySource.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;

#define MAX_ENERGY_SOURCE 1

class Scene
{
private:
	IrrlichtDevice *device;
	ISceneManager  *scenemgr;
	ICameraSceneNode *camera;
	IMeshSceneNode* node;

	EnergySource *sources[MAX_ENERGY_SOURCE];

public:
	Scene(IrrlichtDevice *device);
	~Scene();
	void load(char *path);
	void initPhysics();
	void stopPhysics();
	void loop();
	IMeshSceneNode *getNode() { return node; }
	ICameraSceneNode *getCamera() { return camera; }

	int getEnergySourcesCount()  { return MAX_ENERGY_SOURCE; }
	EnergySource *getEnergySource(int index) { if(index < MAX_ENERGY_SOURCE) return sources[index]; else return NULL; }


};


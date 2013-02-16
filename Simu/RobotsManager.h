#pragma once
#include "Robot.h"
#include "Scene.h"
#include "physics.h"
#include "Perceptible.h"

class RobotsManager
{
private:
	int robotsCount;
	Robot **robots;
	Scene *scene;

	IrrlichtDevice *device;
public:
	RobotsManager(IrrlichtDevice *device, Scene *scene);
	~RobotsManager();
	void initialize(int robotsCount);
	void loop();
	void stopPhysics();
	Robot *getRobot(int i) { if(i<robotsCount) return robots[i]; else return NULL; }
};


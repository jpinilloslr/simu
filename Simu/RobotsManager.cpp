#include "RobotsManager.h"


RobotsManager::RobotsManager(IrrlichtDevice *device, Scene *scene)
{
	this->device = device;
	this->scene = scene;
	robotsCount = 0;
}


RobotsManager::~RobotsManager()
{
	for(int i=0; i<robotsCount; i++)
	{
		delete robots[i];
	}

	delete[] robots;
}

void RobotsManager::initialize(int robotsCount)
{
	this->robotsCount = robotsCount;

	robots = new Robot*[robotsCount];

	for(int i=0; i<robotsCount; i++)
	{
		robots[i] = new Robot(device);
		robots[i]->showDebugLabel = true;
		robots[i]->load();
		robots[i]->getChasis()->getNode()->setPosition(vector3df(i * 10.0f, 1.0f, 10.0f));
		initCarPhysics(robots[i]);		
	}
}

void RobotsManager::loop()
{
	for(int i=0; i<robotsCount; i++)
	{
		robots[i]->loop();		
		updateCar(robots[i]);
		robots[i]->getPerceptionField()->clearPerceptions();

		//percibe la camara
		if(robots[i]->getNode()->getPosition().getDistanceFrom(device->getSceneManager()->getActiveCamera()->getPosition()) <= robots[i]->getPerceptionRange())
		{
			Perceptible perception;
			perception.pointer = (void *)device->getSceneManager()->getActiveCamera();
			perception.type    = pt_camera;

			robots[i]->getPerceptionField()->addPerception(&perception);
		}

		//percibe otros robots
		for(int j=0; j<robotsCount; j++)
		{
			if(j != i)
			{
				vector3df p1 = robots[i]->getNode()->getPosition();
				vector3df p2 = robots[j]->getNode()->getPosition();

				if(p1.getDistanceFrom(p2) <= robots[i]->getPerceptionRange())
				{
					Perceptible perception;
					perception.pointer = (void *)robots[j];
					perception.type    = pt_robot;

					robots[i]->getPerceptionField()->addPerception(&perception);
				}
			}
		}

		//percibe fuentes de energia
		for(int j=0; j<scene->getEnergySourcesCount(); j++)
		{
			vector3df p1 = robots[i]->getNode()->getPosition();
			vector3df p2 = scene->getEnergySource(j)->getNode()->getPosition();

			if(p1.getDistanceFrom(p2) <= robots[i]->getPerceptionRange())
			{
				Perceptible perception;
				perception.pointer = (void *)scene->getEnergySource(j);
				perception.type    = pt_energysource;

				robots[i]->getPerceptionField()->addPerception(&perception);
			}
		}
	}
}

void RobotsManager::stopPhysics()
{
	for(int i=0; i<robotsCount; i++)
	{
		stopCarPhysics(robots[i]);
	}
}
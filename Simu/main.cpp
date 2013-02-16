#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <irrlicht.h>
#include "physics.h"
#include "Scene.h"
#include "RobotsManager.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;

#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "ode_single.lib")

IrrlichtDevice *device;
IVideoDriver   *driver;
ISceneManager  *scenemgr;

RobotsManager *robotsManager;
Scene *testScene;

bool startIrrlicht()
{
	device = createDevice(EDT_DIRECT3D9, dimension2d<u32>(1024, 768), 32, false, true, true, NULL);

	if(NULL == device)
		return false;

	driver = device->getVideoDriver();
	scenemgr = device->getSceneManager();

	device->setWindowCaption(L"Simu");
	return true;
}

void setCamera(Car *car)
{
	vector3df position;
	vector3df camPosition;

	position = car->getNode()->getPosition();
	camPosition = position;


	float actualAngleInRadians;
	actualAngleInRadians = ( robotsManager->getRobot(0)->getChasis()->getAbsoluteYAngle() * 3.1415f / 180.0f );
	camPosition.X -= sin( actualAngleInRadians ) *  -10.0f;
	camPosition.Z -= cos( actualAngleInRadians ) *  -10.0f;
	

	testScene->getCamera()->setPosition(camPosition);
	testScene->getCamera()->setTarget(position);
}

void loop()
{
	device->getCursorControl()->setVisible(false);

	while(device->run())
	{		
		if (device->isWindowActive())
		{			
			driver->beginScene(true, true, SColor(255,100,101,140));
			scenemgr->drawAll();
			driver->endScene();		
			
			core::stringw str = L"Simu [";
			str += driver->getName();
			str += "] Primitives count:";
			str += driver->getPrimitiveCountDrawn();
			str += " FPS: ";
			str += driver->getFPS();
			str += " speed: ";
			str += robotsManager->getRobot(0)->getSpeed();
			
			device->setWindowCaption(str.c_str());
			//setCamera(carsManager->getCar(0));
			
			robotsManager->loop();
			testScene->loop();

			simLoop(false);
		}
	}
}

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	startIrrlicht();
	
	testScene = new Scene(device);
	robotsManager = new RobotsManager(device, testScene);

	testScene->load("data/scenes/scene_001");	

	startOde();
	testScene->initPhysics();	
	robotsManager->initialize(7);

	loop();	

	robotsManager->stopPhysics();
	testScene->stopPhysics();
	stopOde();
	delete robotsManager;
	delete testScene;
	return 0;
}

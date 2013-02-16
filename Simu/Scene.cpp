#include "Scene.h"


Scene::Scene(IrrlichtDevice *device)
{
	this->device = device;
	if(NULL != device)
		this->scenemgr = device->getSceneManager();

	for(int i=0; i<MAX_ENERGY_SOURCE; i++)
	{
		sources[i] = new EnergySource(device);
	}
}

Scene::~Scene()
{
	getNode()->drop();
}

void Scene::load(char *path)
{
	char szFile[255];

	strncpy_s(szFile, path, 255);
	strcat_s(szFile, "/scene.3ds");

	IAnimatedMesh* mesh = scenemgr->getMesh(szFile);
	node = scenemgr->addOctreeSceneNode(mesh);

	scenemgr->addLightSceneNode(0, core::vector3df(220, 200, 112),	video::SColorf(1.0f, 1.0f, 1.0f,1.0f), 400.0f);
	scenemgr->addLightSceneNode(0, core::vector3df(294, 200, -104),	video::SColorf(1.0f, 1.0f, 1.0f,1.0f), 400.0f);

	ITriangleSelector* selector = 0;

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, true);
		selector = scenemgr->createOctreeTriangleSelector(mesh->getMesh(0), node, 32);
		node->setTriangleSelector(selector);		
		node->setPosition(vector3df(0.0f, 0.0f, 0.0f));

		node->getMaterial(0).Shininess = 0.0f;
	}

	camera = scenemgr->addCameraSceneNodeFPS(0, 50.0f, 0.02f);
	camera->setPosition(vector3df(-10.0f, 5.0f, 0.0f));
	camera->setFarValue(1400.0f);
	camera->setNearValue(0.1f);

	ISceneNodeAnimator* anim = scenemgr->createCollisionResponseAnimator(selector, camera, vector3df(2.0f, 2.0f, 2.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f));
	//camera->addAnimator(anim);

	strncpy_s(szFile, path, 255);
	irr::core::stringc strPath = szFile;	

	ISceneNode *SkyBox = scenemgr->addSkyBoxSceneNode(
		device->getVideoDriver()->getTexture(strPath + "/skybox1_up.jpg"),
        device->getVideoDriver()->getTexture(strPath + "/skybox1_dn.jpg"),
        device->getVideoDriver()->getTexture(strPath + "/skybox1_rt.jpg"),
        device->getVideoDriver()->getTexture(strPath + "/skybox1_lf.jpg"),
        device->getVideoDriver()->getTexture(strPath + "/skybox1_ft.jpg"),
        device->getVideoDriver()->getTexture(strPath + "/skybox1_bk.jpg"));
	
	anim->drop();
	selector->drop();

	vector3df position;

	for(int i=0; i<MAX_ENERGY_SOURCE; i++)
	{
		sources[i]->showDebugLabel = true;
		sources[i]->load("data/common/energysrc/");

		position.X = (float)rand() / (RAND_MAX + 1) * (100.0f - -100.0f) + -100.0f;
		position.Z = (float)rand() / (RAND_MAX + 1) * (100.0f - -100.0f) + -100.0f;
		position.Y = 5.1f;

		sources[i]->getNode()->setPosition(position);
	}
}

void Scene::initPhysics()
{
	setScenePhysics(this->getNode());

	for(int i=0; i<getEnergySourcesCount(); i++)
	{
		initEnergySourcePhysics(getEnergySource(i));
	}
}

void Scene::stopPhysics()
{
	for(int i=0; i<getEnergySourcesCount(); i++)
		stopEnergySourcePhysics(getEnergySource(i));
}


void Scene::loop()
{
	for(int i=0; i<getEnergySourcesCount(); i++)
	{
		updateEnergySource(getEnergySource(i));
		getEnergySource(i)->loop();
	}
}
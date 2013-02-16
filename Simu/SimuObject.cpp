#include "SimuObject.h"


float SimuObject::getDistanceTo(vector3df position)
{
	vector3df currentPos = getNode()->getPosition();
	return sqrtf(powf(currentPos.X - position.X, 2.0f) + powf(currentPos.Y - position.Y, 2.0f) + powf(currentPos.Z - position.Z, 2.0f));
}

float SimuObject::getAngleTo(vector3df position)
{
	vector3df currentPos = getNode()->getPosition();
	float distX, distY, dist, angle;
	distX = position.X - currentPos.X;
	distY = position.Z - currentPos.Z;

	dist = sqrtf(powf(distY, 2.0f) + powf(distX, 2.0f));

	float sin = distY / dist;
	float cos = distX / dist;

	if ((sin > 1) || (sin < -1) || (cos > 1) || (cos < -1))
		return 0;  

	float arcsin = asin(sin) * (180.0f/3.1415f);
	float arccos = acos(cos) * (180.0f/3.1415f);

	if(arcsin >= 0)
	{
		angle = arccos;
	} else
	{
		angle = 360.0f - arccos;
	}

	float currentAngle = absoluteYAngle + 90.0f;
	while(currentAngle > 360.0f)
		currentAngle -= 360.0f;
	while(currentAngle < 0.0f)
		currentAngle += 360.0f;

	angle += currentAngle;
	while(angle > 360.0f)
		angle -= 360.0f;
	while(angle < 0.0f)
		angle += 360.0f;

	return angle;
}

SimuObject::SimuObject(IrrlichtDevice *device)
{
	this->device = device;
	if(NULL != device)
		this->scenemgr = device->getSceneManager();
}


SimuObject::~SimuObject()
{
	getNode()->drop();
}

void SimuObject::load(char *part)
{
	IAnimatedMesh* mesh = scenemgr->getMesh(part);
	node = scenemgr->addMeshSceneNode(mesh);
	//node->setMaterialFlag(EMF_ANTI_ALIASING, true);
}


void SimuObject::loadCollisionBox()
{
	collisionBox = device->getSceneManager()->addCubeSceneNode(1.0f);	
	collisionBox->setScale(vector3df(getSizeX(), getSizeY(), getSizeZ()));
	collisionBox->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
}

void SimuObject::updateCollisionBox()
{
	vector3df pos = getNode()->getPosition();	
	collisionBox->setRotation(getNode()->getRotation());
	collisionBox->setPosition(pos);
}

void SimuObject::loadCollisionSphere()
{
	collisionSphere = device->getSceneManager()->addSphereSceneNode(getRadius());
}

void SimuObject::updateCollisionSphere()
{
	vector3df pos = getNode()->getPosition();	
	collisionSphere->setRotation(getNode()->getRotation());
	collisionSphere->setPosition(pos);
}

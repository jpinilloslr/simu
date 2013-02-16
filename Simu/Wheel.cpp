#include "Wheel.h"


Wheel::Wheel(IrrlichtDevice *device): CarPart(device)
{
	radius = 0.0f;
	weight = 0.0f;
}


Wheel::~Wheel(void)
{
	
}

void Wheel::turn(float angle)
{	
	if(angle > 35.0f)
		angle = 35.0f;

	if(angle < -35.0f)
		angle = -35.0f;

	if(descriptor.canRotate)
	{
		vector3df rotation = getNode()->getRotation();
		getNode()->setRotation(vector3df(rotation.X, angle + descriptor.initialRotation, rotation.Z));
	}
}

void Wheel::setWheelDescriptor(WheelDescriptor *wd) 
{ 
	memcpy(&descriptor, wd, sizeof(WheelDescriptor)); 
	getNode()->setPosition(vector3df(descriptor.posX, descriptor.posY, descriptor.posZ));
	getNode()->setRotation(vector3df(0.0f, descriptor.initialRotation, 0.0f));
}

void Wheel::setSpeed(float speed)
{
	float factor;
	vector3df rotation = getNode()->getRotation();

	factor = -100.0f;

	if(descriptor.initialRotation != 0)
		factor = 100.0f;

	getNode()->setRotation(vector3df(rotation.X + speed*factor, rotation.Y, rotation.Z));
	
}

void Wheel::load(char *path)
{
	char szModelFile[255];
	char szXMLFile[255];

	strncpy_s(szModelFile, path, 255);
	strcat_s(szModelFile, "/wheel.obj");

	strncpy_s(szXMLFile, path, 255);
	strcat_s(szXMLFile, "/wheel.xml");

	io::IXMLReader* xml = device->getFileSystem()->createXMLReader(szXMLFile);
	core::stringc value;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
			case io::EXN_ELEMENT:
            {
                if (core::stringw("wheel") == xml->getNodeName()) 
				{
                    value = xml->getAttributeValue(L"radius");
					radius = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"weight");
					weight = (float)atof(value.c_str());
				}
            }
            break;
        default:
            break;
        }
    }

    if (xml)
        xml->drop(); 

	CarPart::load(szModelFile);
	
}
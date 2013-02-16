#include "EnergySource.h"


EnergySource::EnergySource(IrrlichtDevice *device) : SimuObject(device)
{
	showDebugLabel = false;
	txtDebug       = NULL;
	batteryLevel   = MAX_SOURCE_BATTERY_LEVEL;
}

EnergySource::~EnergySource()
{
}

void EnergySource::load(char *path)
{
	char szModelFile[255];
	char szXMLFile[255];

	strncpy_s(szModelFile, path, 255);
	strcat_s(szModelFile, "/energysrc.obj");

	strncpy_s(szXMLFile, path, 255);
	strcat_s(szXMLFile, "/energysrc.xml");

	io::IXMLReader* xml = device->getFileSystem()->createXMLReader(szXMLFile);
	core::stringc value;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
			case io::EXN_ELEMENT:
            {
                if (core::stringw("energysrc") == xml->getNodeName()) 
				{
                    value = xml->getAttributeValue(L"sizeX");
					sizeX = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"sizeY");
					sizeY = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"sizeZ");
					sizeZ = (float)atof(value.c_str());

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

	if(showDebugLabel)
	{
		txtDebug = device->getSceneManager()->addTextSceneNode(device->getGUIEnvironment()->getBuiltInFont(), L"", SColor(255, 255, 255, 255));
	}

	SimuObject::load(szModelFile);
	
}

float EnergySource::extractBatteryPower()
{
	if(batteryLevel > MIN_SOURCE_BATTERY_LEVEL) 
		batteryLevel -= 0.1f; 
	
	if(batteryLevel < MIN_SOURCE_BATTERY_LEVEL)
		batteryLevel = MIN_SOURCE_BATTERY_LEVEL;

	return 0.1f;
}

void EnergySource::updateDebugLabel()
{
	if(NULL != txtDebug)
	{
		vector3df pos = getNode()->getPosition();
		pos.Y = getSizeY() * 2.0f;
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

void EnergySource::loop()
{
	updateDebugLabel();
}
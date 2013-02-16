#include "Chasis.h"


Chasis::Chasis(IrrlichtDevice *device): CarPart(device)
{
	wheelsDesc = NULL;
}

Chasis::~Chasis()
{
	if(NULL != wheelsDesc)
		delete[] wheelsDesc;
}

WheelDescriptor *Chasis::getWheelDescriptor(unsigned int wheelIndex)
{
	if(wheelIndex < wheelsCount)
	{
		return &wheelsDesc[wheelIndex];
	}

	return NULL;
}

ToolDescriptor *Chasis::getArmDescriptor()
{
	return &armDesc;
}

void Chasis::load(char *path)
{
	char szModelFile[255];
	char szXMLFile[255];

	strncpy_s(szModelFile, path, 255);
	strcat_s(szModelFile, "/chasis.3ds");

	strncpy_s(szXMLFile, path, 255);
	strcat_s(szXMLFile, "/chasis.xml");

	io::IXMLReader* xml = device->getFileSystem()->createXMLReader(szXMLFile);
	core::stringc value;
	
	unsigned int currentWheel = 0;

	collisionBoxTranslation.X = 0;
	collisionBoxTranslation.Y = 0;
	collisionBoxTranslation.Z = 0;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
			case io::EXN_ELEMENT:
            {
                if (core::stringw("chasis") == xml->getNodeName()) 
				{
                    value = xml->getAttributeValue(L"sizeX");
					sizeX = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"sizeY");
					sizeY = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"sizeZ");
					sizeZ = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"weight");
					weight = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"wheelsCount");
					wheelsCount = atoi(value.c_str());

					value = xml->getAttributeValue(L"collisionBoxTransX");
					collisionBoxTranslation.X = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"collisionBoxTransY");
					collisionBoxTranslation.Y = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"collisionBoxTransZ");
					collisionBoxTranslation.Z = (float)atof(value.c_str());

					if((wheelsCount > 0) && (wheelsDesc == NULL))
						wheelsDesc = new WheelDescriptor[wheelsCount];
				}
                else
                if (core::stringw("wheelDescriptor") == xml->getNodeName())
				{
					if(currentWheel < wheelsCount)
					{
						value = xml->getAttributeValue(L"canRotate");						
						wheelsDesc[currentWheel].canRotate = (atoi(value.c_str()) != 0);
						
						value = xml->getAttributeValue(L"initialRotation");
						wheelsDesc[currentWheel].initialRotation = (float)atof(value.c_str());

						value = xml->getAttributeValue(L"motor");
						wheelsDesc[currentWheel].motor = (atoi(value.c_str()) != 0);

						value = xml->getAttributeValue(L"maxRotation");
						wheelsDesc[currentWheel].maxRotation = (float)atof(value.c_str());

						value = xml->getAttributeValue(L"posX");
						wheelsDesc[currentWheel].posX = (float)atof(value.c_str());

						value = xml->getAttributeValue(L"posY");
						wheelsDesc[currentWheel].posY = (float)atof(value.c_str());

						value = xml->getAttributeValue(L"posZ");
						wheelsDesc[currentWheel].posZ = (float)atof(value.c_str());

						currentWheel++;
					}
				}
				else
				if (core::stringw("armDescriptor") == xml->getNodeName())
				{
					value = xml->getAttributeValue(L"posX");
					armDesc.posX = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"posY");
					armDesc.posY = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"posZ");
					armDesc.posZ = (float)atof(value.c_str());
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

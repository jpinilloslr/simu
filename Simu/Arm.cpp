#include "Arm.h"


Arm::Arm(IrrlichtDevice *device): CarPart(device)
{
	batteryConsume = 0.0f;
}


Arm::~Arm(void)
{
}

void Arm::load(char *path)
{
	char szModelFile[255];
	char szXMLFile[255];

	strncpy_s(szModelFile, path, 255);
	strcat_s(szModelFile, "/tool.3ds");

	strncpy_s(szXMLFile, path, 255);
	strcat_s(szXMLFile, "/tool.xml");

	io::IXMLReader* xml = device->getFileSystem()->createXMLReader(szXMLFile);
	core::stringc value;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
			case io::EXN_ELEMENT:
            {
                if (core::stringw("tool") == xml->getNodeName()) 
				{
                    value = xml->getAttributeValue(L"sizeX");
					sizeX = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"sizeY");
					sizeY = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"sizeZ");
					sizeZ = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"weight");
					weight = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"anchorX");
					anchorX = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"anchorY");
					anchorY = (float)atof(value.c_str());

					value = xml->getAttributeValue(L"anchorZ");
					anchorZ = (float)atof(value.c_str());
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

void Arm::setToolDescriptor(ToolDescriptor *td) 
{ 
	memcpy(&descriptor, td, sizeof(ToolDescriptor)); 
	getNode()->setPosition(vector3df(descriptor.posX, descriptor.posY, descriptor.posZ));
}

void Arm::setToNormalPosition()
{
	batteryConsume += 0.0005;
	float angle = dJointGetHingeAngle(odeJoint) * (180.0f/irr::core::PI);

	dJointSetHingeParam(odeJoint, dParamLoStop, -140.0f / (180.0f/irr::core::PI));
	dJointSetHingeParam(odeJoint, dParamHiStop, 0.0f);

	dJointSetHingeParam(odeJoint, dParamVel, 1.0f);
	dJointSetHingeParam(odeJoint, dParamFMax, 20.0f);
}

void Arm::launch()
{
	batteryConsume += 0.0005;
	float angle = dJointGetHingeAngle(odeJoint) * (180.0f/irr::core::PI);

	dJointSetHingeParam(odeJoint, dParamLoStop, -140.0f / (180.0f/irr::core::PI));
	dJointSetHingeParam(odeJoint, dParamHiStop, 0.0f);

	dJointSetHingeParam(odeJoint, dParamVel, -1.0f);
	dJointSetHingeParam(odeJoint, dParamFMax, 20.0f);
}

bool Arm::isInPosition() 
{ 
	float angle = dJointGetHingeAngle(odeJoint) * (180.0f/irr::core::PI); 
	return(abs(angle) < 7.0f); 
}

float Arm::popBatteryConsume()
{
	float v = batteryConsume;
	batteryConsume = 0.0f;
	return v;
}
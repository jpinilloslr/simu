#pragma once


typedef struct 
{
	float posX;
	float posY;
	float posZ;
	float initialRotation;
	float maxRotation;
	bool canRotate;
	bool motor;
} WheelDescriptor;
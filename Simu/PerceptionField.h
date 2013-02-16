#pragma once
#include <irrlicht.h>
#include "Perceptible.h"

#define MAX_PERCEPTIONS 10

class PerceptionField
{
private:
	Perceptible perceptionField[MAX_PERCEPTIONS];
	unsigned int perceptions;
public:
	PerceptionField();
	~PerceptionField();
	void addPerception(Perceptible *perc);
	unsigned int getPerceptionsCount()           { return perceptions; }
	Perceptible* getPerception(unsigned int i)   { if(i < perceptions) return &perceptionField[i]; else return NULL; }
	void clearPerceptions()                      { perceptions = 0; }
};


#include "PerceptionField.h"


PerceptionField::PerceptionField()
{
	perceptions = 0;
}


PerceptionField::~PerceptionField()
{
}

void PerceptionField::addPerception(Perceptible *perc)
{
	if(perceptions < MAX_PERCEPTIONS)
	{
		memcpy(&perceptionField[perceptions], perc, sizeof(Perceptible));
		perceptions++;
	}
	else
	{
		for(int i=MAX_PERCEPTIONS-1; i>0; i--)
		{
			memcpy(&perceptionField[i-1], &perceptionField[i], sizeof(Perceptible));
		}
	}
}
#pragma once

enum PerceptibleType  { pt_unknown, pt_robot, pt_energysource, pt_camera };

typedef struct 
{
	void *pointer;
	PerceptibleType type;
} Perceptible;
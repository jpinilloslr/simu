#pragma once
#include "Car.h"
#include "EnergySource.h"
#include <ode\ode.h>

void startOde();
void initCarPhysics(Car *car);
void initEnergySourcePhysics(EnergySource *source);
void simLoop (int pause);
void updateCar(Car *car);
void updateEnergySource(EnergySource *source);
void QuaternionToEuler(const dQuaternion quaternion, vector3df &euler);
float getAbsoluteYFromQuaternion(const dQuaternion quaternion);
static void nearCallback (void *data, dGeomID o1, dGeomID o2);
void stopCarPhysics(Car *car);
void stopEnergySourcePhysics(EnergySource *source);
void setScenePhysics(IMeshSceneNode* node);
void stopOde();

dWorldID odeGetWorld();
dSpaceID odeGetSpace();
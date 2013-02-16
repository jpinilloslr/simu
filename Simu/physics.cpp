#include "physics.h"

static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
static dGeomID ground;

int jointsCount;

dWorldID odeGetWorld()
{
	return world;
}

dSpaceID odeGetSpace()
{
	return space;
}

/*void debug(char *text)
{
	FILE *file = fopen("debug.txt", "a");
	fwrite(text, 1, strlen(text), file);
	fclose(file);
}*/

void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	int i,n;

	const int N = 10;
	dContact contact[N];
	n = dCollide (o1, o2, N, &contact[0].geom, sizeof(dContact));

	if (n > 0) 
	{
		for (i=0; i<n; i++) 
		{
			contact[i].surface.mode = dContactApprox1 | dContactSlip1 | dContactSlip2;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.slip1 = 0.1f;
			contact[i].surface.slip2 = 0.1f;
			contact[i].surface.soft_erp = 0.5f;
			contact[i].surface.soft_cfm = 0.3f;
			dJointID c = dJointCreateContact (world, contactgroup, &contact[i]);
			dJointAttach (c, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));							
			jointsCount++;

			/*Car *car;
			car = static_cast<Car*>(dGeomGetData(contact[i].geom.g1));

			if(car != NULL)
			{
				if(contact[i].geom.g2 == ground)
					car->getSensors()->setContact(contact[i].geom.g1, NULL);
				else
					car->getSensors()->setContact(contact[i].geom.g1, contact[i].geom.g2);
			}
			else
			{
				car = static_cast<Car*>(dGeomGetData(contact[i].geom.g2));
				if(car != NULL)
				{
					if(contact[i].geom.g1 == ground)
						car->getSensors()->setContact(contact[i].geom.g2, NULL);
					else
						car->getSensors()->setContact(contact[i].geom.g2, contact[i].geom.g1);
				}
			}*/
		}		
	}

}

float getAbsoluteYFromQuaternion(const dQuaternion quaternion)
{
	dReal W,X,Y,Z;

	W = quaternion[0];
	X = quaternion[1];
	Y = quaternion[2];
	Z = quaternion[3];

    double sqw = W * W;
    double sqx = X * X;
    double sqy = Y * Y;
    double sqz = Z * Z;

	float angle;

    // If quaternion is normalised the unit is one, otherwise it is the correction factor
    double unit = sqx + sqy + sqz + sqw;
    double test = X * Y + Z * W;

    if (test > 0.4999f * unit)                              // 0.4999f OR 0.5f - EPSILON
    {
        // Singularity at north pole
        angle = 2.0f * (float)atan2(X, W);
    }
    else if (test < -0.4999f * unit)                        // -0.4999f OR -0.5f + EPSILON
    {
        // Singularity at south pole
        angle = -2.0f * (float)atan2(X, W);
    }
    else
    {
        angle = (float)atan2((float)(2.0f * Y * W - 2.0f * X * Z), (float)(sqx - sqy - sqz + sqw));       // Yaw        
    }

	angle *= (180.0f/irr::core::PI);
	return angle;
}

void QuaternionToEuler(const dQuaternion quaternion, vector3df &euler)
{
	dReal w,x,y,z;

	w = quaternion[0];
	x = quaternion[1];
	y = quaternion[2];
	z = quaternion[3];

	double sqw = w*w;    
	double sqx = x*x;    
	double sqy = y*y;    
	double sqz = z*z; 
  
	euler.Z = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw)) * (180.0f/irr::core::PI));  
	euler.X = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw)) * (180.0f/irr::core::PI));    		
	euler.Y = (irr::f32) (asin(-2.0 * (x*z - y*w)) * (180.0f/irr::core::PI));

	while(euler.X > 360.0f)
		euler.X -= 360.0f;
	while(euler.X < 0.0f)
		euler.X += 360.0f;

	while(euler.Z > 360.0f)
		euler.Z -= 360.0f;
	while(euler.Z < 0.0f)
		euler.Z += 360.0f;
}

void updateCar(Car *car)
{
	dQuaternion result;
	vector3df rot;

	dGeomGetQuaternion(car->getChasis()->odeGeom, result);
	QuaternionToEuler(result, rot);
	car->getChasis()->setAbsoluteYAngle(getAbsoluteYFromQuaternion(result));

	car->getChasis()->getNode()->setRotation(rot);	
	const dReal *positionChasis = dBodyGetPosition(car->getChasis()->odeBody);	
	car->getChasis()->getNode()->setPosition(vector3df(positionChasis[0], positionChasis[1], positionChasis[2]));	
	

	for(int i=0; i< car->getChasis()->getWheelsCount(); i++)
	{
		dGeomGetQuaternion(car->getWheels()[i]->odeGeom, result);
		QuaternionToEuler(result, rot);
		car->getWheels()[i]->getNode()->setRotation(rot);
		const dReal *positionWheel = dBodyGetPosition(car->getWheels()[i]->odeBody);	
		car->getWheels()[i]->getNode()->setPosition(vector3df(positionWheel[0], positionWheel[1], positionWheel[2]));	
	}	

	dGeomGetQuaternion(car->getArm()->odeGeom, result);
	QuaternionToEuler(result, rot);
	car->getArm()->getNode()->setRotation(rot);
	const dReal *positionTool = dBodyGetPosition(car->getArm()->odeBody);	
	car->getArm()->getNode()->setPosition(vector3df(positionTool[0], positionTool[1], positionTool[2]));	

}

void updateEnergySource(EnergySource *source)
{
	dQuaternion result;
	vector3df rot;

	dGeomGetQuaternion(source->odeGeom, result);
	QuaternionToEuler(result, rot);
	source->setAbsoluteYAngle(getAbsoluteYFromQuaternion(result));

	source->getNode()->setRotation(rot);	
	const dReal *positionChasis = dBodyGetPosition(source->odeBody);	
	source->getNode()->setPosition(vector3df(positionChasis[0], positionChasis[1], positionChasis[2]));		
}

void simLoop (int pause)
{
	if (!pause) {				

		dSpaceCollide (space, 0, &nearCallback);
		dWorldStep (world, 0.08f);
		//dWorldStepFast1 (world, 0.05f, 3);
		dJointGroupEmpty (contactgroup);
	}
}

void initCarPhysics(Car *car)
{
	dMass m;

	// chassis body
	car->getChasis()->odeBody = dBodyCreate (world);
	
	dMassSetBox (&m, 1, car->getChasis()->getSizeX(), car->getChasis()->getSizeY(), car->getChasis()->getSizeZ());
	dMassAdjust (&m, car->getChasis()->getWeight());
	dBodySetMass (car->getChasis()->odeBody, &m);
	car->getChasis()->odeGeom = dCreateBox (0, car->getChasis()->getSizeX(), car->getChasis()->getSizeY(), car->getChasis()->getSizeZ());
	dGeomSetBody (car->getChasis()->odeGeom, car->getChasis()->odeBody);

	dBodySetPosition (car->getChasis()->odeBody, car->getChasis()->getNode()->getPosition().X, 
													car->getChasis()->getNode()->getPosition().Y, 
													car->getChasis()->getNode()->getPosition().Z);
	
	// wheel bodies
	for (int i=0; i<car->getChasis()->getWheelsCount(); i++) 
	{
		car->getWheels()[i]->odeBody = dBodyCreate (world);

		dMassSetSphere (&m, 1, car->getWheels()[i]->getRadius());
		dMassAdjust (&m, car->getWheels()[i]->getWeight());
		dBodySetMass (car->getWheels()[i]->odeBody, &m);
		car->getWheels()[i]->odeGeom = dCreateSphere (0, car->getWheels()[i]->getRadius());
		dGeomSetBody (car->getWheels()[i]->odeGeom, car->getWheels()[i]->odeBody);
	}

	vector3df wheelPosition;

	for(int i=0;i < car->getChasis()->getWheelsCount(); i++)
	{
		wheelPosition.X = car->getWheels()[i]->getWheelDescriptor()->posX;
		wheelPosition.Y = car->getWheels()[i]->getWheelDescriptor()->posY;
		wheelPosition.Z = car->getWheels()[i]->getWheelDescriptor()->posZ;
		dBodySetPosition (car->getWheels()[i]->odeBody, (float)(car->getChasis()->getNode()->getPosition().X + wheelPosition.X), 
														(float)(car->getChasis()->getNode()->getPosition().Y + wheelPosition.Y) , 
														(float)(car->getChasis()->getNode()->getPosition().Z + wheelPosition.Z));
	}

	//wheel hinges
	for (int i=0; i<car->getChasis()->getWheelsCount(); i++) 
	{
		car->getWheels()[i]->odeJoint = dJointCreateHinge2 (world,0);
		dJointAttach (car->getWheels()[i]->odeJoint, car->getChasis()->odeBody, car->getWheels()[i]->odeBody);
		const dReal *a = dBodyGetPosition (car->getWheels()[i]->odeBody);
		dJointSetHinge2Anchor (car->getWheels()[i]->odeJoint, a[0], a[1], a[2]);
		dJointSetHinge2Axis1 (car->getWheels()[i]->odeJoint, 0, 1, 0);
		dJointSetHinge2Axis2 (car->getWheels()[i]->odeJoint, 1, 0, 0);
	}

	for (int i=0; i<car->getChasis()->getWheelsCount(); i++) 
	{
		dJointSetHinge2Param (car->getWheels()[i]->odeJoint,dParamSuspensionERP, 0.5f);
		dJointSetHinge2Param (car->getWheels()[i]->odeJoint,dParamSuspensionCFM, 0.2f);
	}

	// lock back wheels along the steering axis
	for (int i=0; i<car->getChasis()->getWheelsCount(); i++) 
	{
		if(!car->getWheels()[i]->getWheelDescriptor()->canRotate)
		{
			dJointSetHinge2Param (car->getWheels()[i]->odeJoint, dParamLoStop, 0);
			dJointSetHinge2Param (car->getWheels()[i]->odeJoint, dParamHiStop, 0);
		}
	}

	// arm bodies
	car->getArm()->odeBody = dBodyCreate (world);

	dMassSetBox (&m, 1, car->getArm()->getSizeX(), car->getArm()->getSizeY(), car->getArm()->getSizeZ());
	dMassAdjust (&m, car->getArm()->getWeight());
	dBodySetMass (car->getArm()->odeBody, &m);
	car->getArm()->odeGeom = dCreateBox (0, car->getArm()->getSizeX(), car->getArm()->getSizeY(), car->getArm()->getSizeZ());
	dGeomSetBody (car->getArm()->odeGeom, car->getArm()->odeBody);

	vector3df toolsPosition;

	//arm hinges
	toolsPosition.X = car->getArm()->getToolDescriptor()->posX;
	toolsPosition.Y = car->getArm()->getToolDescriptor()->posY;
	toolsPosition.Z = car->getArm()->getToolDescriptor()->posZ;
	dBodySetPosition (car->getArm()->odeBody, (float)(car->getChasis()->getNode()->getPosition().X + toolsPosition.X), 
														(float)(car->getChasis()->getNode()->getPosition().Y + toolsPosition.Y) , 
														(float)(car->getChasis()->getNode()->getPosition().Z + toolsPosition.Z));

	car->getArm()->odeJoint = dJointCreateHinge (world, 0);
	dJointAttach (car->getArm()->odeJoint, car->getChasis()->odeBody, car->getArm()->odeBody);


	dJointSetHingeAnchor(car->getArm()->odeJoint, car->getChasis()->getNode()->getPosition().X + toolsPosition.X + car->getArm()->getAnchorX(), 
															car->getChasis()->getNode()->getPosition().Y + toolsPosition.Y + car->getArm()->getAnchorY(), 
															car->getChasis()->getNode()->getPosition().Z + toolsPosition.Z + car->getArm()->getAnchorZ());
	dJointSetHingeAxis(car->getArm()->odeJoint, 1, 0, 0);
	
	car->getArm()->setToNormalPosition();

	// create car space and add it to the top level space
	car->odeSpace = dSimpleSpaceCreate (space);
	dSpaceSetCleanup(car->odeSpace, 0);
	dSpaceAdd(car->odeSpace, car->getChasis()->odeGeom);

	for(int i=0; i<car->getChasis()->getWheelsCount(); i++)
	{
		dSpaceAdd(car->odeSpace, car->getWheels()[i]->odeGeom);
		dGeomSetData(car->getWheels()[i]->odeGeom, (void*)car);
	}
	
	dSpaceAdd(space, car->getArm()->odeGeom);
	dGeomSetData(car->getArm()->odeGeom, (void*)car);

	dGeomSetData(car->getChasis()->odeGeom, (void*)car); 
}

void initEnergySourcePhysics(EnergySource *source)
{
	dMass m;
	source->odeBody = dBodyCreate (world);
	
	dMassSetBox (&m, 1, source->getSizeX(), source->getSizeY(), source->getSizeZ());
	dMassAdjust (&m, source->getWeight());
	dBodySetMass (source->odeBody, &m);

	

	source->odeGeom = dCreateBox (0, source->getSizeX(), source->getSizeY(), source->getSizeZ());
	//source->odeGeom = dCreateCylinder(0, source->getRadius(), source->getSizeY);
	dGeomSetBody (source->odeGeom, source->odeBody);

	dBodySetPosition (source->odeBody, source->getNode()->getPosition().X, 
													source->getNode()->getPosition().Y, 
													source->getNode()->getPosition().Z);

	dSpaceAdd(space, source->odeGeom);
}

void startOde()
{	
	dInitODE2(0);
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contactgroup = dJointGroupCreate(0);
	dWorldSetGravity (world, 0.0f, -0.6f, 0.0f);
	//ground = dCreatePlane (space, 0, 1, 0, 0);	
	//dGeomSetData(ground, (void*)0);
}

void stopOde()
{
	dJointGroupDestroy (contactgroup);
	dSpaceDestroy (space);
	dWorldDestroy (world);
	dCloseODE();
}

void stopCarPhysics(Car *car)
{
	dGeomDestroy (car->getChasis()->odeGeom);

	for(int i=0; i<car->getChasis()->getWheelsCount(); i++)
	{
		dGeomDestroy (car->getWheels()[i]->odeGeom);
	}

	dGeomDestroy (car->getArm()->odeGeom);
}

void stopEnergySourcePhysics(EnergySource *source)
{
	dGeomDestroy (source->odeGeom);
}

void setScenePhysics(IMeshSceneNode* node)
{
	if(node->getMesh() == NULL) 
		return; 
    int i, j, ci, cif, cv;
    int indexcount = 0;
    int vertexcount = 0;

    // count vertices and indices
	for(i=0;(unsigned int)i<node->getMesh()->getMeshBufferCount();i++)
	{
		irr::scene::IMeshBuffer* mb = node->getMesh()->getMeshBuffer(i);
		indexcount += mb->getIndexCount();
		vertexcount += mb->getVertexCount();
	}

    // build structure for ode trimesh geom
    dVector3 *vertices= new dVector3[vertexcount];
    int *indices = new int[indexcount];

    // fill trimesh geom
    ci  = 0; // current index in the indices array
    cif = 0; // offset of the irrlicht-vertex-index in the vetices array 
    cv  = 0; // current index in the vertices array

    for(i=0; (unsigned int)i < node->getMesh()->getMeshBufferCount(); i++)
	{
		irr::scene::IMeshBuffer* mb = node->getMesh()->getMeshBuffer(i);

		// fill indices
		irr::u16* mb_indices = mb->getIndices();

		for(j=0; (unsigned int)j < mb->getIndexCount(); j++)
		{
			// scale the indices from multiple meshbuffers to single index array
			indices[ci]=cif+mb_indices[j];
			ci++;
		}

		// update the offset for the next meshbuffer
		cif = cif + mb->getVertexCount();

		// fill vertices
		if(mb->getVertexType()==irr::video::EVT_STANDARD)
		{
			irr::video::S3DVertex* mb_vertices=
			(irr::video::S3DVertex*)mb->getVertices();

			for(j=0; (unsigned int)j<mb->getVertexCount();j++)
			{
				vertices[cv][0]=mb_vertices[j].Pos.X;
				vertices[cv][1]=mb_vertices[j].Pos.Y;
				vertices[cv][2]=mb_vertices[j].Pos.Z;
				cv++;
			} 
		} 
		else 
		if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
			irr::video::S3DVertex2TCoords* mb_vertices=
			(irr::video::S3DVertex2TCoords*)mb->getVertices();

			for(j=0; (unsigned int)j<mb->getVertexCount(); j++)
			{
				vertices[cv][0] = mb_vertices[j].Pos.X;
				vertices[cv][1] = mb_vertices[j].Pos.Y;
				vertices[cv][2] = mb_vertices[j].Pos.Z;
				cv++;
			}    
		} 
	}

	irr::core::vector3df pos=node->getPosition();
	// build the trimesh data
	dTriMeshDataID data=dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(data, (dReal*)vertices, vertexcount, (dTriIndex *)indices, indexcount);

	// build the trimesh geom 
	dGeomID geom=dCreateTriMesh(space, data, 0, 0, 0);

	// set the geom position 
	dGeomSetPosition(geom, pos.X, pos.Y, pos.Z);

	// converted from meshes to have a body
	dGeomSetBody(geom, 0); 
}

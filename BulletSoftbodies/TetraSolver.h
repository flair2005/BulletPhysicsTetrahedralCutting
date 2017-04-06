#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <fstream>
#include <math.h>
#include <stdio.h> 
#include <cmath>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody\btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody\btSoftBody.h>
#include <BulletSoftBody\btSoftBodyHelpers.h>
#include "CLUnique.h"
#include <tchar.h>
#include <ios>
#include <Windows.h>
#include <LinearMath\btAlignedObjectArray.h>

struct ContactResult : public btCollisionWorld::ContactResultCallback
{
	bool m_connected;
	btScalar m_margin;
	ContactResult() :m_connected(false), m_margin(0.05)
	{
	}
	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		if (cp.getDistance() <= m_margin)
			m_connected = true;
		return 1.f;
	}
};

static std::string TCHARToString2(const TCHAR* tchar)
{
	std::wstring temp(tchar);
	std::string str(temp.begin(), temp.end());
	return str;
}

static std::string getexepath()
{
    TCHAR result[MAX_PATH];
    GetModuleFileName(NULL, result, MAX_PATH);
	std::string str = TCHARToString2(result);
	for (int x=0; 20 > x; x++)
	{
		str.pop_back();
	}
	return str;
}

class Pair
{
public:
	Pair()
	{
		i1 = 0;
		i2 = 0;
	}
	Pair(int index1, int index2, btVector3 off)
	{
		i1 = index1;
		i2 = index2;
		offset = off;
	}
	int i1;
	int i2;
	btVector3 offset;
};

class int3
{
public:
	int3(int ii1, int ii2, int ii3)
	{
		i1 = ii1;
		i2 = ii2;
		i3 = ii3;
	}
	int i1;
	int i2;
	int i3;

};

class TetraSolver
{
public:
	TetraSolver()
	{
		counter = 0;
		settings.resize(11, 0);
		settings[0] = 2.0; // timestep
		settings[1] = -9.8; // gravity
		settings[2] = 1.0; // stiffness
		settings[3] = 0.0; // damping
		settings[4] = 1.0; // friction
		settings[5] = 1; // iterations
		settings[6] = 1.0; // mass
		settings[7] = 0.0; // pressure
		settings[8] = 0.0; // volume conservation
		settings[9] = 2.0; // margin
		settings[10] = 1.0; // scale

		softbodymade = false;
		path = getexepath();

	}
	void ImportBaseMesh(std::string path);
	void ImportPrimaryMesh(std::string path);
	void ImportScapel(std::string path);
	void GeneratePairs();
	void Init(btTransform trans);
	void Init2();
	void MakeFEMSoftbody(btTransform trans);
	std::vector<btVector3> ExportPath();
	std::vector<btVector3> ExportPath2();
	std::vector<btVector3> ExportPath3();
	void StepSimulation2();
	void StepSimulation();
	void ExportBinary();
	void ExportBinary2();
	void ExportBinary3();
	void ExportPath4();
	void LoadSettings();
	void LoadSettings2();
	void CutTest();
	void CutTest2();
	void GetData();
	std::string path;
	std::vector<float> settings;

	void ImportBaseMesh9(std::string path, btTransform trans);
	std::vector<btVector3> sb9;
	std::vector<btScalar> sbdata9;
	std::vector<int> sbi9;

	// Softbody
	btSoftBody* psb;
	btSoftBody* softbody;
	btCollisionShape* scapelshape;
	btVector3 scapelpos;
	std::vector<Pair> pairs;
	std::vector<btVector3> BaseTetMesh;
	std::vector<btVector3> PrimaryTetMesh;
	std::vector<btVector3> Offset;
	std::vector<btVector3> scapelpoints;
	std::vector<std::vector<int>> vertpairs;
	std::vector<std::vector<int>> linkpairs;
	std::vector<bool> validlinks;
	btTransform t;
	int counter;
	bool softbodymade;

	// Bullet world
	btDbvtBroadphase* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btSoftBodyRigidBodyCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSoftRigidDynamicsWorld* dynamicsWorld;

};

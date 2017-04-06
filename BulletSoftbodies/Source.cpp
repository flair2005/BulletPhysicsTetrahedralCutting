#include "TetraSolver.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "Helpers.h"

int main()
{

	//tetra
	
	btTransform trans; trans.setIdentity();
	trans.setOrigin(btVector3(0, 80, 0));
	TetraSolver* solver = new TetraSolver();
	solver->ImportBaseMesh9(getexepath() + "soft.obj", trans);

	/*
	TetraSolver* solver = new TetraSolver();
	solver->ImportBaseMesh(getexepath() + "tetra.obj");
	solver->ImportScapel(getexepath() + "scapel.obj");
	//solver->ImportPrimaryMesh("sphere.obj");
	//solver->GeneratePairs();
	solver->Init(trans);
	*/

	// ground
	btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(5000.),btScalar(5.),btScalar(5000.)));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-5,0));
	btScalar mass(0.);
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass,localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	solver->dynamicsWorld->addRigidBody(body);
	

	std::cout << "Simulation Started!" << std::endl;
	HWND hwnd = GetBulletSoftbodiesEXE();
	while(true)
	{	
		TCHAR text[256];
		SendMessage(hwnd, WM_GETTEXT, sizeof(text)/sizeof(text[0]), LPARAM(text));
		std::string titletext = TCHARToString(text);
		int index = (std::atoi(titletext.c_str()));

	    if (index == 1)
		{
			solver->StepSimulation2();
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)stringToTCHAR(std::to_string(2)));
		}
	}

	std::cout << "Simulation Done!" << std::endl;
	std::cin.get();
	

	std::cin.get();
	return 0;

}



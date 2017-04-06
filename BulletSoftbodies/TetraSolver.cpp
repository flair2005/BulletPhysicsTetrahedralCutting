#include "TetraSolver.h"

void TetraSolver::ImportBaseMesh(std::string path)
{


	std::ifstream file(path);
	std::string str;
	std::vector<std::string> objFile;
	std::vector<std::string> objIndexFile;
	while (std::getline(file, str))
	{
		if (str[0] == 'v') {
			str[0] = ' ';
			objFile.push_back(str);
		}
		if (str[0] == 'f') {
			str[0] = ' ';
			objIndexFile.push_back(str);
		}
	}

	for (int x = 0; objFile.size() > x; x++)
	{
		std::string str = objFile[x];
		std::istringstream buf(str);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end); // done!
		float px = std::atof(tokens[0].c_str());
		float py = std::atof(tokens[1].c_str());
		float pz = std::atof(tokens[2].c_str());
		BaseTetMesh.push_back(btVector3(px, py, pz));
	}


}

void TetraSolver::ImportBaseMesh9(std::string path, btTransform trans)
{


	std::ifstream file(path);
	std::string str;
	std::vector<std::string> objFile;
	std::vector<std::string> objIndexFile;
	while (std::getline(file, str))
	{
		if (str[0] == 'v') {
			str[0] = ' ';
			objFile.push_back(str);
		}
		if (str[0] == 'f') {
			str[0] = ' ';
			objIndexFile.push_back(str);
		}
	}

	for (int x = 0; objFile.size() > x; x++)
	{
		std::string str = objFile[x];
		std::istringstream buf(str);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end); // done!
		float px = std::atof(tokens[0].c_str());
		float py = std::atof(tokens[1].c_str());
		float pz = std::atof(tokens[2].c_str());
		sb9.push_back(btVector3(px, py, pz));
		sbdata9.push_back(px);
		sbdata9.push_back(py);
		sbdata9.push_back(pz);
	}

	for (int x = 0; objIndexFile.size() > x; x++)
	{
		std::string str = objIndexFile[x];
		std::istringstream buf(str);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end); // done!
		int px = std::atoi(tokens[0].c_str());
		int py = std::atoi(tokens[1].c_str());
		int pz = std::atoi(tokens[2].c_str());
		sbi9.push_back(px - 1);
		sbi9.push_back(py - 1);
		sbi9.push_back(pz - 1);
	}

	Init2();
	psb = btSoftBodyHelpers::CreateFromTriMesh(dynamicsWorld->getWorldInfo(), &sbdata9[0], &sbi9[0], (int)(sbi9.size() / 3), false);

	std::vector<btVector3> points;
	for (int x=0; psb->m_nodes.size() > x; x++)
	{
		points.push_back(psb->m_nodes[x].m_x);
	}

	//psb = new btSoftBody(&dynamicsWorld->getWorldInfo(), (int)sb9.size(), &sb9[0], 0);
	//for (int x = 0; sbi9.size() > x; x++)
	{
		//psb->appendFace(sbi9[x].i1, sbi9[x].i2, sbi9[x].i3);
	}

	psb->transform(trans);
	psb->m_materials[0]->m_kLST = 0.1;
	//psb->m_cfg.collisions|=btSoftBody::fCollision::VF_SS;
	psb->getCollisionShape()->setMargin(3);
	psb->setTotalMass(1.0, false);
	psb->generateClusters(1);
	psb->m_cfg.kPR = 0;
	softbody = psb;
	softbodymade = true;
	dynamicsWorld->addSoftBody(softbody);

}

void TetraSolver::ImportPrimaryMesh(std::string path)
{


	std::ifstream file(path);
	std::string str;
	std::vector<std::string> objFile;
	std::vector<std::string> objIndexFile;
	while (std::getline(file, str))
	{
		if (str[0] == 'v') {
			str[0] = ' ';
			objFile.push_back(str);
		}
		if (str[0] == 'f') {
			str[0] = ' ';
			objIndexFile.push_back(str);
		}
	}

	for (int x = 0; objFile.size() > x; x++)
	{
		std::string str = objFile[x];
		std::istringstream buf(str);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end); // done!
		float px = std::atof(tokens[0].c_str());
		float py = std::atof(tokens[1].c_str());
		float pz = std::atof(tokens[2].c_str());
		PrimaryTetMesh.push_back(btVector3(px, py, pz));
	}


}

void TetraSolver::ImportScapel(std::string path)
{


	std::ifstream file(path);
	std::string str;
	std::vector<std::string> objFile;
	std::vector<std::string> objIndexFile;
	while (std::getline(file, str))
	{
		if (str[0] == 'v') {
			str[0] = ' ';
			objFile.push_back(str);
		}
	}

	for (int x = 0; objFile.size() > x; x++)
	{
		std::string str = objFile[x];
		std::istringstream buf(str);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end); // done!
		float px = std::atof(tokens[0].c_str());
		float py = std::atof(tokens[1].c_str());
		float pz = std::atof(tokens[2].c_str());
		scapelpoints.push_back(btVector3(px, py, pz));
	}

	btConvexHullShape* ss = new btConvexHullShape();
    for (int i = 0; i < scapelpoints.size(); ++i) {
     	ss->addPoint(scapelpoints[i]);
   	}         
   	ss->recalcLocalAabb();
	scapelshape = ss;



}

void TetraSolver::GeneratePairs()
{

	for (int x=0; PrimaryTetMesh.size() > x; x++)
	{
		btVector3 pos1 = PrimaryTetMesh[x];
		int LowestIndex = 0;
		float LowestDist = 10000000;
		for (int i=0; BaseTetMesh.size() > i; i++)
		{
			btVector3 pos2 = BaseTetMesh[i];
			float dist = btDistance(pos1, pos2);
			if (dist < LowestDist)
			{
				LowestDist = dist;
				LowestIndex = i;
			}
		}
		Pair p(x, LowestIndex, (BaseTetMesh[LowestIndex] - pos1));
		pairs.push_back(p);
	}


}

void TetraSolver::MakeFEMSoftbody(btTransform trans)
{

	std::vector<btVector3> points = BaseTetMesh;
	psb = new btSoftBody(&dynamicsWorld->getWorldInfo(), (int)points.size(), &points[0], 0);

	for (int x = 3; points.size() > x; x+=4)
	{
		int n0 = x-3;
		int n1 = x-2;
		int n2 = x-1;
		int n3 = x-0;
		psb->appendTetra(n0, n1, n2, n3);

		psb->appendLink(n0, n1, 0, true);
		psb->m_links[psb->m_links.size() - 1].m_index1 = n0;
		psb->m_links[psb->m_links.size() - 1].m_index2 = n1;
		validlinks.push_back(false);

		psb->appendLink(n1, n2, 0, true);
		psb->m_links[psb->m_links.size() - 1].m_index1 = n1;
		psb->m_links[psb->m_links.size() - 1].m_index2 = n2;
		validlinks.push_back(false);

		psb->appendLink(n2, n0, 0, true);
		psb->m_links[psb->m_links.size() - 1].m_index1 = n2;
		psb->m_links[psb->m_links.size() - 1].m_index2 = n0;
		validlinks.push_back(false);

		psb->appendLink(n0, n3, 0, true);
		psb->m_links[psb->m_links.size() - 1].m_index1 = n0;
		psb->m_links[psb->m_links.size() - 1].m_index2 = n3;
		validlinks.push_back(false);

		psb->appendLink(n1, n3, 0, true);
		psb->m_links[psb->m_links.size() - 1].m_index1 = n1;
		psb->m_links[psb->m_links.size() - 1].m_index2 = n3;
		validlinks.push_back(false);

		psb->appendLink(n2, n3, 0, true);
		psb->m_links[psb->m_links.size() - 1].m_index1 = n2;
		psb->m_links[psb->m_links.size() - 1].m_index2 = n3;
		validlinks.push_back(false);
	}

	CLUniqueFinder* unique = new CLUniqueFinder();
	unique->InitCL();
	std::vector<CLVector4> positions;
	for (int x = 0; points.size() > x; x++)
		positions.push_back(CLVector4(points[x].x(), points[x].y(), points[x].z()));
	unique->positions = positions;

	std::vector<int> pindexs;
	std::vector<CLVec> vecs = unique->GPUMultipleInstances2(pindexs);
	int counter = 0;
	vertpairs.resize(pindexs.size());
	for (int x = 0; pindexs.size() > x; x++)
	{
		vertpairs[pindexs[x]].push_back(x);
		counter += 1;
	}

	std::vector<std::vector<int>> pair2;
	for (int x = 0; vertpairs.size() > x; x++)
	{
		if (vertpairs[x].size() < 1)
			continue;
		pair2.push_back(std::vector<int>());
		for (int i = 0; vertpairs[x].size() > i; i++)
		{
			pair2[pair2.size() - 1].push_back(vertpairs[x][i]);
		}
	}

	for (int x = 0; vertpairs.size() > x; x++)
	{
		if (vertpairs[x].size() < 1)
			continue;
		linkpairs.push_back(std::vector<int>());
		for (int i = 1; vertpairs[x].size() > i; i++)
		{
			int i1 = vertpairs[x][i - 1];
			int i2 = vertpairs[x][i - 0];
			psb->appendLink(i1, i2, 0, true);
			psb->m_links[psb->m_links.size() - 1].m_index1 = i1;
			psb->m_links[psb->m_links.size() - 1].m_index2 = i2;
			validlinks.push_back(false);
			linkpairs[linkpairs.size() - 1].push_back(psb->m_links.size() - 1);
		}
	}
	
	vertpairs = pair2;
	psb->transform(trans);
	psb->m_materials[0]->m_kLST = settings[2];
	psb->m_cfg.piterations = 40;
	psb->m_cfg.kDG = 0.6;
	psb->m_cfg.kDF = settings[4];
	psb->getCollisionShape()->setMargin(settings[9]);
	psb->setTotalMass(settings[6], false);
	psb->setRestLengthScale(settings[10]);
	softbody = psb;
	softbodymade = true;

}

void TetraSolver::Init(btTransform trans)
{

	// Softworld
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	((btSoftRigidDynamicsWorld*)dynamicsWorld)->getWorldInfo().m_sparsesdf.Initialize();
	dynamicsWorld->getDispatchInfo().m_enableSPU = true;
	t = trans;

	LoadSettings();

	// Make softbody
	MakeFEMSoftbody(trans);
	std::cout << "Solver Started!" << std::endl;

	dynamicsWorld->addSoftBody(softbody);

}

void TetraSolver::Init2()
{

	// Softworld
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	((btSoftRigidDynamicsWorld*)dynamicsWorld)->getWorldInfo().m_sparsesdf.Initialize();
	dynamicsWorld->getDispatchInfo().m_enableSPU = true;

	std::cout << "Solver Started!" << std::endl;

}

std::vector<btVector3> TetraSolver::ExportPath()
{
	
	std::vector<btVector3> points = PrimaryTetMesh;
	/*
	for (int x=0; pairs.size() > x; x++)
	{
		//int i1 = pairs[x].i1;
		int i2 = pairs[x].i2;

		points[x] += softbody->m_nodes[i2].m_x;

	}

	std::string wpath = ("Solver.bgeo");

	using namespace Partio;
	ParticlesDataMutable* p = create();
	ParticleAttribute data1Attr = p->addAttribute("position", VECTOR, 3);

	for (int x = 0; softbody->m_nodes.size() > x; x++) {
		int particle = p->addParticle();
		float* Pos1 = p->dataWrite<float>(data1Attr, particle);
		Pos1[0] = softbody->m_nodes[x].m_x.x();    Pos1[1] = softbody->m_nodes[x].m_x.y();    Pos1[2] = softbody->m_nodes[x].m_x.z();  
	}
	Partio::write(wpath.c_str(), *p);
	p->release();

	*/
	return points;

}

std::vector<btVector3> TetraSolver::ExportPath2()
{

	std::vector<btVector3> points = PrimaryTetMesh;
	for (int x=0; pairs.size() > x; x++)
	{
		//int i1 = pairs[x].i1;
		int i2 = pairs[x].i2;

		points[x] += softbody->m_nodes[i2].m_x;

	}

	std::string wpath = ("Solver" + std::to_string(counter) + ".obj");
	std::ofstream ofstream(wpath.c_str(), std::ios::trunc);

	for (int x=0; points.size() > x; x++)
	{
		ofstream << "v " + std::to_string(points[x].x()) + " " + std::to_string(points[x].z()) + " " + std::to_string(points[x].y()) + "\n";
	}
	
	for (int x = 0; points.size() / 4 > x; x++)
	{
		int tcount = 4 * x;
		btVector3 f1 = (btVector3(1, 2, 4) + btVector3(tcount, tcount, tcount));
		btVector3 f2 = (btVector3(1, 3, 2) + btVector3(tcount, tcount, tcount));
		btVector3 f3 = (btVector3(2, 3, 4) + btVector3(tcount, tcount, tcount));
		btVector3 f4 = (btVector3(1, 4, 3) + btVector3(tcount, tcount, tcount));

		ofstream << "f " + std::to_string(f1.x()) + " " + std::to_string(f1.z()) + " " + std::to_string(f1.y()) + "\n";
		ofstream << "f " + std::to_string(f2.x()) + " " + std::to_string(f2.z()) + " " + std::to_string(f2.y()) + "\n";
		ofstream << "f " + std::to_string(f3.x()) + " " + std::to_string(f3.z()) + " " + std::to_string(f3.y()) + "\n";
		ofstream << "f " + std::to_string(f4.x()) + " " + std::to_string(f4.z()) + " " + std::to_string(f4.y()) + "\n";
	}
	ofstream.close();


	return points;

}

std::vector<btVector3> TetraSolver::ExportPath3()
{

	std::vector<btVector3> points = PrimaryTetMesh;

	std::string wpath = ("Solver" + std::to_string(counter) + ".obj");
	std::ofstream ofstream(wpath.c_str(), std::ios::trunc);

	for (int x=0; softbody->m_nodes.size() > x; x++)
	{
		btVector3 points = softbody->m_nodes[x].m_x;
		ofstream << "v " + std::to_string(points.x()) + " " + std::to_string(points.z()) + " " + std::to_string(points.y()) + "\n";
	}
	
	for (int x = 0; softbody->m_nodes.size() / 4 > x; x++)
	{
		int tcount = 4 * x;
		btVector3 f1 = (btVector3(1, 2, 4) + btVector3(tcount, tcount, tcount));
		btVector3 f2 = (btVector3(1, 3, 2) + btVector3(tcount, tcount, tcount));
		btVector3 f3 = (btVector3(2, 3, 4) + btVector3(tcount, tcount, tcount));
		btVector3 f4 = (btVector3(1, 4, 3) + btVector3(tcount, tcount, tcount));

		ofstream << "f " + std::to_string(f1.x()) + " " + std::to_string(f1.z()) + " " + std::to_string(f1.y()) + "\n";
		ofstream << "f " + std::to_string(f2.x()) + " " + std::to_string(f2.z()) + " " + std::to_string(f2.y()) + "\n";
		ofstream << "f " + std::to_string(f3.x()) + " " + std::to_string(f3.z()) + " " + std::to_string(f3.y()) + "\n";
		ofstream << "f " + std::to_string(f4.x()) + " " + std::to_string(f4.z()) + " " + std::to_string(f4.y()) + "\n";
	}
	ofstream.close();


	return points;

}

void TetraSolver::ExportPath4()
{

	std::vector<btVector3> points = PrimaryTetMesh;

	std::string wpath = ("Solver" + std::to_string(counter) + ".obj");
	std::ofstream ofstream(wpath.c_str(), std::ios::trunc);

	for (int x=0; softbody->m_nodes.size() > x; x++)
	{
		btVector3 points = softbody->m_nodes[x].m_x;
		ofstream << "v " + std::to_string(points.x()) + " " + std::to_string(points.z()) + " " + std::to_string(points.y()) + "\n";
	}
	
	for (int x = 0; softbody->m_faces.size() > x; x++)
	{
		
	}
	ofstream.close();

}

void TetraSolver::LoadSettings()
{

	std::ifstream file(path + "settings.txt");
	if (file.is_open() == true)
	{
		std::string str;
		int settingcounter = 0;
		while (std::getline(file, str))
		{
			if (str[0] == '#') {
				str[0] = ' ';				
				settings[settingcounter] = std::atof(str.c_str());
				settingcounter += 1;
			}
		}

	}

	if (softbodymade == false)
		return;

	softbody->m_materials[0]->m_kLST = settings[2];
	softbody->m_cfg.kDF = settings[4];
	softbody->m_cfg.piterations = 40;
	if (settings[10] == 0.0)
		settings[10] = 0.1;
	softbody->setRestLengthScale(settings[10]);


}

void TetraSolver::LoadSettings2()
{

	std::ifstream file(path + "settings.txt");
	if (file.is_open() == true)
	{
		std::string str;
		int settingcounter = 0;
		while (std::getline(file, str))
		{
			if (str[0] == '#') {
				str[0] = ' ';				
				settings[settingcounter] = std::atof(str.c_str());
				settingcounter += 1;
			}
		}

	}

	if (softbodymade == false)
		return;

	softbody->m_cfg.kPR = settings[0];


}

void TetraSolver::ExportBinary()
{

	std::string wpath = (path + "Solver" + std::to_string(counter) + ".bin");
	std::ofstream ofstream(wpath.c_str(), std::ios::out | std::ios::binary);
	for (int x=0; softbody->m_nodes.size() > x; x++)
	{
		if (softbody->m_nodes[x].cut == true)
			continue;
		btVector3 points = softbody->m_nodes[x].m_x;
		ofstream << std::to_string(points.x()) + "|" + std::to_string(points.y()) + "|" + std::to_string(points.z()) + "|";
	}
	ofstream.close();

}

void TetraSolver::ExportBinary3()
{

	std::string wpath = (path + "Solver" + std::to_string(counter) + ".bin");
	std::ofstream ofstream(wpath.c_str(), std::ios::out | std::ios::binary);
	for (int x=0; softbody->m_nodes.size() > x; x++)
	{
		btVector3 points = softbody->m_nodes[x].m_x;
		ofstream << std::to_string(points.x()) + "|" + std::to_string(points.y()) + "|" + std::to_string(points.z()) + "|";
	}
	ofstream.close();

}

void TetraSolver::ExportBinary2()
{

	std::string wpath = (path + "Solver" + std::to_string(counter) + ".bin");
	FILE *fp;
	fp = fopen(wpath.c_str(), "w");

	for (int x=0; softbody->m_nodes.size() > x; x++)
	{
		if (softbody->m_nodes[x].cut == true)
			continue;
		btVector3 points = softbody->m_nodes[x].m_x;
		fprintf(fp, (std::to_string(points.x()) + "|").c_str());
		fprintf(fp, (std::to_string(points.y()) + "|").c_str());
		fprintf(fp, (std::to_string(points.z()) + "|").c_str());
	}
	fclose(fp);


}

void TetraSolver::CutTest()
{

	scapelshape = new btBoxShape(btVector3(300,50,5));
	for (int i = 3; psb->m_nodes.size() > i; i+=4)
	{

		int i1 = i - 3;
		int i2 = i - 2;
		int i3 = i - 1;
		int i4 = i - 0;
		std::vector<btVector3> points;
		points.push_back(psb->m_nodes[i1].m_x);
		points.push_back(psb->m_nodes[i2].m_x);
		points.push_back(psb->m_nodes[i3].m_x);
		points.push_back(psb->m_nodes[i4].m_x);
		btConvexHullShape* shape = new btConvexHullShape((const btScalar*)points.data(), points.size());
		btTransform empty;
		empty.setIdentity();

		btCollisionObject objA;
		objA.setWorldTransform(empty);
		objA.setCollisionShape(shape);

		 
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(scapelpos);

		btCollisionObject objB;
		//objB.setWorldTransform(trans);
		objB.setCollisionShape(scapelshape);

		ContactResult result;
		dynamicsWorld->contactPairTest(&objA, &objB, result);
		if (result.m_connected == true)
		{
			psb->m_nodes[i1].cut = true;
			psb->m_nodes[i2].cut = true;
			psb->m_nodes[i3].cut = true;
			psb->m_nodes[i4].cut = true;

			btSoftBody::Node* index0 = &psb->m_nodes[i1];
			btSoftBody::Node* index1 = &psb->m_nodes[i2];
			btSoftBody::Node* index2 = &psb->m_nodes[i3];
			btSoftBody::Node* index3 = &psb->m_nodes[i4];

			for (int z = 0; psb->m_links.size() > z; z++)
			{
				if (psb->m_links[z].m_n[0] == index0 || psb->m_links[z].m_n[1] == index0)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}
				if (psb->m_links[z].m_n[0] == index1 || psb->m_links[z].m_n[1] == index1)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}
				if (psb->m_links[z].m_n[0] == index2 || psb->m_links[z].m_n[1] == index2)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}
				if (psb->m_links[z].m_n[0] == index3 || psb->m_links[z].m_n[1] == index3)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}				
			}
			for (int z = 0; psb->m_clusters.size() > z; z++)
			{
				if (psb->m_clusters[z]->m_nodes[0] == index0)
				{
					psb->releaseCluster(z);
				}
			}
  		}
	}




}

void TetraSolver::CutTest2()
{

	//scapelshape = new btBoxShape(btVector3(150,10,5));
	for (int i = 3; psb->m_nodes.size() > i; i+=4)
	{

		int i1 = i - 3;
		int i2 = i - 2;
		int i3 = i - 1;
		int i4 = i - 0;
		std::vector<btVector3> points;
		points.push_back(psb->m_nodes[i1].m_x);
		points.push_back(psb->m_nodes[i2].m_x);
		points.push_back(psb->m_nodes[i3].m_x);
		points.push_back(psb->m_nodes[i4].m_x);
		btConvexHullShape* shape = new btConvexHullShape((const btScalar*)points.data(), points.size());
		btTransform empty;
		empty.setIdentity();

		btCollisionObject objA;
		objA.setWorldTransform(empty);
		objA.setCollisionShape(shape);

		 
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(scapelpos);

		btCollisionObject objB;
		objB.setWorldTransform(trans);
		objB.setCollisionShape(scapelshape);


		ContactResult result;
		dynamicsWorld->contactPairTest(&objA, &objB, result);
		if (result.m_connected == true)
		{
			psb->m_nodes[i1].cut = true;
			psb->m_nodes[i2].cut = true;
			psb->m_nodes[i3].cut = true;
			psb->m_nodes[i4].cut = true;

			btSoftBody::Node* index0 = &psb->m_nodes[i1];
			btSoftBody::Node* index1 = &psb->m_nodes[i2];
			btSoftBody::Node* index2 = &psb->m_nodes[i3];
			btSoftBody::Node* index3 = &psb->m_nodes[i4];

			for (int z = 0; psb->m_links.size() > z; z++)
			{
				if (psb->m_links[z].m_n[0] == index0 || psb->m_links[z].m_n[1] == index0)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}
				if (psb->m_links[z].m_n[0] == index1 || psb->m_links[z].m_n[1] == index1)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}
				if (psb->m_links[z].m_n[0] == index2 || psb->m_links[z].m_n[1] == index2)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}
				if (psb->m_links[z].m_n[0] == index3 || psb->m_links[z].m_n[1] == index3)
				{
					btSwap(psb->m_links[z], psb->m_links[psb->m_links.size() - 1]);    psb->m_links.pop_back();
					continue;
				}				
			}
			for (int z = 0; psb->m_clusters.size() > z; z++)
			{
				if (psb->m_clusters[z]->m_nodes[0] == index0)
				{
					psb->releaseCluster(z);
				}
			}
  		}
	}




}

void TetraSolver::StepSimulation()
{

	LoadSettings();
	dynamicsWorld->stepSimulation(settings[0]);
	ExportBinary();
	//GetData();
	CutTest();
	counter += 1;

}

void TetraSolver::StepSimulation2()
{

	LoadSettings2();
	dynamicsWorld->stepSimulation(4.5);
	ExportBinary3();
	counter += 1;

}

void TetraSolver::GetData()
{

	std::ifstream file(path + "data.txt");
	if (file.is_open() == true)
	{
		std::string str;
		int settingcounter = 0;
		while (std::getline(file, str))
		{
			if (str[0] == '#') {
				str[0] = ' ';	
				std::istringstream buf(str);
				std::istream_iterator<std::string> beg(buf), end;
				std::vector<std::string> tokens(beg, end); // done!
				float px = std::atof(tokens[0].c_str());
				float py = std::atof(tokens[1].c_str());
				float pz = std::atof(tokens[2].c_str());
				scapelpos = btVector3(px, pz, py);		
			}
		}
	}



}
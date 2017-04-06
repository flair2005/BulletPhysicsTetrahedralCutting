#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <CL/cl.hpp>
#include <fstream>
#include <iterator>
#include <algorithm>

#define LOAD_FROM_MEMORY
#ifdef LOAD_FROM_MEMORY
#define MSTRINGIFY(A) #A
static const char* source =
#include "GPUCode.cl"
//static const char* fsource =
//#include "FoamCL.cl"
#endif //LOAD_FROM_MEMORY

struct CLVector4
{
	CLVector4(float x = 0, float y = 0, float z = 0) :x(x), y(y), z(z) 
	{
		w = 0;
	}
	float x;
	float y;
	float z;
	float w;
	bool operator==(CLVector4 vec)
	{
		return (x == vec.x && y == vec.y && z == vec.z);
	}
	CLVector4 minus(CLVector4 vec)
	{
		CLVector4 newvec;
		newvec.x = x;
		newvec.y = y;
		newvec.z = z;
		newvec.x -= vec.x;
		newvec.y -= vec.y;
		newvec.z -= vec.z;
		return newvec;
	}
	CLVector4 operator-(CLVector4 vec)
	{
		CLVector4 newvec;
		newvec.x = x;
		newvec.y = y;
		newvec.z = z;
		newvec.x -= vec.x;
		newvec.y -= vec.y;
		newvec.z -= vec.z;
		return newvec;
	}
	CLVector4 operator+(CLVector4 vec)
	{
		CLVector4 newvec;
		newvec.x = x;
		newvec.y = y;
		newvec.z = z;
		newvec.x += vec.x;
		newvec.y += vec.y;
		newvec.z += vec.z;
		return newvec;
	}
	CLVector4 operator*(float scalar)
	{
		CLVector4 vec;
		vec.x = x;
		vec.y = y;
		vec.z = z;

		vec.x *= scalar;
		vec.y *= scalar;
		vec.z *= scalar;
		return vec;
	}
	void operator+=(CLVector4 vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}
	void operator-=(CLVector4 vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}
	void operator*=(CLVector4 vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}
	void operator/=(CLVector4 vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
	}
	void operator*=(float vec)
	{
		x *= vec;
		y *= vec;
		z *= vec;
	}
	void operator/=(float vec)
	{
		x /= vec;
		y /= vec;
		z /= vec;
	}
	void operator-=(float vec)
	{
		x -= vec;
		y -= vec;
		z -= vec;
	}
	float distance(CLVector4 vec)
	{
		float newx = pow((vec.x - x), 2);
		float newy = pow((vec.y - y), 2);
		float newz = pow((vec.z - z), 2);
		float added = newx + newy + newz;
		return sqrt(added);
	}

};

struct CLVector3
{

	float x;
	float y;
	float z;
	float w;

};

struct CLVector9
{

	float x1;
	float y1;
	float z1;
	float x2;
	

};

class CLVec
{

public:
	CLVec()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	CLVec(CLVector4 vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	void minus(CLVector4 vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		
	}
	bool operator==(CLVec vec)
	{
		return (x == vec.x && y == vec.y && z == vec.z);
	}
	bool operator==(CLVector4 vec)
	{
		return (x == vec.x && y == vec.y && z == vec.z);
	}
	void operator=(CLVec vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	bool operator>(CLVec vec)
	{
		return (x > vec.x && y > vec.y && z > vec.z);
	}
	bool operator<(CLVec vec)
	{
		return (x < vec.x && y < vec.y && z < vec.z);
	}

	//bool operator>(CLVec vec

	float x;
	float y;
	float z;

};

struct CLVector2
{

	float x;
	float y;

};

struct CLInt3
{

	int x;
	int y;
	int z;

};

struct CLInt4
{

	int x;
	int y;
	int z;
	int w;

};

// CL 
static std::string GlobalKernel = "\n" \
"__kernel void SolveInstants(int pcount, __global float4* positions, __global int* instants) \n" \
"{ \n" \
"int x = get_global_id(0); \n" \
"if (x >= pcount) \n" \
"{ \n" \
"	return; \n" \
"} \n" \
"float4 vec = positions[x]; \n " \
"for (int i = 0; pcount > i; i++) \n " \
"{ \n " \
"	if (positions[x].x == positions[i].x && \n " \
"		positions[x].y == positions[i].y && \n " \
"		positions[x].z == positions[i].z) \n " \
"	{ \n" \
"		instants[x] += 1; \n" \
"	} \n" \
"} \n "\
"} \n";



class CLUniqueFinder
{

public:
	void InitCL()
	{

		//get all platforms (drivers)
		std::vector<cl::Platform> all_platforms;
		cl::Platform::get(&all_platforms);
		if (all_platforms.size() == 0) {
			std::cout << " No platforms found. Check OpenCL installation!\n";
			std::cin.get();
		}
		default_platform = all_platforms[0];
		std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

		//get default device of the default platform
		default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
		if (all_devices.size() == 0) {
			std::cout << " No devices found. Check OpenCL installation!\n";
			std::cin.get();
		}
		default_device = all_devices[0];
		std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

		context = cl::Context(default_device);

		// load opencl source
		std::ifstream cl_file(path + "GPUCode.cl");
		std::string cl_string(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));
		sources.push_back(
			std::make_pair(
				source,
				strlen(source)
			)	
		);

		program = cl::Program(context, sources);
		if (program.build(all_devices) != CL_SUCCESS) {
			std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
			std::cin.get();
		}


		// Particle Data
		int particleCount = positions.size();
		positionbuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(CLVector4) * particleCount);
		queue = cl::CommandQueue(context, default_device);
		queue.enqueueWriteBuffer(positionbuffer, CL_TRUE, 0, sizeof(CLVector4) * particleCount, positions.data());


	}
public:

	std::vector<CLVec> GPUMultipleInstances()
	{

		std::vector<CLVec> duplicates;
		int particleCount = positions.size();

		// instances
		positionbuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(CLVector4) * particleCount);
		instantbuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * particleCount);


		queue2 = cl::CommandQueue(context, default_device);
		queue2.enqueueWriteBuffer(positionbuffer, CL_TRUE, 0, sizeof(CLVector4) * particleCount, positions.data());

		for (int x = 0; particleCount > x; x++)
			CLInstants.push_back(0);

		queue2.enqueueWriteBuffer(instantbuffer, CL_TRUE, 0, sizeof(int) * particleCount, CLInstants.data());

		instances = cl::Kernel(program, "SolveInstants");
		instances.setArg(0, particleCount);
		instances.setArg(1, positionbuffer);
		instances.setArg(2, instantbuffer);

		queue2.enqueueNDRangeKernel(instances, cl::NullRange, cl::NDRange(particleCount), cl::NullRange);
		queue2.finish();

		int* instantints = new int[particleCount];
		queue2.enqueueReadBuffer(instantbuffer, CL_TRUE, 0, sizeof(int) * particleCount, instantints);

		CLInstants.clear();
		for (int i = 0; i < particleCount; i++) {
			CLInstants.push_back(instantints[i]);
		}

		for (int x = 0; CLInstants.size() > x; x++)
		{
			if (CLInstants[x] > 1)
			{
				duplicates.push_back(CLVec(positions[x]));
			}
		}

		std::sort(duplicates.begin(), duplicates.end());
		duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());
		return duplicates;

	}
	std::vector<CLVec> GPUMultipleInstances2(std::vector<int>& pindexs)
	{

		std::vector<CLVec> duplicates;
		int particleCount = positions.size();

		// instances
		positionbuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(CLVector4) * particleCount);
		instantbuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * particleCount);
		cl::Buffer instantbuffer2 = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * particleCount);


		queue2 = cl::CommandQueue(context, default_device);
		queue2.enqueueWriteBuffer(positionbuffer, CL_TRUE, 0, sizeof(CLVector4) * particleCount, positions.data());

		std::vector<int> CLInstants2;
		for (int x = 0; particleCount > x; x++) {
			CLInstants.push_back(0);
			CLInstants2.push_back(-1);
		}

		queue2.enqueueWriteBuffer(instantbuffer, CL_TRUE, 0, sizeof(int) * particleCount, CLInstants.data());
		queue2.enqueueWriteBuffer(instantbuffer2, CL_TRUE, 0, sizeof(int) * particleCount, CLInstants2.data());

		instances = cl::Kernel(program, "SolveInstants2");
		instances.setArg(0, particleCount);
		instances.setArg(1, positionbuffer);
		instances.setArg(2, instantbuffer);
		instances.setArg(3, instantbuffer2);

		queue2.enqueueNDRangeKernel(instances, cl::NullRange, cl::NDRange(particleCount), cl::NullRange);
		queue2.finish();

		int* instantints = new int[particleCount];
		queue2.enqueueReadBuffer(instantbuffer, CL_TRUE, 0, sizeof(int) * particleCount, instantints);

		int* instantints2 = new int[particleCount];
		queue2.enqueueReadBuffer(instantbuffer2, CL_TRUE, 0, sizeof(int) * particleCount, instantints2);


		CLInstants.clear();
		for (int i = 0; i < particleCount; i++) {
			CLInstants.push_back(instantints[i]);
		}

		CLInstants2.clear();
		for (int i = 0; i < particleCount; i++) {
			pindexs.push_back(instantints2[i]);
		}

		for (int x = 0; CLInstants.size() > x; x++)
		{
			if (CLInstants[x] > 1)
			{
				duplicates.push_back(CLVec(positions[x]));
			}
		}

		std::sort(duplicates.begin(), duplicates.end());
		duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());
		return duplicates;

	}
	std::vector<int> GPUMultipleInstances3()
	{

		std::vector<CLVec> duplicates;
		CLInstants.clear();
		int particleCount = positions.size();

		// instances
		positionbuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(CLVector4) * particleCount);
		instantbuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * particleCount);


		queue2 = cl::CommandQueue(context, default_device);
		queue2.enqueueWriteBuffer(positionbuffer, CL_TRUE, 0, sizeof(CLVector4) * particleCount, positions.data());

		for (int x = 0; particleCount > x; x++)
			CLInstants.push_back(0);

		queue2.enqueueWriteBuffer(instantbuffer, CL_TRUE, 0, sizeof(int) * particleCount, CLInstants.data());

		instances = cl::Kernel(program, "SolveInstants");
		instances.setArg(0, particleCount);
		instances.setArg(1, positionbuffer);
		instances.setArg(2, instantbuffer);

		queue2.enqueueNDRangeKernel(instances, cl::NullRange, cl::NDRange(particleCount), cl::NullRange);
		queue2.finish();

		int* instantints = new int[particleCount];
		queue2.enqueueReadBuffer(instantbuffer, CL_TRUE, 0, sizeof(int) * particleCount, instantints);

		CLInstants.clear();
		for (int i = 0; i < particleCount; i++) {
			CLInstants.push_back(instantints[i]);
		}

		return CLInstants;

	}
	std::vector<std::vector<int>> PairUpInstances(std::vector<CLVec> unique, std::vector<CLVector4> vecs)
	{

		std::vector<std::vector<int>> pair;
		pair.resize(vecs.size());

		for (int x = 0; vecs.size() > x; x++)
		{
			for (int i = 0; unique.size() > i; i++)
			{
				if (unique[i] == vecs[x])
				{
					pair[i].push_back(x);
				}
			}
		}

		return pair;

	}
	std::vector<int> CLInstants;
	std::vector<int> CPUMultipleInstances()
	{

		std::vector<int> instant;
		instant.resize(positions.size(), 0);
		for (int x = 0; positions.size() > x; x++)
		{
			for (int i = 0; positions.size() > i; i++)
			{
				if (positions[x] == positions[i])
				{
					instant[x] += 1;
				}
			}
		}

		return instant;


	}
	std::vector<int> CPUMultipleInstances2()
	{

		std::vector<int> instant;
		instant.resize(positions.size() / 3, 0);
		int counter = 0;
		for (int x = 2; positions.size() > x; x += 3)
		{
			for (int i = 2; positions.size() > i; i += 3)
			{
				if (positions[x - 2].x == positions[i - 2].x &&
					positions[x - 2].y == positions[i - 2].y &&
					positions[x - 2].z == positions[i - 2].z &&

					positions[x - 1].x == positions[i - 1].x &&
					positions[x - 1].y == positions[i - 1].y &&
					positions[x - 1].z == positions[i - 1].z &&

					positions[x - 0].x == positions[i - 0].x &&
					positions[x - 0].y == positions[i - 0].y &&
					positions[x - 0].z == positions[i - 0].z)
				{
					instant[counter] += 1;
				}
			}
			counter += 1;
		}

		return instant;


	}
	std::vector<CLVector4> positions;
	std::string path;
	
private:
	// OpenCL
	bool gpulookup;
	cl::Buffer positionbuffer;
	cl::Buffer velocitybuffer;
	cl::Buffer constraintbuffer;
	cl::Buffer lengthbuffer;
	cl::CommandQueue queue;
	cl::CommandQueue queue2;
	cl::Platform default_platform;
	cl::Device default_device;
	std::vector<cl::Device> all_devices;
	cl::Context context;
	cl::Program::Sources sources;
	cl::Program program;
	cl::Kernel solverconstraints;
	cl::Kernel instances;
	cl::Buffer instantbuffer;


};


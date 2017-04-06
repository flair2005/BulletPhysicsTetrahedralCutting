// Structs

MSTRINGIFY(

float adot3(float4 a, float4 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float alength(float4 a)
{
	//_asm _opencl e f 3k f2
	a.w = 0;
	return length(a);
}

float4 anormalize(float4 a)
{
	a.w = 0;
	return normalize(a);
}

float4 projectOnAxis(float4 v, float4 a)
{
	return (a*adot3(v, a));
}

__kernel void SolveInstants(int pcount, __global float4* positions, __global int* instants)
{

	int x = get_global_id(0);
	if (x >= pcount)
	{
		return;
	}

	float4 vec = positions[x];
	for (int i = 0; pcount > i; i++)
	{
		if (positions[x].x == positions[i].x &&
			positions[x].y == positions[i].y &&
			positions[x].z == positions[i].z)
		{
			instants[x] += 1;
		}
	}


}

__kernel void SolveInstants2(int pcount, __global float4* positions, __global int* instants, __global int* positionindex)
{

	int x = get_global_id(0);
	if (x >= pcount)
	{
		return;
	}

	float4 vec = positions[x];
	for (int i = 0; pcount > i; i++)
	{
		if (positions[x].x == positions[i].x &&
			positions[x].y == positions[i].y &&
			positions[x].z == positions[i].z)
		{
			instants[x] += 1;
			positionindex[x] = i;
		}
	}


}

__kernel void PairInstants(int pcount, __global float4* positions, __global int* instants)
{

	int x = get_global_id(0);
	if (x >= pcount)
	{
		return;
	}






}

);
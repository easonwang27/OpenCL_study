#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define VECTOR_SIZE 1024
int main(void)
{
	int i;
	//Allocate space for vectors A,B,andc
	float alpha =2.0;
	float *A = (float*)malloc(sizeof(float)*VECTOR_SIZE)
	float *B = (float*)malloc(sizeof(float)*VECTOR_SIZE)
	float *C = (float*)malloc(sizeof(float)*VECTOR_SIZE)
	for(i = 0;i < VECTOR_SIZE;i++)
	{
		A[i] = i;
		B[i] = VECTOR_SIZE-i;
		C[i] = 0;
	}
	cl_platform_id *platforms;
	cl_int num_platforms;
	cl_int  clStatus;
	//set up the platforms
	clStatus = clGetPlatformIDs(0,NULL,&num_platforms);
	printf("this num_platforms =%d\n",num_platforms);
	platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id)*num_platforms);
	clStatus = clGetPlatformIDs(num_platforms,platforms,NULL);
	if(clStatus < 0)
	{
		perror("clGetPlatformIDs failed!!!!\n");
		exit(1);
	}
	//get the devices list and choose
	cl_device_id *devices_list =NULL;
	cl_uint       num_devices;
	clStatus = clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_CPU,0,NULL,&num_devices);
	if(clStatus < 0)
	{
		perror("clGetPlatformIDs step 1 failed\n");
		exit(1);
	}
	devices_list = (cl_device_id *)malloc(sizeof(cl_device_id)*num_devices);
	clStatus = clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_CPU,num_devices,devices_list,NULL);
	if(clStatus < 0)
	{
		perror("clGetPlatformIDs step 1 failed\n");
		exit(1);
	}
	printf("num_devices = %d\n",num_devices);
	//creat one opencl context
	cl_context context;
	context = clCreateContext(NULL,num_devices,devices_list,NULL,NULL,&clStatus);
	if(clStatus < 0)
	{
		perror("clCreateContext error!\n");
		exit(1);
	}

}

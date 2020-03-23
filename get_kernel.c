#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>
int main(void)
{
	int i;
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


}

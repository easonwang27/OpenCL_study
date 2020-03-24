#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define PROGRAM_FILE "get_kernel.cl"
#define VECTOR_SIZE 1024

int main(void)
{
	int i;
    FILE *program_handle;
	//Allocate space for vectors A,B,andc

	size_t program_size, log_size;
    char *program_buffer, *program_log;
	float alpha =2.0;

	float *A = (float*)malloc(sizeof(float)*VECTOR_SIZE);
	float *B = (float*)malloc(sizeof(float)*VECTOR_SIZE);
	float *C = (float*)malloc(sizeof(float)*VECTOR_SIZE);

	for(i = 0;i < VECTOR_SIZE;i++)
	{
		A[i] = i;
		B[i] = VECTOR_SIZE-i;
		C[i] = 0;
	}
	cl_platform_id *platforms;
	cl_int num_platforms;
	cl_int  clStatus;
	const char options[] = "-cl-std=CL1.1 -cl-mad-enable -Werror";
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
	program_handle = fopen(PROGRAM_FILE, "r");
   if(program_handle == NULL) {
      perror("Couldn't find the program file");
      exit(1);
   }
   fseek(program_handle, 0, SEEK_END);
   program_size = ftell(program_handle);
   rewind(program_handle);
   program_buffer = (char*)malloc(program_size + 1);
   program_buffer[program_size] = '\0';
   fread(program_buffer, sizeof(char), program_size, program_handle);
   fclose(program_handle);
	//create a command queue
	cl_command_queue command_queue = clCreateCommandQueue(context,devices_list[0],0,&clStatus);
	if(clStatus < 0)
	{
		perror("clCreateCommandQueue error!\n");
		exit(1);
	}
	//create memory buffers on the device for each vector

	cl_mem A_clmem = clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(float)*VECTOR_SIZE,NULL,&clStatus);
	cl_mem B_clmem = clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(float)*VECTOR_SIZE,NULL,&clStatus);
	cl_mem C_clmem = clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(float)*VECTOR_SIZE,NULL,&clStatus);
	if(clStatus < 0)
	{
		perror("clCreateBuffer error!\n");
		exit(1);
	}
	//copy buffer A and B to device
	clStatus = clEnqueueWriteBuffer(command_queue,A_clmem,CL_TRUE,0,VECTOR_SIZE*sizeof(float),
			A,0,NULL,NULL);

	clStatus = clEnqueueWriteBuffer(command_queue,B_clmem,CL_TRUE,0,VECTOR_SIZE*sizeof(float),
			B,0,NULL,NULL);

	cl_program  program = clCreateProgramWithSource(context,1,(const char **)&program_buffer,&program_size,&clStatus);

	clStatus = clBuildProgram(program,1,devices_list,options,NULL,NULL);
	if(clStatus < 0)
	{
		clGetProgramBuildInfo(program,devices_list,CL_PROGRAM_BUILD_LOG,0,NULL,&log_size);
		program_log = (char*)calloc(log_size+1,sizeof(char));
		clGetProgramBuildInfo(program,devices_list,CL_PROGRAM_BUILD_LOG,log_size+1,program_log,NULL);
		printf("%s\n",program_log);
		free(program_log);
	}
	cl_kernel kernel = clCreateKernel(program,"saxpy_kernel",&clStatus);
	if(clStatus < 0)
	{
		perror("create kernel fail!\n");
	}
	//set the argumemts of the kernel
	clStatus = clSetKernelArg(kernel,0,sizeof(float),(void *)&alpha);
	clStatus = clSetKernelArg(kernel,1,sizeof(float),(void *)&A_clmem);
	clStatus = clSetKernelArg(kernel,2,sizeof(float),(void *)&B_clmem);
	clStatus = clSetKernelArg(kernel,3,sizeof(float),(void *)&C_clmem);
	//excute the OpenCL kernel on the list
	size_t global_size =VECTOR_SIZE;
	size_t local_size =64;
	clStatus = clEnqueueNDRangeKernel(command_queue,kernel,1,NULL,&global_size,&local_size,0,NULL,NULL);
	//read the cl memory c_clmem  on device to the host variable C
	clStatus = clEnqueueReadBuffer(command_queue,C_clmem,CL_TRUE,0,VECTOR_SIZE*sizeof(float),C,0,NULL,NULL);

	clStatus  = clFlush(command_queue);
	clStatus =clFinish(command_queue);

	for(i = 0; i< VECTOR_SIZE;i++)
	{
		printf("%f * %f +%f = %f\n",alpha,A[i],B[i],C[i]);
	}

	clStatus = clReleaseKernel(kernel);
	clStatus = clReleaseProgram(program);
	clStatus = clReleaseMemObject(A_clmem);
	clStatus = clReleaseMemObject(B_clmem);
	clStatus = clReleaseMemObject(C_clmem);
	clStatus = clReleaseCommandQueue(command_queue);
	clStatus = clReleaseContext(context);
	free(A);
	free(B);
	free(C);
	free(platforms);
	free(devices_list);
	return 0;
}

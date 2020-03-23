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

}

#pragma OPENCL EXTENSION all : enable


__kernel void dummy_kernel_1(__global int *src1, __global int *src2, __global int *dst)
{
    int gidx = get_global_id(0);
    int gidy = get_global_id(1);

    int lidx = get_local_id(0);
    int lidy = get_local_id(1);

    int indexa = 16;
    int indexb = 18;

   __local int smem[16][18];
   __local int smem2[16][18], smem3[16][18];
   __local int smem4[16][18]; __local int smem5[16][18];

   /* Simple 2d array */
   int nosmem[20][20];

   smem[0][0] = src1[10];
   indexa = indexb;
}

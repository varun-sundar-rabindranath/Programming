
__kernel void dummy_kernel(__global int src1[200][200], __global int *src2, __global int *dst)
{
    int gidx = get_global_id(0);
    int gidy = get_global_id(1);

    int lidx = get_local_id(0);
    int lidy = get_local_id(1);

    __local int smem[16][18];
    smem[0][0] = src1[0][0]+lidy;
}

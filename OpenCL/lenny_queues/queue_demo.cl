// input q1: pointer to Qeueu 1
// input q2 pointer to Qeueu 2
// input q_atomic: pointer to Queue indices of two


#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
__kernel void Queue(global uint* q1, global uint* q2,
                    global unsigned int * q_atomic) {

    uint gid = get_global_id(0);
    uint lid = get_local_id(0);
    uint choice = 0;

    __local unsigned int lqidx[2];
    
    if (lid < 2)
        lqidx[lid] = 0;
    barrier(CLK_LOCAL_MEM_FENCE);

    __global uint* q = q1;

    __local int *p_lqidx = lqidx;

    // if/else
    if (gid & 1) { /// just a random if...
        p_lqidx = lqidx + 1;
        q = q2;
        choice = 1;
    }

    unsigned int lidx = atomic_inc(p_lqidx);
    barrier(CLK_LOCAL_MEM_FENCE);
    if( lid < 2)
        lqidx[lid] = atomic_add(q_atomic+lid, lqidx[lid]);

    barrier(CLK_LOCAL_MEM_FENCE);
    (q + lqidx[choice])[lidx] = gid;
}

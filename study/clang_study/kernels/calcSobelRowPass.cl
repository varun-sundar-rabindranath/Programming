#define cn 1
#define loadpix(addr) convertToIntT(vload3(0, (__global const uchar *)(addr)))
#define storepix(val, addr) vstore3(convertToShortT(val), 0, (__global short *)(addr))
#define shortSize (int)sizeof(short) * cn

// Smoothing perpendicular to the derivative direction with a triangle filter
// only support 3x3 Sobel kernel
// h (-1) = 1, h (0) = 2, h (1) = 1
// h'(-1) = -1, h'(0) = 0, h'(1) = 1
// thus sobel 2D operator can be calculated as:
// h'(x, y) = h'(x)h(y) for x direction
//
// src input 8bit single channel image data
// dx_buf output dx buffer
// dy_buf output dy buffer

__kernel void calcSobelRowPass(__global const uchar * src, int src_step, int src_offset, int rows, int cols,
                               __global uchar * dx_buf, int dx_buf_step, int dx_buf_offset,
                               __global uchar * dy_buf, int dy_buf_step, int dy_buf_offset)
{
    int gidx = get_global_id(0);
    int gidy = get_global_id(1);

    int lidx = get_local_id(0);
    int lidy = get_local_id(1);

    __local intT smem[16][18];

    smem[lidy][lidx + 1] = loadpix(src + mad24(src_step, min(gidy, rows - 1), mad24(gidx, cn, src_offset)));
    if (lidx == 0)
    {
        smem[lidy][0] = loadpix(src + mad24(src_step, min(gidy, rows - 1), mad24(max(gidx - 1, 0), cn, src_offset)));
        smem[lidy][17] = loadpix(src + mad24(src_step, min(gidy, rows - 1), mad24(min(gidx + 16, cols - 1), cn, src_offset)));
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    if (gidy < rows && gidx < cols)
    {
        storepix(smem[lidy][lidx + 2] - smem[lidy][lidx],
                 dx_buf + mad24(gidy, dx_buf_step, mad24(gidx, shortSize, dx_buf_offset)));
        storepix(mad24(2, smem[lidy][lidx + 1], smem[lidy][lidx] + smem[lidy][lidx + 2]),
                 dy_buf + mad24(gidy, dy_buf_step, mad24(gidx, shortSize, dy_buf_offset)));
    }
}

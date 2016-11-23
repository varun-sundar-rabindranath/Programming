#pragma OPENCL_EXTENSION cl_clang_storage_class_specifiers : enable

int main()
{
  __local int smem[10][10];
  return 0;
}

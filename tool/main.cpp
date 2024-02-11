#include <iostream>
#include <vcopy/vcopy.h>


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      vcopy_main(cnt, arr)
#endif

int main(int argc, const char **argv) 
{
  return 0;
}

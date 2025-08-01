
#include <vcopy/vcopy.h>
#include <cstdio>
#include <cstdlib>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      vcopy_test_cpp_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char **argv)
{
	fprintf(stderr, "VCopy Test (C++ version)\n\n");

	return 0;
}


#include <eternal_timestamp.h>
#include <stdio.h>
#include <stdlib.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      eternalty_test_c_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	fprintf(stderr, "Eternal Timestamp Test (C version)\n\n");

	return 0;
}

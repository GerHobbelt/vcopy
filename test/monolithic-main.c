
#include "monolithic_examples.h"

#define USAGE_NAME   "eternal"

#include "monolithic_main_internal_defs.h"

MONOLITHIC_CMD_TABLE_START()
	{ "test_c", { .fa = eternalty_test_c_main } },
	{ "test_cpp", { .fa = eternalty_test_cpp_main } },
MONOLITHIC_CMD_TABLE_END();

#include "monolithic_main_tpl.h"

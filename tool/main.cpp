#include <iostream>
#include <vcopy/vcopy.h>


#if defined(BUILD_MONOLITHIC)
#define main       vcopy_main
#endif

// load the monolithic definitions which you need for the dispatch table:
#include "monolithic_main_internal_defs.h"

// load the monolithic core init APIs.
#include "monolithic_main_appinit_tpl.h"

int main(int argc, const char **argv) 
{
	int rv = setup_exe_for_monitor_dpi_etc();
	rv |= setup_exe_for_utf8_console_locale();
	if (rv) {
		fprintf(stderr, "application/locale setup (monitor DPI + console UTF8) failed to comply: code %d. Aborting the application.\n", rv);
		return rv;
	}
	return 0;
}

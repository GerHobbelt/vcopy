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

#if 0

#include <glob/glob.h>
#include <glob/version.h>

#include <clipp.h>
#include <iostream>
#include <string>
#include <set>

#include <ghc/fs_std.hpp>  // namespace fs = std::filesystem;   or   namespace fs = ghc::filesystem;

#include "monolithic_examples.h"


static int test()
{
	std::vector<fs::path> testpaths{
		"C:\\users\\abcdef\\AppData\\Local\\Temp\\",
		"/home/user/.config/Cppcheck/Cppcheck-GUI.conf",
		"~/a/b/.config/Cppcheck/Cppcheck-GUI.conf",
		"~mcFarlane/.config/Cppcheck/Cppcheck-GUI.conf",
		"base/~mcFarlane/.config/Cppcheck/Cppcheck-GUI.conf",
		"Cppcheck/Cppcheck-GUI.conf",
		"../../Cppcheck/Cppcheck-GUI.conf",
		"..\\..\\Cppcheck\\Cppcheck-GUI.conf",
		"Z:\\Cppcheck\\Cppcheck-GUI.conf",
		"\\\\?:\\Cppcheck\\Cppcheck-GUI.conf",
		"./Cppcheck/Cppcheck-GUI.conf",
		"Cppcheck-GUI.conf",
		"./Cppcheck-GUI.conf"
	};

	for (const auto& p : testpaths) {
		std::cout << "Examining the path " << p << " through iterators gives\n";
		std::cout << p.root_directory() << " |RN " << p.root_name() << " |RP " << p.root_path() << " |PP " << p.parent_path() << " |FN " << p.filename() << " |EX " << p.extension() << " |ST " << p.stem() << " |0 " << *(p.begin()) << " |1 " << *(++p.begin()) << " ||\n";
		for (auto it = p.begin(); it != p.end(); ++it) {
			std::cout << *it << " | ";
		}
		std::cout << '\n';
	}

	return EXIT_SUCCESS;
}


#if defined(BUILD_MONOLITHIC)
#define main     glob_standalone_main
#endif

int main(int argc, const char** argv)
{
	using namespace clipp;

	bool recursive = false;
	std::vector<std::string> patterns;
	std::set<std::string> tags;
	std::string basepath;
	enum class mode { none, help, version, glob, test };
	mode selected = mode::none;

	auto options = (
		option("-r", "--recursive").set(recursive) % "Run glob recursively",
		repeatable(option("-i", "--input").set(selected, mode::glob) & values("patterns", patterns)) % "Patterns to match",
		option("-b", "--basepath").set(basepath) % "Base directory to glob in"
	);
	auto cli = (
		(options
			| command("-h", "--help").set(selected, mode::help) % "Show this screen."
			| command("-t", "--test").set(selected, mode::test) % "Run the glob system tests."
			| command("-v", "--version").set(selected, mode::version) % "Show version."
			),
		any_other(patterns).set(selected, mode::glob)
	);

	auto help = [cli]()
		{
			std::cerr << make_man_page(cli, "glob")
				.prepend_section("DESCRIPTION", "    Run glob to find all the pathnames matching a specified pattern")
				.append_section("LICENSE", "    MIT");
		};

	parse(argc, argv, cli);
	switch (selected)
	{
	default:

	case mode::none:
	case mode::help:
		help();
		return EXIT_SUCCESS;

	case mode::test:
		return test();

	case mode::version:
		std::cout << "glob, version " << GLOB_VERSION << std::endl;
		return EXIT_SUCCESS;

	case mode::glob:
		break;
	}

	if (patterns.empty())
	{
		help();
		return EXIT_SUCCESS;
	}

	try
	{
		if (recursive)
		{
			if (!basepath.empty())
			{
				for (auto& match : glob::rglob_path(basepath, patterns))
				{
					std::cout << match << "\n";
				}
			} else
			{
				for (auto& match : glob::rglob(patterns))
				{
					std::cout << match << "\n";
				}
			}
		} else
		{
			if (!basepath.empty())
			{
				for (auto& match : glob::glob_path(basepath, patterns))
				{
					std::cout << match << "\n";
				}
			} else
			{
				for (auto& match : glob::glob(patterns))
				{
					std::cout << match << "\n";
				}
			}
		}
	}
	catch (fs::filesystem_error &ex)
	{
		std::cerr << "glob/filesystem error " << ex.code().value() << ": " << ex.code().message() << " :: " << ex.what() << " (path: '" << ex.path1().string() << "')" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#endif

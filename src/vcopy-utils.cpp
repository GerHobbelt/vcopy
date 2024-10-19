
#include <ghc/fs_std.hpp>  // namespace fs = std::filesystem;   or   namespace fs = ghc::filesystem;
#include <dirent.h>  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include <glob/glob.h>

#include "vcopy/vcopy.h"
#include "vcopy-priv.h"

namespace vcopy
{

#if 0

void dirscan_dirent()
{
		/* Pass the pattern to filter function */
		filter_pattern = pattern;

		/* Scan files in directory */
		struct dirent **files;
		int n = scandir(dirname, &files, filter, alphasort);
		if (n < 0) {
			/* Cannot open directory */
			return 0;
		}

		/* Loop through file names */
		for (int i = 0; i < n; i++) {
			/* Get pointer to file entry */
			struct dirent *ent = files[i];

			/* Output file name */
			switch (ent->d_type) {
			case DT_REG:
				printf("%s\n", ent->d_name);
				break;

			case DT_DIR:
				printf("%s/\n", ent->d_name);
				break;

			case DT_LNK:
				printf("%s@\n", ent->d_name);
				break;

			default:
				printf("%s*\n", ent->d_name);
			}
		}

		/* Release file names */
		for (int i = 0; i < n; i++) {
			free(files[i]);
		}
		free(files);

		/* Success */
		return 1;
}

	/*
	 * Compare directory entry to pattern and return 1 if pattern matches the
	 * entry.
	 */
	static int
		filter(const struct dirent *entry)
	{
		return match(entry->d_name, filter_pattern);
	}

	/* Compare name to a pattern and return 1 if pattern matches the name */
	static int
		match(const char *name, const char *patt)
	{
		do {
			switch (*patt) {
			case '\0':
				/* Only end of string matches NUL */
				return *name == '\0';

			case '/':
			case '\\':
			case ':':
				/* Invalid pattern */
				return 0;

			case '?':
				/* Any character except NUL matches question mark */
				if (*name == '\0')
					return 0;

				/* Consume character and continue scanning */
				name++;
				patt++;
				break;

			case '*':
				/* Any sequence of characters match asterisk */
				switch (patt[1]) {
				case '\0':
					/* Trailing asterisk matches anything */
					return 1;

				case '*':
				case '?':
				case '/':
				case '\\':
				case ':':
					/* Invalid pattern */
					return 0;

				default:
					/* Find the next matching character */
					while (*name != patt[1]) {
						if (*name == '\0')
							return 0;
						name++;
					}

					/* Terminate sequence on trailing match */
					if (match(name, patt + 1))
						return 1;

					/* No match, continue from next character */
					name++;
				}
				break;

			default:
				/* Only character itself matches */
				if (*patt != *name)
					return 0;

				/* Character passes */
				name++;
				patt++;
			}
		} while (1);
		/*NOTREACHED*/
	}



#if defined(BUILD_MONOLITHIC)
#define main		dirent_scandir_main
#endif

	/* Stub for converting arguments to UTF-8 on Windows */
#if defined(_MSC_VER) && !defined(BUILD_MONOLITHIC)
	int
		wmain(int argc, const wchar_t *argv[])
	{
		/* Select UTF-8 locale */
		setlocale(LC_ALL, ".utf8");
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);

		/* Allocate memory for multi-byte argv table */
		char **mbargv;
		mbargv = (char**)malloc(argc * sizeof(char*));
		if (!mbargv) {
			puts("Out of memory");
			exit(3);
		}

		/* Convert each argument in argv to UTF-8 */
		for (int i = 0; i < argc; i++) {
			size_t n;
			wcstombs_s(&n, NULL, 0, argv[i], 0);

			/* Allocate room for ith argument */
			mbargv[i] = (char*)malloc(n + 1);
			if (!mbargv[i]) {
				puts("Out of memory");
				exit(3);
			}

			/* Convert ith argument to utf-8 */
			wcstombs_s(NULL, mbargv[i], n + 1, argv[i], n);
		}

		/* Pass UTF-8 converted arguments to the main program */
		int errorcode = _main(argc, mbargv);

		/* Release UTF-8 arguments */
		for (int i = 0; i < argc; i++) {
			free(mbargv[i]);
		}

		/* Release the argument table */
		free(mbargv);
		return errorcode;
	}
#else
	int
		main(int argc, const char **argv)
	{
		return _main(argc, argv);
	}
#endif


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

#endif

}

#include "vcopy/vcopy.h"
#include "vcopy-priv.h"

// cp -vHLR --preserve=timestamps ../../500GB_Movies_01/[Amr]* ./
// chown -R ger.ger .
// chmod -R -c a+rw .

namespace vcopy
{
	/*
	   Commands / options:

		 --conf <configuration-file>            : loads a configuration file, which serves as an alternative method to feed the tool its set of parameters.

		                                          Note: the tool will always look in the current active directory for a 'default' configuration file.
																							The order of precedence is:
																							+ configuration file in currently active directory (a.k.a. present working directory, pwd, cwd)
																							+ configuration file(s) specified at the command line
																							+ command line arguments themselves

																							Command line options which accept file paths and directories may occur multiple times and *accumulate*.
																							Consequently they *accumulate* what already is, i.e. append the set of paths already loaded from the
																							configuration files.

																							When you don't want this behaviour, i.e. wish to start from scratch, you can do so by specifying the
																							option prefixed with `clear-`, e.g. `--clear-skip`, which will remove all 'skip' paths collected until
																							the moment the `--clear-skip` option is observed. As a corollary, this can also be used to clear out
																							settings from pre-written command scripts/aliases: `--skip A --clear-skip --skip B` will discard 'A'
																							as that path occurred *before* the `clear-skip` command, resulting in only the 'B' path being preserved.
																							`--skip A --skip B` will accumulate, i.e. preserve the set {A, B}.

																							As this 'it applies in order of occurrence' rule is followed through rigorously, the 'order of precedence'
																							listed above is not entirely correct: the configuration file(s) specified via `--conf` at the command
																							line are applied *in order* with the rest of the command line options, i.e. the configuration file is
																							loaded and processed in line with the other command line options. The order of precedence table is an
																							over-simplification of this behaviour as usually configuration files are specified before the other
																							command line options.
																							Example: assuming the configuration file CFG only carries a 'clear-skip' command and config file PATH
																							only specifies a skip path 'C-A', then these command lines will produce the following path sets as a
																							result:

																							--conf PATH --skip A --skip B                      --> {C-A, A,B}
																							--conf CFG --skip A --skip B                       --> {A,B}
																							--conf PATH --conf CFG --skip A --skip B           --> {A,B}
																							--skip A --conf CFG --skip B                       --> {B}
																							--conf PATH --skip A --conf CFG --skip B           --> {B}
																							--skip A --conf CFG                                --> {}   (empty set)

		 --skip <directories>                     similar to the 'ignore' set, but the search/scan can be optimized by completely skipping these directories.
		                                          Meanwhile the 'ignore' set is more useful to indicate particular file name patterns which do not interest us
																							(and should therefore be ignored).
		 --ignore <files>;<directories>           file and directory names and patterns (we do accept wildcards in these, of course) which should be ignored
		                                          at both sides of the fence: source *and* destination.
		 --ignore-source <files>;<directories>
		                                          ditto as `--ignore` but only applies to the source side of things.
		 --ignore-destination <files>;<directories>
		                                          ditto as `--ignore` but only applies to the destination side of things.
     --source <directories>                   one or more 'base directories' serving as source trees for the operation.
		 --match-depth <n>                        the maximum depth the filesystem scan will traverse each source directory. This number can adjusted
		                                          for specific subtrees by using the `match` command for those:
     --match {<depth>:} <directories>         when any of these paths are matched, the max traversal depth is adjusted/reset to the given depth.
		                                          When no explicit depth number is specified, the actual traversal depth limit is simply reset to the
																							`match-depth` number: this behaviour makes it easy to scan a large directory tree at a relatively shallow
																							depth (which should be fast) and then match a few subdirectories of interest in there, which will be
																							automatically scanned to a deeper level.
																							Indeed, when, while scanning such a subdirectory tree, any other `match` pattern matches, the traversal
																							depth limit is reset again for that sub-sub-tree!

																							When the inverse is desired, i.e. a relatively deep general scan, but only a shallow traversal of
																							specific sub-directories, then one could specify a larger `match-depth` number to start with, and specify
																							a much lower depth level for those `match`-ed sub-directories: the 'depth limit' will be set to the
																							specified `depth` number, even when the current depth limit happened to be higher.
     --destination <directory>                       
     --target <directory>                     the target 'base' directory where everything, as collected from the `sources` directory trees, will be
		                                          compared against / copied / moved to.

     --forced-rewrite [<paths>]
		 --overwrite      [<paths>]
		 --update         [<paths>]
		 --only-existing  [<paths>]
		 --only-orphaned  [<paths>]
		 --only-widowed   [<paths>]
																							overwrite / update the files found at the destination. Optionally a set of paths/file-patterns may be
																							specified to have this behaviour only apply to the given set. When no paths/patterns are specified, the
																							given rule applies to all.
																							This allows us to specify a diversified copy/write behaviour in a single execution/run, e.g.
																							`--update --overwrite F` will instruct us to *update* all files/directories at the destination, except
																							for the files which match the F pattern/name: those will be forcibly overwritten, even when the
																							destination copy appears to be up to date already. Use this when you need to use certain files as
																							'signal files' for various purposes.

																							Do note that `update`, `overwrite` and `only-existing` may attempt to optimize the copy/move action
																							by updating only the necessary parts of the target file a la `rsync` et al, minimizing write activity.
																							Also note that these options will direct vcopy to replace any target file it finds wanting by overwriting
																							its content.
																							`forced-overwrite` does not do any of this: the destination file is re-created from scratch (via
																							a dedicated create-rename-delete-rename action sequence) which is meant to break any hardlink or softlink
																							the previous destination file might have been part of.

																							If you also wish to keep the create+change timestamps as found at the source, then you must also specify
																							the `copy-attributes` command.

		 --copy-attributes {<spec>:] [<paths>]    copy the given file access timestamps and attributes to the destination.         

		 --database <dbfile>                      keep a database of the file paths, content hashes, timestamps, etc. around. This can be used speed
		                                          up subsequent runs as any source file matching a database entry will not be content-hashed for update
																							comparison: instead, the data is taken from the matching database record.

																							As using the obviously safest way of matching a file in there, using its content hash, is obviously
																							thwarting our performance optimization goal here, a file is matched using its path, size and timestamp
																							attributes: when those match any database record, we assume a content match.
																							If this does not suit you, e.g. when you need/want a stronger destination-equals-source-content guarantee,
																							you can specify an `ignore-database` command.

     --ignore-database [<paths>]              ignore the database performance shortcut for these files / directories. When no paths patterns are specified,
		                                          the command is applied to all files involved.

	   --delete-obsolete-targets                delete any files found at the destination site which are not present in the source tree. This is a useful
		                                          command option when mirroring large source repositories which see frequent significant change in files
																							appearing and disappearing from the collective: use this option to ensure no 'old cruft' lingers unnoticed.

     --ignore-obsolete-targets                default behaviour when `delete-obsolete-targets` is not specified.

		 --create-target-dir                      create the base target / destination directory when it does not exist yet. Think UNIX `mkdir -p`.

		 --move
		 --copy
		 --test
		 --report [<reportfile>]
		 --diff   [<targetdirectory>]
		 --delete
		 --rename
																							the various commands we support. `copy` is obvious; `move` comes with the notion that the source tree is
																							*first* copied over to the destination (unless the vcopy tool discovers the file can be *moved* at
																							*file system API level*, e.g. when source and destination trees exist in the same filesystem/drive.
																							`move` will only delete the source files ones those have all been successfully produced at the destination
																							site.

																							`test` is a bit like `make -n`: all the prep-work is done, but rather than performing the actual copy/move/delete,
																							the tool will print/report the planned action for review and nothing will change. `report` is an alias
																							of `test` which can write the planned actions in a useful format to the given report file, e.g. for further
																							processing that action list by external tools.

																							`diff` is like `report` but only reports the actions required due to observed differences between source and
																							destination. Also note that `diff` creates a copy of each differing file at the given `targetdirectory`: use
																							`diff` when you want a copy of the differences between directory trees A (source) and B (target), copied into
																							`targetdirectory`.

																							`delete` is another special which applies its actions to the source trees and does not require any 'target'
																							path.

																							`rename` is most useful for reorganizing and/or sanitizing a directory tree (see also the `sanitize` option),
																							otherwise it might be a no-op.

    --verify                                  explicitly verify the identicality of the source and target file contents, before reporting a success, deleting
		                                          the source file (as part of the 'move' operation) or other action which might otherwise loose you your original
																							content.

																							As a corollary, `verify` does not perform a content-hash comparison but a 1:1 all-bytes file content
																							comparison instead. Handy when you feel a wee bit paranoid about your target medium, e.g. an USB disk or
																							file storage that sits on the other side of an Internet connection (though the latter might be better served
																							using `rsync` or similar tooling instead).

    --sanitize [<paths>]                      sanitize any matching file and directory name that's to be created/updated during this run.


		General note: while path-carrying options can be 'reset' using the `clear-` prefix, the same can be done for the other command line options,
		resetting them to their built-in default values.
		Meanwhile you can also *invert* / *countermand* the boolean command line switches by using the `no-` or 'dont-' prefixes, e.g. `no-verify` will shut down
		the maximum effort comparing file content at both source and destination, trusting the copying process instead.
	*/


}

# `vcopy` -- safe copy

Copy files / directory trees while using write locks, content verify, and the option to use / check / publish content checksum, ...

# Goal 

Safely copy files across various (local) hardware under circumstances where there may be another file writer active at the same time and/or the (USB?) connection is sometimes flakey or system I/O drivers buggered[^1].

# Extras: *SAFE<sup>2</sup>* operation

As we will use this one for our *Qiqqa(R) Sync* process among other things, "mirroring" local document libraries to arbitrary local-portable/remote storage, this tool has the additional feature of **encryption**: files can be encrypted while being copied, thus making `vcopy` the endpoint for a *SAFE*<sup>2</sup> end-to-end sharing/backup system: 

**SAFE<sup>2</sup>** =

1. **_SAFE_<sub>0</sub> data transfer**: the file copies are *verified* to ensure their content remains undamaged / not corrupted.
2. **_SAFE_<sub>1</sub> from prying eyes**: when encryption/decrypion is chosen, the files will be encrypted/decrypted on the fly while copying them to/from the 'unsafe' destination.

`vcopy` also has built-in counter-measures to ensure that incoming files, which turn out to be corrupted (i.e. not match their published checksums), DO NOT overwrite your local copy: we deem it better to keep your -- possily older -- existing data file than have it blown out of the water by incoming corruption. After all, the sync/fetch can be re-tried later, when the remote side's issue has been fixed; meanwhile, your own data is not *gone*.


# Usage

Command line tool.


# See also

For networked / remote storage access, please check out these tools:

- rsync / unison
- rclone (for DropBox, Google Drive, et al)
- zsync  (alternative; for DropBox, Google Drive, OneDrive, MegaNZ, ...)


# Technology info

`vcopy` targets scenarios where both source and destination of the copy action are "addressable" by regular file I/O operations, i.e. both sides are mounted disks, which are both accessable by standard tools such as `cp` (UNIX) and `robocopy` (MS Windows). When you want to cross network / SSH boundaries which do *not* match this condition, then we assume you'll be using `unison`, `rsync` or similar tools in the next part of your copy/mirror/backup pipeline, while we assume you intend to keep a local 'cache' of the exact same data as is kept at that far-away remote site: `vcopy` can make sure your local 'cache' is A-okay.


----


[^1]: happened to me. Lost several TB of stock material while a possibly buggered Windows 10 rig joined forces with crummy GigaByte hardware to permanently nuke large tracts of data on my external drives. That *hurt*. To top it off, I discovered while using `teracopy` (with verify enabled) during the recovery from that (yet incopmpletely diagnosed :cry: ) disaster, that *sometimes*, *if only rarely*, copied data would verify as *corrupted* at the destination drive. `teracopy` helped recover from that situation, but we need something more universal, OS-wise, for our Qiqqa(R) 'sync path'...


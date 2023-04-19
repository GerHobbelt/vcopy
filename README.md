# `vcopy` -- safe copy

Copy files / directory trees while using write locks, content verify, and the option to use / check / publish content checksum, ...

# Goal 

Safely copy files across various (local) hardware under circumstances where there may be another file writer active at the same time and/or the (USB?) connection is sometimes flakey or system I/O drivers buggered[^1].

# Usage

Command line tool.


# See also

For networked / remote storage access, please check out these tools:

- rsync / unison
- rclone (for DropBox, Google Drive, et al)
- zsync  (alternative; for DropBox, Google Drive, OneDrive, MegaNZ, ...)
-


----


[^1]: happened to me. Lost several TB of stock material while a possibly buggered Windows 10 rig joined forces with crummy GigaByte hardware to permanently nuke large tracts of data on my external drives. That *hurt*. To top it off, I discovered while using `teracopy` (with verify enabled) during the recovery from that (yet incopmpletely diagnosed :cry: ) disaster, that *sometimes*, *if only rarely*, copied data would verify as *corrupted* at the destination drive. `teracopy` helped recover from that situation, but we need something more universal, OS-wise, for our Qiqqa(R) 'sync path'...


# TODO :: Features

## delayed delete on move, but do not wait until the end of the run a la `mv`

GNU/Unix `mv` doesn't seem to delete files until the very end of the run. **We** should delete files almost immediately after having copied them. **Almost** here meaning: delayed by queue: the intent here is that when we get a system / disk crash (I've had it happen enough times by now) and the last part of the filesystem update didn't make it through, you loose the files as the copy/write didn't yet complete entirely, while the *delete* at the other end, of course (Murphy's Law) went through A-okay, so that file (or *files*, plural) got lost into the eternal void.
`sync/flush` won't deliver here as experience shows that internal disk caches, etc. are not always touched by sync, so this is a bit of a "playing with loaded guns" option... The idea is that we can specify a delay in terms of either MBytes written or number of files moved, before an already moved file is popped from the to-be-deleted-at-the-source queue and actually deleted at the source.
Hm, might want to combine that with a flush at the write side, just for improved chances of not shooting ourselves in the foot.



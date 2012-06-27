=============
 ext2-parser
=============

Navigating the ext2-fs data-structures.


==========================
  1. Introduction 
==========================

The headers are borrowed from project ext2reader.
http://www.cs.washington.edu/education/courses/cse451/09sp/projects.html


==========================
  2. Building
==========================
- Build binaries (ext cp).
$ make

- Delete all generated files.
$ make clean


==========================
  3. Running
==========================

- Running ext

$ ./ext <ext2-file.img> [inode-num]

The above command lists the contents of the filesystem contained in the .img
 file passed as the first parameter. The second parameter is optional, if NOT
 passed, ext will parse and list the directory structure of "/" i.e root. If
 an inode-num is passed, ext attempts to parse the particular inode and list
 the contents of that directory.

- Running cp

$ ./cp <ext2-file.img> <inode-num> <dst-filename>

The above command copies the contents of the file present at the inode and
 saves the contents in a local-file(outside ext2-file.img) by the name given
 in dst-filename.

==========================
  4. Version History
==========================
- ext.c to parse and list directory structure.
  Syntax: ./ext <img-file> [inode-num]
- cp.c to copy the contents of file out of the img file.
  Syntax: ./cp <img-file> <src-inode-num> <dst-filename>

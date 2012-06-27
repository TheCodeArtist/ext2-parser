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
$make
- Build binaries.

$make clean
- Delete all generated files.


==========================
  3. Version History
==========================
- ext.c to parse and list directory structure.
  Syntax: ./ext <img-file> [inode-num]
- cp.c to copy the contents of file out of the img file.
  Syntax: ./cp <img-file> <src-inode-num> <dst-filename>

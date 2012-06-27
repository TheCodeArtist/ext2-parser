// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu)
// May 22nd, 2009
//
// This file defines the structure of an ext2 directory entry.
//
// Quoting from http://www.nongnu.org/ext2-doc/ext2.html:
//
//// A directory is a filesystem object and has an inode just like a
//// file. It is a specially formatted file containing records which
//// associate each name with an inode number. Later revisions of the
//// filesystem also encode the type of the object (file, directory,
//// symlink, device, fifo, socket) to avoid the need to check the
//// inode itself for this information.
////
//// Also note that as directory grows additional blocks are assigned
//// to store the additional file records. When filenames are removed,
//// some implementations do not free these additional blocks.
//
// In this project, we're using ext2 revision 0, and therefore the
// "linked list" directory style, rather than the indexed directory
// style.

#ifndef EXT2READER_INC_DIRECTORYENTRY_H
#define EXT2READER_INC_DIRECTORYENTRY_H

#include "types.h"

// Constants used in some of the directory entry fields.
#define EXT2_NAME_LEN 255
#define EXT2_FT_UNKNOWN   0  //  Unknown File Type
#define EXT2_FT_REG_FILE  1  //  Regular File
#define EXT2_FT_DIR       2  //  Directory File
#define EXT2_FT_CHRDEV    3  //  Character Device
#define EXT2_FT_BLKDEV    4  //  Block Device
#define EXT2_FT_FIFO      5  //  Buffer File
#define EXT2_FT_SOCK      6  //  Socket File
#define EXT2_FT_SYMLINK   7  //  Symbolic Link

// This structure represents a directory entry.  Note that the "name"
// field is actually variable length on disk; for the purposes of this
// structure, we'll assume the worst case of EXT2_NAME_LEN bytes.

struct os_direntry_t {
  // The 32-bit inode number of the file referred to by this directory
  // entry.  If the inode number is zero, it means this directory entry
  // is invalid, and should be "ignored".  ext2 uses a directory
  // entry with a zero inode number as a terminator of the linked list
  // of directory entries within a directory file.
  os_uint32_t inode;

  // The 16-bit unsigned displacement of the next directory entry
  // within the directory file, relative to the start of this
  // directory entry.  This field will have a value at least
  // equal to the length of the current record, but perhaps longer,
  // if some space in the directory file is going unused.
  os_uint16_t rec_len;

  // The 8-bit unsigned value indicating how many bytes of character
  // data are in the name.  This value must be never be larger than
  // rec_len - 8.
  os_uint8_t name_len;

  // The 8-bit unsigned value indicating the file's type.  Can be
  // any one of the following:
  //
  // EXT2_FT_UNKNOWN   0  Unknown File Type
  // EXT2_FT_REG_FILE  1  Regular File
  // EXT2_FT_DIR       2  Directory File
  // EXT2_FT_CHRDEV    3  Character Device
  // EXT2_FT_BLKDEV    4  Block Device
  // EXT2_FT_FIFO      5  Buffer File
  // EXT2_FT_SOCK      6  Socket File
  // EXT2_FT_SYMLINK   7  Symbolic Link
  os_uint8_t file_type;

  // The file's name itself.  File names will be padded with NULL
  // characters (i.e., value 0) so that there are always a multiple of
  // 4 characters in a directory entry.  But, only the first
  // "name_len" characters are actually part of the file name, and you
  // cannot rely on there being a NULL character in the entry (e.g.,
  // if the file name actually is a perfect multiple of 4).
  os_uint8_t file_name[EXT2_NAME_LEN];
};

#endif // EXT2READER_INC_DIRECTORYENTRY_H

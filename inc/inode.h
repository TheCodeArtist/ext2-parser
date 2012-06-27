// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu)
// May 22nd, 2009
//
// This file defines the structure of an ext2 inode.
//
// Quoting from http://www.nongnu.org/ext2-doc/ext2.html:
//
//// The inode (index node) is a fundamental concept in the ext2
//// filesystem. Each object in the filesystem is represented by an
//// inode. The inode structure contains pointers to the filesystem
//// blocks which contain the data held in the object and all of the
//// metadata about an object except its name.
////
//// There are pointers to the first 12 blocks which contain the
//// file's data in the inode. There is a pointer to an indirect block
//// (which contains pointers to the next set of blocks), a pointer to
//// a doubly-indirect block (which contains pointers to indirect
//// blocks) and a pointer to a trebly-indirect block (which contains
//// pointers to doubly-indirect blocks). 
////
//// All the inodes are stored in inode tables, with one inode table
//// per block group. 

#ifndef EXT2READER_INC_INODE_H
#define EXT2READER_INC_INODE_H

#include "types.h"


// These constants define the inode number of various special
// inodes (such as the inode of the root directory) within an
// ext2 file system.  You'll only need to understand and use
// EXT2_ROOT_INO.

#define EXT2_BAD_INO          1  // Bad blocks inode
#define EXT2_ROOT_INO         2  // Root directory inode
#define EXT2_ACL_IDX_INO      3  // ACL index inode (deprecated)
#define EXT2_ACL_DATA_INO     4  // ACL data inode (deprecated)
#define EXT2_BOOT_LOADER_INO  5  // boot loader inode
#define EXT2_UNDEL_DIR_INO    6  // undelete directory inode

// Here are other constants used within the inode structure itself.

#define EXT2_S_IFSOCK 0xC000
#define EXT2_S_IFLNK  0xA000
#define EXT2_S_IFREG  0x8000
#define EXT2_S_IFBLK  0x6000
#define EXT2_S_IFDIR  0x4000
#define EXT2_S_IFCHR  0x2000
#define EXT2_S_IFIFO  0x1000
#define EXT2_S_ISUID  0x0800
#define EXT2_S_ISGID  0x0400
#define EXT2_S_ISVTX  0x0200
#define EXT2_S_IRUSR  0x0100
#define EXT2_S_IWUSR  0x0080
#define EXT2_S_IXUSR  0x0040
#define EXT2_S_IRGRP  0x0020
#define EXT2_S_IWGRP  0x0010
#define EXT2_S_IXGRP  0x0008
#define EXT2_S_IROTH  0x0004
#define EXT2_S_IWOTH  0x0002
#define EXT2_S_IXOTH  0x0001

// This structure represents an inode.  In ext2, an inode is 128 bytes.

struct os_inode_t {
  // a 16 bit value used to indicate the format of the file and access
  // rights.  Here are the possible values, which can be combined
  // in various ways:
  //
  //  -- file format --
  //  EXT2_S_IFSOCK   0xC000  socket
  //  EXT2_S_IFLNK    0xA000  symbolic link
  //  EXT2_S_IFREG    0x8000  regular file
  //  EXT2_S_IFBLK    0x6000  block device
  //  EXT2_S_IFDIR    0x4000  directory
  //  EXT2_S_IFCHR    0x2000  character device
  //  EXT2_S_IFIFO    0x1000  fifo
  //  -- process execution user/group override --
  //  EXT2_S_ISUID    0x0800  Set process User ID
  //  EXT2_S_ISGID    0x0400  Set process Group ID
  //  EXT2_S_ISVTX    0x0200  sticky bit
  //  -- access rights --
  //  EXT2_S_IRUSR    0x0100  user read
  //  EXT2_S_IWUSR    0x0080  user write
  //  EXT2_S_IXUSR    0x0040  user execute
  //  EXT2_S_IRGRP    0x0020  group read
  //  EXT2_S_IWGRP    0x0010  group write
  //  EXT2_S_IXGRP    0x0008  group execute
  //  EXT2_S_IROTH    0x0004  others read
  //  EXT2_S_IWOTH    0x0002  others write
  //  EXT2_S_IXOTH    0x0001  others execute
  //
  os_uint16_t i_mode;

  // 16-bit user id associated with the file.
  os_uint16_t i_uid;

  // 32-bit value indicating the size of the file in bytes
  // (revision 0).  In revision 1 and later, this represents the lower
  // 32 bits of the file size (upper 32 bits are in i_dir_acl).
  os_uint32_t i_size;

  // 32-bit value representing the unix time (# of seconds since the
  // epoch, january 1st 1970) of the last time this inode was
  // accessed.
  os_uint32_t i_atime;

  // 32-bit value representing the unix time (# of seconds since the
  // epoch, january 1st 1970) of when this inode was created.
  os_uint32_t i_ctime;

  // 32-bit value representing the unix time (# of seconds since the
  // epoch, january 1st 1970) of the last time this inode was
  // modified.
  os_uint32_t i_mtime;

  // 32-bit value representing the unix time (# of seconds since the
  // epoch, january 1st 1970) of when this inode was deleted.
  os_uint32_t i_dtime;

  // 16-bit value of the group id having access to this file.
  os_uint16_t i_gid;

  // 16-bit value indicating how many times this inode is linked
  // (i.e., referred to).  Most files will have a link count of
  // 1. Files with hard links pointing to them will have an additional
  // count for each hard link.  Symbolic links do not affect the link
  // count of an inode.  When the link count reaches 0, the inode and
  // all of its associated blocks are deleted.
  os_uint16_t i_links_count;

  // 32-bit value representing the total # of blocks reserved to
  // contain data of this inode, regardless of whether these blocks
  // are used.  The block numbers of these are contained in the
  // i_block array.
  os_uint16_t i_blocks;

  // 32-bit value indicating flags controlling how the filesystem
  // should access the data of this node.
  //
  // To see the flags that are defined, and their values and meaning,
  // refer to this web page:
  //     http://www.nongnu.org/ext2-doc/ext2.html#I-FLAGS
  //
  os_uint32_t i_flags;

  // 32-bit value that is os-dependent.  in linux, is reserved.
  // this looks complicated, but a union is simple -- it defines
  // multiple different ways to represent the same region of
  // the structure.  So, you could reference the field in any
  // of the following ways, all of which refer to the same
  // 32 bit value in this structure:
  //   (variable).i_osdi1.linux1.l_i_reserved1;
  union {
    struct {
      os_uint32_t  l_i_reserved1;
    } linux1;
    struct {
      os_uint32_t  h_i_translator;
    } hurd1;
    struct {
      os_uint32_t  m_i_reserved1;
    } masix1;
  } i_osdi1;

  // an array of 15 32-bit blocknumbers pointing to the blocks
  // containing data.  The first 12 entries in the array
  // (i_block[0]-iblock[11) contain the block numbers of data blocks
  // (i.e., are "direct" entries). The 13th entry (i_block[12])
  // contains the block number of the first indirect block, which
  // itself contains (blocksize / 4) block numbers of data blocks.
  // The 14th entry (i_block[13]) contains the block number of 
  // the first doubly-indirect block.  The 15th entry (i_block[14]) is
  // the block number of the first trebly-indirect block.
  os_uint32_t i_block[15];

  // 32-bit value used to indicate the file version (used by NFS).
  os_uint32_t i_generation;

  // 32-bit value indicating the block number containing extended
  // attributes.  With revision 0 of ext2 (which we're using in this
  // class), this value is always 0.
  os_uint32_t i_file_acl;

  // 32-bit value.  With revision 0 of ext2, this is always 0.
  // With revision 1, it contains the high 32 bits of the 64bit file
  // size.  
  os_uint32_t i_dir_acl;

  // 32-bit value indicating the location of the file fragment.
  // in linux, fragments are unsupported, so this value is always 0.
  os_uint32_t i_faddr;

  // the next 96-bits are os-dependant structures.   We don't
  // really have to worry about these, since the linux version
  // is about fragmentation, and revision 0 of ext2 does not
  // use fragmentation.  So, this is safe to ignore.
  union {
    struct {
      os_uint8_t    l_i_frag;       /* Fragment number */
      os_uint8_t    l_i_fsize;      /* Fragment size */
      os_uint16_t   i_pad1;
      os_uint16_t   l_i_uid_high;   /* these 2 fields    */
      os_uint16_t   l_i_gid_high;   /* were reserved2[0] */
      os_uint32_t   l_i_reserved2;
    } linux2;
    struct {
      os_uint8_t    h_i_frag;       /* Fragment number */
      os_uint8_t    h_i_fsize;      /* Fragment size */
      os_uint16_t   h_i_mode_high;
      os_uint16_t   h_i_uid_high;
      os_uint16_t   h_i_gid_high;
      os_uint32_t   h_i_author;
    } hurd2;
    struct {
      os_uint8_t    m_i_frag;       /* Fragment number */
      os_uint8_t    m_i_fsize;      /* Fragment size */
      os_uint16_t   m_pad1;
      os_uint32_t   m_i_reserved2[2];
    } masix2;
  } i_osd2;
};

#endif // EXT2READER_INC_INODE_H

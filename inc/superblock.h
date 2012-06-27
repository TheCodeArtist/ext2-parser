// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu)
// May 22nd, 2009
//
// This file defines the structure of an ext2 superblock, as well
// as a bunch of constants used in fields within the superblock.
// You wll read the superblock from disk into this in-memory
// structure.
//
// Quoting from http://www.nongnu.org/ext2-doc/ext2.html:
//
//// The superblock contains all the information about the configuration
//// of the filesystem. The information in the superblock contains fields
//// such as the total number of inodes and blocks in the filesystem
////and how many are free, how many inodes and blocks are in each
//// block group, when the filesystem was mounted (and if it was
//// cleanly unmounted), when it was modified, what version of the
//// filesystem it is and which OS created it.
////
//// The primary copy of the superblock is stored at an offset of 1024
//// bytes from the start of the device, and it is essential to
//// mounting the filesystem. Since it is so important, backup copies
//// of the superblock are stored in block groups throughout the
//// filesystem.
////
//// The first version of ext2 (revision 0) stores a copy at the start
//// of every block group, along with a backup of the group descriptor
//// table block(s). Because this can consume a considerable amount of
//// space for large filesystems, later revisions can optionally reduce
//// the number of backup copies by only putting backups in specific
//// groups (this is the sparse superblock feature). The groups chosen
//// are 0, 1 and powers of 3, 5 and 7.
//
// For 451, we'll be using a disk image that is formatted according to
// revision 0, so every block group will have a superblock and a copy
// of the group descriptor table.

#ifndef EXT2READER_INC_SUPERBLOCK_H
#define EXT2READER_INC_SUPERBLOCK_H

#include "types.h"

// The following constants are used in some of the superblock fields.
// Search for them in the comments to find out what they mean.
#define EXT2_SUPER_MAGIC 0xEF53
#define EXT2_VALID_FS 1
#define EXT2_ERROR_FS 2
#define EXT2_ERRORS_CONTINUE 1
#define EXT2_ERRORS_RO 2
#define EXT2_ERRORS_PANIC 3
#define EXT2_OS_LINUX 0
#define EXT2_OS_HURD 1
#define EXT2_OS_MASIC 2
#define EXT2_OS_FREEBSD 3
#define EXT2_OS_LITES 4
#define EXT2_GOOD_OLD_REV 0
#define EXT2_DYNAMIC_REV 1
#define EXT2_DEF_RESUID 0
#define EXT2_DEF_RESGID 0
#define EXT2_GOOD_OLD_FIRST_INO 11
#define EXT2_GOOD_OLD_INODE_SIZE 128
#define EXT2_FEATURE_COMPAT_DIR_PREALLOC 0x0001
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES 0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL 0x0004
#define EXT2_FEATURE_COMPAT_EXT_ATTR 0x0008
#define EXT2_FEATURE_COMPAT_RESIZE_INO 0x0010
#define EXT2_FEATURE_COMPAT_DIR_INDEX 0x0020
#define EXT2_FEATURE_INCOMPAT_COMPRESSION 0x0001
#define EXT2_FEATURE_INCOMPAT_FILETYPE 0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER 0x0004
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV 0x0008
#define EXT2_FEATURE_INCOMPAT_META_BG 0x0010
#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER 0x0001
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE 0x0002
#define EXT2_FEATURE_RO_COMPAT_BTREE_DIR 0x0004
#define EXT2_LZV1_ALG 0x00000001
#define EXT2_LZRW3A_ALG 0x00000002
#define EXT2_GZIP_ALG 0x00000004
#define EXT2_BZIP2_ALG 0x00000008
#define EXT2_LZO_ALG 0x00000010

// The on-disk representation of a superblock.  Note that everything
// on disk is stored little endian, just like x86, so you don't need
// to worry about endianness-conversion on x86.
//
// The field names and comments are largely taken from
//
//     http://www.nongnu.org/ext2-doc/ext2.html
//
// You should find that sizeof(struct os_superblock_t) == 1024.

struct os_superblock_t {
  // total # of inodes, used and free, in system
  os_uint32_t s_inodes_count;

  // total # of blocks, used and free, in system
  os_uint32_t s_blocks_count;

  // total # of blocks reserved for superuser
  os_uint32_t s_r_blocks_count;

  // total # of free blocks, including s_r_blocks_count reserved blocks
  os_uint32_t s_free_blocks_count;

  // total # of free inodes
  os_uint32_t s_free_inodes_count;

  // First data block location -- i.e., id of the block containing
  // the superblock.  Is 0 for filesystems with block size > 1KB, or
  // 1 for file systems with block size of 1KB.  Remember that the
  // superblock always starts at the 1024th byte on the disk.
  os_uint32_t s_first_data_block;

  // log_2(blocksize/1024).  So, blocksize is computed as:
  //
  //   os_uint32_t blocksize = 1024 << s_log_block_size;
  os_uint32_t s_log_block_size;

  // shift of log_2(fragmentsize/1024).  So, fragmentsize is computed as:
  //
  //   os_uint32_t fragmentsize =
  //       (s_log_frag_size > 0) ?
  //           (1024 << s_log_frag_size) : (1024 >> -s_log_frag_size);
  os_int32_t s_log_frag_size;

  // number of blocks per group.  Combined with s_first_data_block,
  // you can use this to figure out the block group boundaries.
  os_uint32_t s_blocks_per_group;

  // number of fragments per group.  Also can be used to determine the
  // size of the block bitmap of each block group.  In ext2, the fragment
  // size is always the same as the block size -- fragmented blocks are
  // not yet implemented.
  os_uint32_t s_frags_per_group;

  // number of inodes per group.  Also can be used to determine the
  // size of the inode bitmap of each block group.
  os_uint32_t s_inodes_per_group;

  // the last time the filesystem was mounted, in "UNIX time" (# seconds
  // since epoch).
  os_uint32_t s_mtime;

  // the last time the file system was written to, in "UNIX time" (#
  // seconds since epoch).
  os_uint32_t s_wtime;

  // the # times the file system was mounted since the last time it
  // was verified (i.e., since fsck has been run)
  os_uint16_t s_mnt_count;

  // the max # of times the file system may be mounted before
  // a full check is performed
  os_uint16_t s_max_mnt_count;

  // a "magic number" identifying the file system as type Ext2.
  // this value is set to EXT2_SUPER_MAGIC, which has value 0xEF53
  os_uint16_t s_magic;

  // the file system state -- EXT2_VALID_FS means the filesystem
  // was last unmounted cleanly, while EXT2_ERROR_FS means the
  // filesystem was not unmounted cleanly, and likely has errors
  os_uint16_t s_state;

  // ext2 file system version information -- minor revision level
  os_uint16_t s_minor_rev_level;

  // the last time a consistency check was run on the filesystem; time
  // is UNIX time, i.e., seconds since the last epoch
  os_uint32_t s_lastcheck;

  // the maximum time (in seconds) allowed between filesystem checks
  os_uint32_t s_checkinterval;

  // the identifier of the operating system that created this FS.
  // could be EXT2_OS_LINUX, EXT2_OS_HURD, EXT2_OS_MASIC,
  // EXT2_OS_FREEBSD, or EXT2_OS_LITES.
  os_uint32_t s_creator_os;

  // ext2 file system version information -- major revision level.
  // could be either EXT2_GOOD_OLD_REV or EXT2_DYNAMIC_REV.
  os_uint32_t s_rev_level;

  // the default userID for reserved blocks.  defaults to
  // EXT2_DEF_RESUID, which has value 0 (i.e., root).
  os_uint16_t s_def_resuid;

  // the default group ID for reserved blocks.  defaults to
  // EXT2_DEF_RESGID, which has value 0 (i.e., root).
  os_uint16_t s_def_resgid;

  // the first, non-reserved inode usable for standard files.
  // in EXT2_GOLD_OLD_REV version, this is set to
  // value 11 (EXT2_GOOD_OLD_FIRST_INO).  In revisions 1 or later,
  // can be anything.
  os_uint32_t s_first_ino;

  // the size of an inode.  In revision 0, this is
  // EXT2_GOOD_OLD_INODE_SIZE (128 bytes).  In later revisions, can
  // be any power of 2 less than or equal to the block size
  // (i.e., s_inode_size <= 1<<s_log_block_size;
  os_uint16_t s_inode_size;

  // the block group number hosting this superblock.  can be used
  // to rebuild the filesystem from any superblock backup.
  os_uint16_t s_block_group_nr;

  // a 32bit bitmask of compatible features.  By compatible, we
  // mean that the filesystem can support these or not without
  // any risk of damaging metadata.  The features are:
  // 
  //  EXT2_FEATURE_COMPAT_DIR_PREALLOC (0x0001):
  //          Block pre-allocation for new directories
  //  EXT2_FEATURE_COMPAT_IMAGIC_INODES (0x0002):
  //          Has "magic" inodes.
  //  EXT3_FEATURE_COMPAT_HAS_JOURNAL (0x0004):
  //          An Ext3 journal exists
  //  EXT2_FEATURE_COMPAT_EXT_ATTR (0x0008):
  //          Extended inode attributes are present
  //  EXT2_FEATURE_COMPAT_RESIZE_INO (0x0010):
  //          Non-standard inode size used
  //  EXT2_FEATURE_COMPAT_DIR_INDEX (0x0020):
  //          Directory indexing (HTree)
  //
  // So, for example, if the filesystem has a journal and extended
  // attributes, then:
  //   s_feature_compat =  EXT2_FEATURE_COMPAT_HAS_JOURNAL &
  //                       EXT2_FEATURE_COMPAT_EXT_ATTR;
  //
  os_uint32_t s_feature_compat;

  // a 32bit bitmask of incompatible features, i.e., if they are
  // present they will affect the filesystem metadata.  The file
  // system implementation should refuse to mount the filesystem if
  // any of the indicated features are unsupported.
  // (steve gribble): I haven't tracked down the meaning of all of
  // these, but the filesystem image we use in 451 has none of them.
  //
  // EXT2_FEATURE_INCOMPAT_COMPRESSION (0x0001):
  //       Disk/File compression is used
  // EXT2_FEATURE_INCOMPAT_FILETYPE (0x0002)
  // EXT3_FEATURE_INCOMPAT_RECOVER (0x0004)
  // EXT3_FEATURE_INCOMPAT_JOURNAL_DEV (0x0008)
  // EXT2_FEATURE_INCOMPAT_META_BG (0x0010)
  //
  os_uint32_t s_feature_incompat;

  // a 32bit bitmask of read-only features.  The filesystem should
  // mount as read-only if any of these are unsupported.
  //
  // EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER (0x0001):
  //      sparse superblock
  // EXT2_FEATURE_RO_COMPAT_LARGE_FILE (0x0002):
  //      Large file support, 64-bit file size
  // EXT2_FEATURE_RO_COMPAT_BTREE_DIR (0x0004):
  //      Binary tree sorted directory files
  os_uint32_t s_feature_ro_compat;

  // a 128 bit value used as the unique volume ID.  every file system
  // in the world should be given a unique volume ID, if possible.
  os_uint8_t s_uuid[16];

  // a null-terminated volume name.  only ISO-Latin-1
  // characters may be used.  this is a rarely used feature.
  os_uint8_t s_volume_name[16];

  // the path (null-terminated string) where the filesystem was last
  // mounted.  could be used for auto-finding the mount point.  often
  // is left as the empty string, i.e., not used by some implementations.
  os_uint8_t s_last_mounted[64];

  // if this volume is compressed (see s_feature_compat), this bitfield
  // tells you the compression algorithms (yes, plural!) used.
  //
  // EXT2_LZV1_ALG (0x00000001)
  // EXT2_LZRW3A_ALG (0x00000002)
  // EXT2_GZIP_ALG (0x00000004)
  // EXT2_BZIP2_ALG (0x00000008)
  // EXT2_LZO_ALG (0x00000010)
  os_uint32_t s_algo_bitmap;

  // 8-bit value representing the number of blocks the implementation
  // should pre-allocate when creating a new regular file.  typically
  // only used with ext4 filesystems.  usually "0" for pre-ext4
  // filesystems.
  os_uint8_t s_prealloc_blocks;

  // 8-bit value representing the number of blocks the implementation
  // should pre-allocated when creating a new directory.  again,
  // only really used in ext4, and only if the
  // EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is present in the
  // s_feature_compat field.
  os_uint8_t s_prealloc_dir_blocks;

  // two bytes of padding
  os_uint16_t s_padding_1;

  // 16 byte value containing the UUID of the journal superblock, if
  // this filesystem has an ext3 journal.
  os_uint8_t s_journal_uuid[16];
  
  // the 32 bit inode number of the journal file.
  os_uint32_t s_journal_inum;

  // the 32-bit device number of the journal file.
  os_uint32_t s_journal_dev;

  // a 32-bit inode number, pointing to the first inode in the list
  // of inodes to delete.  (see documentation on ext3 journaling for
  // information -- safe to ignore for non-ext3 systems).
  os_uint32_t s_last_orphan;

  // an array of 4 32-bit values containing seeds used for the
  // hash algorithm for directory indexing, if this is used.
  os_uint32_t s_hash_seed[4];

  // an 8-bit value containing the default hash version used for
  // directory indexing.
  os_uint8_t s_def_hash_version;

  // 3 bytes of padding, reserved for future expansion
  os_uint8_t s_padding_2[3];

  // a 32-bit value containing the default mount options for this
  // filesystem.
  os_uint32_t s_default_mount_options;

  // a 32-bit value indicating the block group ID of the first
  // meta-block group.  (ext3-only extension, I believe).
  os_uint32_t s_first_meta_bg;

  // unused -- reserved for future revisions
  os_uint8_t s_unused[760];
};

#endif // EXT2READER_INC_SUPERBLOCK_H
